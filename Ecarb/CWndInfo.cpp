#include "stdafx.h"
#include "CWndInfo.h"


CWndInfo::CWndInfo()
	: m_nID(0)
	, m_strName(_T(""))
	, m_nWndType(TYPE_CALIBRATE)
	, m_nLeft(0)
	, m_nTop(0)
	, m_nRight(0)
	, m_nBottom(0)
	, m_nPageID(0)
{
}


CWndInfo::CWndInfo(int nID, CString strName, int nWndType, int nLeft, int nTop, int nRight, int nBottom, int nPageID, const std::list<CWndItem> & list_wnditem)
	: m_nID(nID)
	, m_strName(strName)
	, m_nWndType(nWndType)
	, m_nLeft(nLeft)
	, m_nTop(nTop)
	, m_nRight(nRight)
	, m_nBottom(nBottom)
	, m_nPageID(nPageID)
	, m_list_wnditem(list_wnditem)
{
}


CWndInfo::~CWndInfo()
{
}


bool CWndInfo::operator==(const CWndInfo & wndinfo)
{
	return m_nID == wndinfo.GetID();
}


int CWndInfo::GetID() const
{
	return m_nID;
}


CString CWndInfo::GetName() const
{
	return m_strName;
}


int CWndInfo::GetWndType() const
{
	return m_nWndType;
}


int CWndInfo::GetLeft() const
{
	return m_nLeft;
}


int CWndInfo::GetTop() const
{
	return m_nTop;
}


int CWndInfo::GetRight() const
{
	return m_nRight;
}


int CWndInfo::GetBottom() const
{
	return m_nBottom;
}


CRect CWndInfo::GetRect() const
{
	return CRect(m_nLeft, m_nTop, m_nRight, m_nBottom);
}


int CWndInfo::GetPageID() const
{
	return m_nPageID;
}


std::list<CWndItem> & CWndInfo::GetWndItem()
{
	return m_list_wnditem;
}


void CWndInfo::SetLeft(int nLeft)
{
	m_nLeft = nLeft;
}


void CWndInfo::SetTop(int nTop)
{
	m_nTop = nTop;
}


void CWndInfo::SetRight(int nRight)
{
	m_nRight = nRight;
}


void CWndInfo::SetBottom(int nBottom)
{
	m_nBottom = nBottom;
}


void CWndInfo::SetRect(const CRect & rc)
{
	m_nLeft = rc.left;
	m_nTop = rc.top;
	m_nRight = rc.right;
	m_nBottom = rc.bottom;
}