#include "stdafx.h"
#include "ZModbus.h"


ZModbus::ZModbus()
	: m_strDeviceAddr(_T(""))
	, m_strFuncCode(_T(""))
	, m_strAutoEnable(_T(""))
	, m_strRegStartAddr(_T(""))
	, m_strRegNum(_T(""))
	, m_strTgtData(_T(""))
	, m_strCheckCode(_T(""))
{
}


ZModbus::~ZModbus()
{
}


bool ZModbus::CheckProtocol(const CString & str)
{
	BYTE sz_cData[512] = { 0 };
	if (CStringToBYTEArrBy2(str, sz_cData, sizeof(sz_cData)) == false)
		return false;
	int nByteLen = str.GetLength() / 2;
	if (nByteLen < 6)
		return false;
	CString strFuncCode;
	strFuncCode = str.Mid(2, 2);
	if (strFuncCode == _T("03") || strFuncCode == _T("04"))
	{
		if (nByteLen != 8)
		{
			int nRegNum = _tcstol(str.Mid(4, 2), NULL, 16) / 2;
			if (nByteLen != nRegNum * 2 + 5)
				return false;
		}
	}
	else if (strFuncCode == _T("06"))
	{
		if (nByteLen != 8)
			return false;
	}
	else if (strFuncCode == _T("41"))
	{
		int nRegNum = _tcstol(str.Mid(6, 2), NULL, 16);
		if (nByteLen != nRegNum * 2 + 6)
			return false;
	}
	else
		return false;
	UINT16 nCRC16ByData, nCRC16ByCalc;
	nCRC16ByData = sz_cData[nByteLen - 2] + sz_cData[nByteLen - 1] * 256;
	nCRC16ByCalc = GetCRC16(sz_cData, nByteLen - 2);
	return nCRC16ByData == nCRC16ByCalc;
}


bool ZModbus::CStringToBYTEArrBy2(const CString & str, BYTE * p_c, int nMaxByteLen)
{
	CString strByte;
	int i = 0, j = 0, nByteLen;
	nByteLen = str.GetLength() / 2;
	if (nByteLen >= nMaxByteLen)
		return false;
	for (; i < nByteLen; ++i, j = j + 2)
	{
		strByte = str.Mid(j, 2);
		p_c[i] = BYTE(_tcstol(strByte, NULL, 16));
	}
	p_c[nByteLen] = '\0';
	return true;
}


CString ZModbus::ReverseCStringBy2(CString str)
{
	CString strRtn;
	str.MakeReverse();
	int nLen = str.GetLength();
	if (nLen < 2)
		return strRtn;
	if (nLen % 2)
		--nLen;
	for (int i = 0; i < nLen; i = i + 2)
	{
		strRtn += str[i + 1];
		strRtn += str[i];
	}
	return strRtn;
}


UINT16 ZModbus::GetCRC16(BYTE * p_c, unsigned int nByteLen)
{
	unsigned int i;
	UINT16 nCRC16 = 0xFFFF;
	while (nByteLen > 0)
	{
		nCRC16 ^= *p_c++;
		for (i = 8; i > 0; --i)
		{
			if (nCRC16 & 1)
			{
				nCRC16 >>= 1;
				nCRC16 ^= 0xA001;
			}
			else
			{
				nCRC16 >>= 1;
			}
		}
		--nByteLen;
	}
	return	nCRC16;
}


CString ZModbus::GetCRC16(const CString & str)
{
	CString strCRC16;
	BYTE sz_cData[512] = { 0 };
	if (CStringToBYTEArrBy2(str, sz_cData, sizeof(sz_cData)))
		strCRC16.Format(_T("%04X"), GetCRC16(sz_cData, str.GetLength() / 2));
	return strCRC16;
}


