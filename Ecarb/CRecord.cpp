#include "stdafx.h"
#include "CRecord.h"


CRecord::CRecord()
	: m_nID(0)
	, m_strWndName(_T(""))
	, m_nWndType(0)
	, m_strTimeStart(_T(""))
{
}


CRecord::CRecord(int nID, const CString & strWndName, int nWndType, const CString & strTimeStart, const std::vector<CRecordItem> & vec_rditem)
	: m_nID(nID)
	, m_strWndName(strWndName)
	, m_nWndType(nWndType)
	, m_strTimeStart(strTimeStart)
	, m_vec_rditem(vec_rditem)
{

}


CRecord::~CRecord()
{
}


int CRecord::GetID() const
{
	return m_nID;
}


CString CRecord::GetWndName() const
{
	return m_strWndName;
}


int CRecord::GetWndType() const
{
	return m_nWndType;
}


CString CRecord::GetTimeStart() const
{
	return m_strTimeStart;
}


std::vector<CRecordItem> & CRecord::GetRecordItem()
{
	return m_vec_rditem;
}