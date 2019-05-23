#pragma once
#include "CRecordData.h"
#include "ZSqlite3.h"
#include <vector>
class CRecordDataDB
{
private:
	CString m_strPathDB;
	void CreateTable();
public:
	CRecordDataDB();
	virtual ~CRecordDataDB();
	bool Insert(const CRecordData & rddata);
	bool Insert(const std::vector<CRecordData> & vec_rddata);
	bool Update(const CRecordData & rddata);
	bool Delete(const CRecordData & rddata);
	bool Delete(const std::vector<CRecordData> & vec_rddata);
	bool Select(const CString & strSql, std::vector<CRecordData> & vec_rddata);//strSql须为：SELECT *选择全部列才能返回对象
	static int GetNextInsertID();
};

