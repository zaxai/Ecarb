#pragma once
#include "CWndItem.h"
#include <list>
class CWndInfo
{
public:
	enum WNDTYPE
	{
		TYPE_CALIBRATE=0,
		TYPE_MONITOR,
		TYPE_DASHBOARD,
		TYPE_WAVE,
		TYPE_STATUSLED
	};
private:
	int m_nID;
	CString m_strName;
	int m_nWndType;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
	int m_nPageID;
	std::list<CWndItem> m_list_wnditem;
public:
	CWndInfo();
	CWndInfo(int nID, CString strName, int nWndType, int nLeft, int nTop, int nRight, int nBottom, int nPageID, const std::list<CWndItem> & list_wnditem);
	virtual ~CWndInfo();
	bool operator==(const CWndInfo & wndinfo);
	int GetID() const;
	CString GetName() const;
	int GetWndType() const;
	int GetLeft() const;
	int GetTop() const;
	int GetRight() const;
	int GetBottom() const;
	CRect GetRect() const;
	int GetPageID() const;
	std::list<CWndItem> & GetWndItem();
	void SetLeft(int nLeft);
	void SetTop(int nTop);
	void SetRight(int nRight);
	void SetBottom(int nBottom);
	void SetRect(const CRect & rc);
};

