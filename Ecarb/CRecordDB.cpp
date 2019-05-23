#include "stdafx.h"
#include "CRecordDB.h"


CRecordDB::CRecordDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Record.mdb");
	CreateTable();
}


CRecordDB::~CRecordDB()
{
}


void CRecordDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE Record (\
				ID INT PRIMARY KEY NOT NULL,\
				WndName NVARCHAR(100),\
				WndType INT,\
				TimeStart DATETIME\
				)");
	zsql.ExecSQL(strSql);
}


bool CRecordDB::Insert(const CRecord & record)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO Record VALUES (%d,'%s',%d,datetime('now','localtime'))"), record.GetID(), record.GetWndName(), record.GetWndType());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDB::InsertFull(CRecord & record)
{
	for (CRecordItem & rditem : record.GetRecordItem())
		m_rditemdb.InsertFull(rditem);
	return Insert(record);
}


bool CRecordDB::Update(const CRecord & record)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE Record SET WndName='%s',WndType=%d,TimeStart='%s' WHERE ID=%d"), record.GetWndName(), record.GetWndType(), record.GetTimeStart(),record.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDB::UpdateFull(CRecord & record)
{
	for (CRecordItem & rditem : record.GetRecordItem())
		m_rditemdb.UpdateFull(rditem);
	return Update(record);
}


bool CRecordDB::Delete(const CRecord & record)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM Record WHERE ID=%d"), record.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDB::DeleteFull(CRecord & record)
{
	for (CRecordItem & rditem : record.GetRecordItem())
		m_rditemdb.DeleteFull(rditem);
	return Delete(record);
}


bool CRecordDB::Select(const CString & strSql, std::vector<CRecord> & vec_record)
{
	vec_record.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 4)
		{
			for (int i = 1; i < nRow; ++i)
			{
				std::vector<CRecordItem> vec_rditem;
				CString strSunSql;
				strSunSql.Format(_T("SELECT * FROM RecordItem WHERE RecordID=%d ORDER BY ID"), _ttoi(vec2_strData[i][0]));
				m_rditemdb.SelectFull(strSunSql, vec_rditem);
				CRecord record(_ttoi(vec2_strData[i][0]), vec2_strData[i][1], _ttoi(vec2_strData[i][2]), vec2_strData[i][3], vec_rditem);
				vec_record.push_back(record);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDB::SelectFull(const CString & strSql, std::vector<CRecord> & vec_record)
{
	return Select(strSql, vec_record);
}


int CRecordDB::GetNextInsertID()
{
	int nID = 0;
	std::vector<CRecord> vec_record;
	CRecordDB recorddb;
	recorddb.Select(_T("SELECT * FROM Record ORDER BY ID DESC LIMIT 1"), vec_record);
	if (vec_record.size() == 1)
		nID = vec_record[0].GetID() + 1;
	return nID;
}