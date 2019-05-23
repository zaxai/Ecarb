#include "stdafx.h"
#include "CActivation.h"


CActivation::CActivation()
{
}


CActivation::~CActivation()
{
}


CString CActivation::GetSerialNumber()
{
	CString strSerialNumber;
	std::vector<CString> vec_stUUID = ZUtil::GetWMIData(_T("SELECT * FROM Win32_ComputerSystemProduct WHERE UUID IS NOT NULL"), _T("UUID"));
	if (vec_stUUID.size())
	{
		if (vec_stUUID[0] != _T("00000000-0000-0000-0000-000000000000"))
		{
			strSerialNumber = vec_stUUID[0];
			return strSerialNumber;
		}
	}
	std::vector<CString> vec_stCPU = ZUtil::GetWMIData(_T("SELECT * FROM Win32_Processor WHERE ProcessorId IS NOT NULL"), _T("ProcessorId"));
	std::vector<CString> vec_stMAC = ZUtil::GetWMIData(_T("SELECT * FROM Win32_NetworkAdapter WHERE MACAddress IS NOT NULL"), _T("MACAddress"));
	if (vec_stCPU.size() && vec_stMAC.size())
	{
		vec_stMAC[0].Remove(_T(':'));
		strSerialNumber = vec_stCPU[0].Right(8) + _T('-') + vec_stMAC[0];
	}
	return strSerialNumber;
}


bool CActivation::Verify(const CString & strSerialNumber, const CString & strActivationFile)
{
	const int nBuffer = 128;
	int nCiphertextBySN, nCiphertextByFile;
	char sz_cCiphertextBySN[nBuffer] = { 0 }, sz_cCiphertextByFile[nBuffer] = { 0 };
	if (!GetCiphertextBySN(strSerialNumber, sz_cCiphertextBySN, nBuffer, nCiphertextBySN))
		return false;
	if (!GetCiphertextByFile(strActivationFile, sz_cCiphertextByFile, nBuffer, nCiphertextByFile))
		return false;
	if (nCiphertextBySN != nCiphertextByFile)
		return false;
	return (!memcmp(sz_cCiphertextBySN, sz_cCiphertextByFile, nCiphertextBySN));
}


bool CActivation::GetCiphertextBySN(CString strSerialNumber, char * p_cCiphertext, int nBuffer,int & nCiphertextLen)
{
	const int nMix = 9;
	const char sz_cMix[nMix] = { 6,8,16,28,56,88,106,118,126 };
	if (strSerialNumber.IsEmpty())
		return false;
	strSerialNumber.Remove(_T('-'));
	CString strCiphertext(strSerialNumber);
	strCiphertext.MakeReverse();
	strCiphertext += strSerialNumber;
	if (!ZUtil::TtoA(strCiphertext, p_cCiphertext, nBuffer))
		return false;
	nCiphertextLen = strlen(p_cCiphertext);
	for (int i = 0; i < nCiphertextLen; ++i)
	{
		p_cCiphertext[i] = ~p_cCiphertext[i];
		p_cCiphertext[i] += sz_cMix[i % nMix];
	}
	return true;
}


bool CActivation::GetCiphertextByFile(CString strActivationFile, char * p_cCiphertext, int nBuffer, int & nCiphertextLen)
{
	if (strActivationFile.IsEmpty())
		return false;
	CStdioFile f;
	if (!f.Open(strActivationFile, CFile::modeRead | CFile::typeBinary))
		return false;
	int nCount = f.Read(p_cCiphertext, nBuffer);
	if (!nCount)
		return false;
	if (p_cCiphertext[0] < 5 || p_cCiphertext[0]>15)
		return false;
	if (p_cCiphertext[nCount - 1] < 5 || p_cCiphertext[nCount - 1]>15)
		return false;
	nCiphertextLen = nCount - p_cCiphertext[0] - p_cCiphertext[nCount - 1];
	if (nCiphertextLen < 0)
		return false;
	int nOffset = p_cCiphertext[0];
	for (int i = 0; i < nCiphertextLen; ++i)
		p_cCiphertext[i] = p_cCiphertext[i+ nOffset];
	return true;
}


bool CActivation::CreateActivationFile(const CString & strSerialNumber, const CString & strActivationFile)
{
	const int nBuffer = 128;
	int nCiphertextLen;
	char sz_cCiphertext[nBuffer] = { 0 };
	if (!GetCiphertextBySN(strSerialNumber, sz_cCiphertext, nBuffer, nCiphertextLen))
		return false;
	::srand(unsigned int(time(nullptr)));
	char cFirst, cLast, sz_cStart[nBuffer] = { 0 }, sz_cEnd[nBuffer] = { 0 };
	cFirst = ::rand() % 11 + 5;
	cLast = ::rand() % 11 + 5;
	sz_cStart[0] = cFirst;
	for (int i = 1; i < cFirst; ++i)
	{
		sz_cStart[i] = ::rand() % 256;
	}
	for (int i = 0; i < cLast - 1; ++i)
	{
		sz_cEnd[i] = ::rand() % 256;
	}
	sz_cEnd[cLast - 1] = cLast;
	CStdioFile f;
	if (!f.Open(strActivationFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return false;
	f.Write(sz_cStart, cFirst);
	f.Write(sz_cCiphertext, nCiphertextLen);
	f.Write(sz_cEnd, cLast);
	return true;
}