#pragma once
#include "CDataItem.h"
#include "ZSqlite3.h"
#include <list>
class CDataItemDB
{
private:
	CString m_strPathDB;
	void CreateTable();
public:
	CDataItemDB();
	virtual ~CDataItemDB();
	bool Insert(const CDataItem & dataitem);
	bool Update(const CDataItem & dataitem);
	bool Delete(const CDataItem & dataitem);
	bool Select(const CString & strSql, std::vector<CDataItem> & vec_dataitem);//strSql须为：SELECT *选择全部列才能返回对象
	bool Select(const CString & strSql, std::list<CDataItem> & list_dataitem);//strSql须为：SELECT *选择全部列才能返回对象
	static int GetNextInsertID();
	static bool GetDataItem(int nDataItemID, CDataItem & dataitem);
};

