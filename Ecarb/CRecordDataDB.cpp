#include "stdafx.h"
#include "CRecordDataDB.h"


CRecordDataDB::CRecordDataDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Record.mdb");
	CreateTable();
}


CRecordDataDB::~CRecordDataDB()
{
}


void CRecordDataDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE RecordData (\
				ID INT PRIMARY KEY NOT NULL,\
				TgtData DOUBLE,\
				TgtTime INT,\
				RecordItemID INT\
				)");
	zsql.ExecSQL(strSql);
}


bool CRecordDataDB::Insert(const CRecordData & rddata)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO RecordData VALUES (%d,%f,%d,%d)"), rddata.GetID(), rddata.GetTgtData(), rddata.GetTgtTime(), rddata.GetRecordItemID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDataDB::Insert(const std::vector<CRecordData> & vec_rddata)
{
	if (vec_rddata.empty())
		return true;
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("BEGIN"));
	zsql.ExecSQL(strSql, &strError);
	for (const CRecordData & rddata : vec_rddata)
	{
		strSql.Format(_T("INSERT INTO RecordData VALUES (%d,%f,%d,%d)"), rddata.GetID(), rddata.GetTgtData(), rddata.GetTgtTime(), rddata.GetRecordItemID());
		zsql.ExecSQL(strSql, &strError);
	}
	strSql.Format(_T("COMMIT"));
	zsql.ExecSQL(strSql, &strError);
	std::vector<CRecordData> vec;
	strSql.Format(_T("SELECT * FROM RecordData WHERE ID = %d"), vec_rddata[0].GetID());
	if (Select(strSql, vec))
	{
		if (vec.size() == 1)
			return true;
	}
	return false;
}


bool CRecordDataDB::Update(const CRecordData & rddata)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE RecordData SET TgtData=%f,TgtTime=%d,RecordItemID=%d WHERE ID=%d"), rddata.GetTgtData(), rddata.GetTgtTime(), rddata.GetRecordItemID(), rddata.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDataDB::Delete(const CRecordData & rddata)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM RecordData WHERE ID=%d"), rddata.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordDataDB::Delete(const std::vector<CRecordData> & vec_rddata)
{
	if (vec_rddata.empty())
		return true;
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("BEGIN"));
	zsql.ExecSQL(strSql, &strError);
	for (const CRecordData & rddata : vec_rddata)
	{
		strSql.Format(_T("DELETE FROM RecordData WHERE ID=%d"), rddata.GetID());
		zsql.ExecSQL(strSql, &strError);
	}
	strSql.Format(_T("COMMIT"));
	zsql.ExecSQL(strSql, &strError);
	std::vector<CRecordData> vec;
	strSql.Format(_T("SELECT * FROM RecordData WHERE ID = %d"), vec_rddata[0].GetID());
	if (Select(strSql, vec))
	{
		if (vec.size() == 0)
			return true;
	}
	return false;
}


bool CRecordDataDB::Select(const CString & strSql, std::vector<CRecordData> & vec_rddata)
{
	vec_rddata.clear();
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
				CRecordData rddata(_ttoi(vec2_strData[i][0]), _ttof(vec2_strData[i][1]), _ttoi(vec2_strData[i][2]), _ttoi(vec2_strData[i][3]));
				vec_rddata.push_back(rddata);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


int CRecordDataDB::GetNextInsertID()
{
	int nID = 0;
	std::vector<CRecordData> vec_rddata;
	CRecordDataDB rddatadb;
	rddatadb.Select(_T("SELECT * FROM RecordData ORDER BY ID DESC LIMIT 1"), vec_rddata);
	if (vec_rddata.size() == 1)
		nID = vec_rddata[0].GetID() + 1;
	return nID;
}