#include "stdafx.h"
#include "CRecordData.h"


CRecordData::CRecordData()
	: m_nID(0)
	, m_dTgtData(0.0)
	, m_dwTgtTime(0)
	, m_nRecordItemID(0)
{
}


CRecordData::CRecordData(int nID, double dTgtData, DWORD dwTgtTime, int nRecordItemID)
	: m_nID(nID)
	, m_dTgtData(dTgtData)
	, m_dwTgtTime(dwTgtTime)
	, m_nRecordItemID(nRecordItemID)
{

}


CRecordData::~CRecordData()
{
}


int CRecordData::GetID() const
{
	return m_nID;
}


double CRecordData::GetTgtData() const
{
	return m_dTgtData;
}


DWORD CRecordData::GetTgtTime() const
{
	return m_dwTgtTime;
}


int CRecordData::GetRecordItemID() const
{
	return m_nRecordItemID;
}