int ZModbus::DataEncode(CString & strData)
{
	strData.Empty();
	m_strDeviceAddr.MakeUpper();
	m_strFuncCode.MakeUpper();
	m_strAutoEnable.MakeUpper();
	m_strRegStartAddr.MakeUpper();
	m_strRegNum.MakeUpper();
	m_strTgtData.MakeUpper();
	if (m_strFuncCode == _T("03") || m_strFuncCode == _T("04"))
	{
		strData = m_strDeviceAddr + m_strFuncCode + m_strRegStartAddr + m_strRegNum;
	}
	else if (m_strFuncCode == _T("06"))
	{
		strData = m_strDeviceAddr + m_strFuncCode + m_strRegStartAddr + m_strTgtData;
	}
	else if (m_strFuncCode == _T("41"))
	{
		strData = m_strDeviceAddr + m_strFuncCode + m_strAutoEnable + m_strRegNum + m_strRegStartAddr;
	}
	else
		return ERROR_FUNCCODE;
	m_strCheckCode = ReverseCStringBy2(GetCRC16(strData));
	strData += m_strCheckCode;
	return ERROR_OK;
}


int ZModbus::DataDecode(CString strData, std::vector<CString> & vec_strFixedData, std::vector<CString> & vec_strValue)
{
	vec_strFixedData.clear();
	vec_strValue.clear();
	if (!CheckProtocol(strData))
		return ERROR_PROTOCOL;
	CString strDeviceAddr,strFuncCode, strAutoEnable, strRegStartAdd, strRegNum, strValue;
	int nByteLen = strData.GetLength() / 2;
	strDeviceAddr = strData.Mid(0, 2);
	strFuncCode = strData.Mid(2, 2);
	if (strFuncCode == _T("03") || strFuncCode == _T("04"))
	{
		switch (nByteLen)
		{
		case 8:
		{
			strRegStartAdd= strData.Mid(4, 4);
			strRegNum.Format(_T("%d"), _tcstol(strData.Mid(8, 4), NULL, 16));
		}
		break;
		default:
		{
			int nRegNum = _tcstol(strData.Mid(4, 2), NULL, 16) / 2;
			strRegNum.Format(_T("%d"), nRegNum);
			for (int i = 0; i < nRegNum; ++i)
			{
				strValue.Format(_T("%d"), _tcstol(strData.Mid(6 + i*4, 4), NULL, 16));
				vec_strValue.push_back(strValue);
			}
		}
		break;
		}
	}
	else if (strFuncCode == _T("06"))
	{
		strRegStartAdd = strData.Mid(4, 4);
		strValue.Format(_T("%d"), _tcstol(strData.Mid(8, 4), NULL, 16));
		vec_strValue.push_back(strValue);
	}
	else if (strFuncCode == _T("41"))
	{
		strAutoEnable = strData.Mid(4, 2);
		int nRegNum = _tcstol(strData.Mid(6, 2), NULL, 16);
		strRegNum.Format(_T("%d"), nRegNum);
		for (int i = 0; i < nRegNum; ++i)
		{
			strValue.Format(_T("%d"), _tcstol(strData.Mid(8 + i * 4, 4), NULL, 16));
			vec_strValue.push_back(strValue);
		}
	}
	else if (strFuncCode == _T("83") || strFuncCode == _T("84") || strFuncCode == _T("86") || strFuncCode == _T("C1"))
	{
		strValue.Format(_T("%02d"), _tcstol(strData.Mid(4, 2), NULL, 16));
		vec_strValue.push_back(strValue);
	}
	else
		return ERROR_FUNCCODE;
	vec_strFixedData.push_back(strDeviceAddr);
	vec_strFixedData.push_back(strFuncCode);
	vec_strFixedData.push_back(strAutoEnable);
	vec_strFixedData.push_back(strRegStartAdd);
	vec_strFixedData.push_back(strRegNum);
	return ERROR_OK;
}


void ZModbus::Init(const CString & strDeviceAddr, const CString & strFuncCode, const CString & strAutoEnable, const CString & strRegStartAddr, const CString & strRegNum, const CString & strTgtData)
{
	m_strDeviceAddr = strDeviceAddr;
	m_strFuncCode = strFuncCode;
	m_strAutoEnable = strAutoEnable;
	m_strRegStartAddr = strRegStartAddr;
	m_strRegNum = strRegNum;
	m_strTgtData = strTgtData;
}