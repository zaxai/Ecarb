#pragma once
class CWndItem
{
private:
	int m_nID;
	int m_nDataItemID;
	int m_nWndInfoID;
public:
	CWndItem();
	CWndItem(int nID, int nDataItemID, int nWndInfoID);
	virtual ~CWndItem();
	bool operator==(const CWndItem & wnditem);
	int GetID() const;
	int GetDataItemID() const;
	int GetWndInfoID() const;
};

