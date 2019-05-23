#include "stdafx.h"
#include "CPage.h"


CPage::CPage()
	: m_nID(0)
	, m_strName(_T(""))
{
}


CPage::CPage(int nID, CString strName, std::list<CWndInfo> & list_wndinfo)
	: m_nID(nID)
	, m_strName(strName)
	, m_list_wndinfo(list_wndinfo)
{
}


CPage::~CPage()
{
}


int CPage::GetID() const
{
	return m_nID;
}


CString CPage::GetName() const
{
	return m_strName;
}


std::list<CWndInfo> & CPage::GetWndInfo()
{
	return m_list_wndinfo;
}