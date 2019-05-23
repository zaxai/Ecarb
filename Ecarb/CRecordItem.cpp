#include "stdafx.h"
#include "CRecordItem.h"


CRecordItem::CRecordItem()
	: m_nID(0)
	, m_nDataItemID(0)
	, m_nDataItemType(0)
	, m_strDataItemName(_T(""))
	, m_strUnit(_T(""))
	, m_strRegStartAddr(_T(""))
	, m_nRegNum(0)
	, m_nDecimalDigits(0)
	, m_dMultiplier(0.0)
	, m_dConstant(0.0)
	, m_dMaxData(0.0)
	, m_dMinData(0.0)
	, m_nRecordID(0)
{
}


CRecordItem::CRecordItem(int nID, int nDataItemID, int nDataItemType, const CString & strDataItemName, const CString & strUnit, const CString & strRegStartAddr, int nRegNum, int nDecimalDigits, double dMultiplier, double dConstant, double dMaxData, double dMinData, int nRecordID, const std::vector<CRecordData> & vec_rddata)
	: m_nID(nID)
	, m_nDataItemID(nDataItemID)
	, m_nDataItemType(nDataItemType)
	, m_strDataItemName(strDataItemName)
	, m_strUnit(strUnit)
	, m_strRegStartAddr(strRegStartAddr)
	, m_nRegNum(nRegNum)
	, m_nDecimalDigits(nDecimalDigits)
	, m_dMultiplier(dMultiplier)
	, m_dConstant(dConstant)
	, m_dMaxData(dMaxData)
	, m_dMinData(dMinData)
	, m_nRecordID(nRecordID)
	, m_vec_rddata(vec_rddata)
{

}


CRecordItem::~CRecordItem()
{
}


int CRecordItem::GetID() const
{
	return m_nID;
}


int CRecordItem::GetDataItemID() const
{
	return m_nDataItemID;
}


int CRecordItem::GetDataItemType() const
{
	return m_nDataItemType;
}


CString CRecordItem::GetDataItemName() const
{
	return m_strDataItemName;
}


CString CRecordItem::GetUnit() const
{
	return m_strUnit;
}


CString CRecordItem::GetRegStartAddr() const
{
	return m_strRegStartAddr;
}


int CRecordItem::GetRegNum() const
{
	return m_nRegNum;
}


int CRecordItem::GetDecimalDigits() const
{
	return m_nDecimalDigits;
}


double CRecordItem::GetMultiplier() const
{
	return m_dMultiplier;
}


double CRecordItem::GetConstant() const
{
	return m_dConstant;
}


double CRecordItem::GetMaxData() const
{
	return m_dMaxData;
}


double CRecordItem::GetMinData() const
{
	return m_dMinData;
}


int CRecordItem::GetRecordID() const
{
	return m_nRecordID;
}


std::vector<CRecordData> & CRecordItem::GetRecordData()
{
	return m_vec_rddata;
}
