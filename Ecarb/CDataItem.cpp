#include "stdafx.h"
#include "CDataItem.h"


CDataItem::CDataItem()
	: m_nID(0)
	, m_nItemType(TYPE_CALIBRATE)
	, m_strName(_T(""))
	, m_strUnit(_T(""))
	, m_strRegStartAddr(_T(""))
	, m_nRegNum(0)
	, m_nDecimalDigits(0)
	, m_dMultiplier(0.0)
	, m_dConstant(0.0)
	, m_dTgtData(0.0)
	, m_dMaxData(0.0)
	, m_dMinData(0.0)
{
}


CDataItem::CDataItem(int nID, int nItemType, const CString & strName, const CString & strUnit, const CString & strRegStartAddr, int nRegNum, int nDecimalDigits,double dMultiplier, double dConstant, double dTgtData, double dMaxData, double dMinData)
	: m_nID(nID)
	, m_nItemType(nItemType)
	, m_strName(strName)
	, m_strUnit(strUnit)
	, m_strRegStartAddr(strRegStartAddr)
	, m_nRegNum(nRegNum)
	, m_nDecimalDigits(nDecimalDigits)
	, m_dMultiplier(dMultiplier)
	, m_dConstant(dConstant)
	, m_dTgtData(dTgtData)
	, m_dMaxData(dMaxData)
	, m_dMinData(dMinData)
{

}


CDataItem::~CDataItem()
{
}


bool CDataItem::operator==(const CDataItem & dataitem)
{
	return m_nID == dataitem.GetID();
}


int CDataItem::GetID() const
{
	return m_nID;
}


int CDataItem::GetItemType() const
{
	return m_nItemType;
}


CString CDataItem::GetName() const
{
	return m_strName;
}


CString CDataItem::GetUnit() const
{
	return m_strUnit;
}


CString CDataItem::GetRegStartAddr() const
{
	return m_strRegStartAddr;
}


int CDataItem::GetRegNum() const
{
	return m_nRegNum;
}


int CDataItem::GetDecimalDigits() const
{
	return m_nDecimalDigits;
}


double CDataItem::GetMultiplier() const
{
	return m_dMultiplier;
}


double CDataItem::GetConstant() const
{
	return m_dConstant;
}


double CDataItem::GetTgtData() const
{
	return m_dTgtData;
}


double CDataItem::GetMaxData() const
{
	return m_dMaxData;
}


double CDataItem::GetMinData() const
{
	return m_dMinData;
}


void  CDataItem::SetID(int nID)
{
	m_nID = nID;
}


void  CDataItem::SetItemType(int nItemType)
{
	m_nItemType = nItemType;
}


void  CDataItem::SetName(const CString & strName)
{
	m_strName = strName;
}


void  CDataItem::SetUnit(const CString & strUnit)
{
	m_strUnit = strUnit;
}


void  CDataItem::SetRegStartAddr(const CString & strRegStartAddr)
{
	m_strRegStartAddr = strRegStartAddr;
}


void  CDataItem::SetRegNum(int nRegNum)
{
	m_nRegNum = nRegNum;
}


void  CDataItem::SetDecimalDigits(int nDecimalDigits)
{
	m_nDecimalDigits = nDecimalDigits;
}


void CDataItem::SetMultiplier(double dMultiplier)
{
	m_dMultiplier = dMultiplier;
}


void CDataItem::SetConstant(double dConstant)
{
	m_dConstant = dConstant;
}


void  CDataItem::SetTgtData(double dTgtData)
{
	m_dTgtData = dTgtData;
}


void  CDataItem::SetMaxData(double dMaxData)
{
	m_dMaxData = dMaxData;
}


void  CDataItem::SetMinData(double dMinData)
{
	m_dMinData = dMinData;
}