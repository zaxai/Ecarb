#include "stdafx.h"
#include "CWndItem.h"


CWndItem::CWndItem()
	: m_nID(0)
	, m_nDataItemID(0)
	, m_nWndInfoID(0)
{
}


CWndItem::CWndItem(int nID, int nDataItemID, int nWndInfoID)
	: m_nID(nID)
	, m_nDataItemID(nDataItemID)
	, m_nWndInfoID(nWndInfoID)
{
}


CWndItem::~CWndItem()
{
}


bool CWndItem::operator==(const CWndItem & wnditem)
{
	return m_nID == wnditem.GetID();
}


int CWndItem::GetID() const
{
	return m_nID;
}


int CWndItem::GetDataItemID() const
{
	return m_nDataItemID;
}


int CWndItem::GetWndInfoID() const
{
	return m_nWndInfoID;
}