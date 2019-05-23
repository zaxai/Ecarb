#pragma once
#include "CRecord.h"
#include "CRecordItemDB.h"
class CRecordDB
{
private:
	CString m_strPathDB;
	void CreateTable();
	CRecordItemDB m_rditemdb;
public:
	CRecordDB();
	virtual ~CRecordDB();
	bool Insert(const CRecord & record);
	bool InsertFull(CRecord & record);
	bool Update(const CRecord & record);
	bool UpdateFull(CRecord & record);
	bool Delete(const CRecord & record);
	bool DeleteFull(CRecord & record);
	bool Select(const CString & strSql, std::vector<CRecord> & vec_record);//strSql须为：SELECT *选择全部列才能返回对象
	bool SelectFull(const CString & strSql, std::vector<CRecord> & vec_record);//strSql须为：SELECT *选择全部列才能返回对象
	static int GetNextInsertID();
};

