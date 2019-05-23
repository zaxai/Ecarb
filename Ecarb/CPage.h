#pragma once
#include "CWndInfo.h"
#include <list>
class CPage
{
private:
	int m_nID;
	CString m_strName;
	std::list<CWndInfo> m_list_wndinfo;
public:
	CPage();
	CPage(int nID, CString strName, std::list<CWndInfo> & list_wndinfo);
	virtual ~CPage();
	int GetID() const;
	CString GetName() const;
	std::list<CWndInfo> & GetWndInfo();
};

