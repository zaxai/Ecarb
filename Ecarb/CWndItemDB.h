#pragma once
#include "CWndItem.h"
#include "ZSqlite3.h"
#include <list>
class CWndItemDB
{
private:
	CString m_strPathDB;
	void CreateTable();
public:
	CWndItemDB();
	virtual ~CWndItemDB();
	bool Insert(const CWndItem & wnditem);
	bool Update(const CWndItem & wnditem);
	bool Delete(const CWndItem & wnditem);
	bool Select(const CString & strSql, std::list<CWndItem> & list_wnditem);//strSql须为：SELECT *选择全部列才能返回对象
	static int GetNextInsertID();
};

