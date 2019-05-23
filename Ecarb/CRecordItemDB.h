#pragma once
#include "CRecordItem.h"
#include "CRecordDataDB.h"
class CRecordItemDB
{
private:
	CString m_strPathDB;
	void CreateTable();
	CRecordDataDB m_rddatadb;
public:
	CRecordItemDB();
	virtual ~CRecordItemDB();
	bool Insert(const CRecordItem & rditem);
	bool InsertFull(CRecordItem & rditem);
	bool Update(const CRecordItem & rditem);
	bool UpdateFull(CRecordItem & rditem);
	bool Delete(const CRecordItem & rditem);
	bool DeleteFull(CRecordItem & rditem);
	bool Select(const CString & strSql, std::vector<CRecordItem> & vec_rditem);//strSql须为：SELECT *选择全部列才能返回对象
	bool SelectFull(const CString & strSql, std::vector<CRecordItem> & vec_rditem);//strSql须为：SELECT *选择全部列才能返回对象
	static int GetNextInsertID();
};

