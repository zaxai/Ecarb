#include "stdafx.h"
#include "CAutoReportCheck.h"


CAutoReportCheck::CAutoReportCheck()
	: m_nLength(0)
{
}


CAutoReportCheck::~CAutoReportCheck()
{
}


void CAutoReportCheck::SetLength(int nLength)
{
	m_nLength = nLength;
}


bool CAutoReportCheck::CheckProtocol(const CString & str)
{
	if (str.GetLength() >= m_nLength)
		return true;
	return false;
}