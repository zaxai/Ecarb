#pragma once
#include "ZUtil.h"

class CActivation
{
private:
	static bool GetCiphertextBySN(CString strSerialNumber, char * p_cCiphertext, int nBuffer, int & nCiphertextLen);
	static bool GetCiphertextByFile(CString strActivationFile, char * p_cCiphertext, int nBuffer, int & nCiphertextLen);
public:
	CActivation();
	virtual ~CActivation();
	static CString GetSerialNumber();
	static bool Verify(const CString & strSerialNumber,const CString & strActivationFile);
	static bool CreateActivationFile(const CString & strSerialNumber, const CString & strActivationFile);
};

