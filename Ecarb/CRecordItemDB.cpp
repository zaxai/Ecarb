#include "stdafx.h"
#include "CRecordItemDB.h"


CRecordItemDB::CRecordItemDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Record.mdb");
	CreateTable();
}


CRecordItemDB::~CRecordItemDB()
{
}


void CRecordItemDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE RecordItem (\
				ID INT PRIMARY KEY NOT NULL,\
				DataItemID INT,\
				DataItemType INT,\
				DataItemName NVARCHAR(100),\
				Unit NVARCHAR(100),\
				RegStartAddr NVARCHAR(100),\
				RegNum INT,\
				DecimalDigits INT,\
				Multiplier DOUBLE,\
				Constant DOUBLE,\
				MaxData DOUBLE,\
				MinData DOUBLE,\
				RecordID INT\
				)");
	zsql.ExecSQL(strSql);
}


bool CRecordItemDB::Insert(const CRecordItem & rditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO RecordItem VALUES (%d,%d,%d,'%s','%s','%s',%d,%d,%f,%f,%f,%f,%d)"), rditem.GetID(), rditem.GetDataItemID(),rditem.GetDataItemType(), rditem.GetDataItemName(), rditem.GetUnit(), rditem.GetRegStartAddr(), rditem.GetRegNum(), rditem.GetDecimalDigits(), rditem.GetMultiplier(), rditem.GetConstant(), rditem.GetMaxData(), rditem.GetMinData(), rditem.GetRecordID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordItemDB::InsertFull(CRecordItem & rditem)
{
	for (const CRecordData & rddata : rditem.GetRecordData())
		m_rddatadb.Insert(rddata);
	return Insert(rditem);
}


bool CRecordItemDB::Update(const CRecordItem & rditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE RecordItem SET DataItemID=%d,DataItemType=%d,DataItemName='%s',Unit='%s',RegStartAddr='%s',RegNum=%d,DecimalDigits=%d,Multiplier=%f,Constant=%f,MaxData=%f,MinData=%f,RecordID=%d WHERE ID=%d"), rditem.GetDataItemID(),rditem.GetDataItemType(), rditem.GetDataItemName(), rditem.GetUnit(), rditem.GetRegStartAddr(), rditem.GetRegNum(), rditem.GetDecimalDigits(), rditem.GetMultiplier(), rditem.GetConstant(), rditem.GetMaxData(), rditem.GetMinData(), rditem.GetRecordID(), rditem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordItemDB::UpdateFull(CRecordItem & rditem)
{
	for (const CRecordData & rddata : rditem.GetRecordData())
		m_rddatadb.Update(rddata);
	return Update(rditem);
}


bool CRecordItemDB::Delete(const CRecordItem & rditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM RecordItem WHERE ID=%d"), rditem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordItemDB::DeleteFull(CRecordItem & rditem)
{
	m_rddatadb.Delete(rditem.GetRecordData());
	return Delete(rditem);
}


bool CRecordItemDB::Select(const CString & strSql, std::vector<CRecordItem> & vec_rditem)
{
	vec_rditem.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 13)
		{
			for (int i = 1; i < nRow; ++i)
			{
				std::vector<CRecordData> vec_rddata;
				CString strSunSql;
				strSunSql.Format(_T("SELECT * FROM RecordData WHERE RecordItemID=%d ORDER BY ID"), _ttoi(vec2_strData[i][0]));
				m_rddatadb.Select(strSunSql, vec_rddata);
				CRecordItem rditem(_ttoi(vec2_strData[i][0]), _ttoi(vec2_strData[i][1]), _ttoi(vec2_strData[i][2]), vec2_strData[i][3], vec2_strData[i][4], vec2_strData[i][5], _ttoi(vec2_strData[i][6]), _ttoi(vec2_strData[i][7]), _ttof(vec2_strData[i][8]), _ttof(vec2_strData[i][9]), _ttof(vec2_strData[i][10]), _ttof(vec2_strData[i][11]), _ttoi(vec2_strData[i][12]), vec_rddata);
				vec_rditem.push_back(rditem);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CRecordItemDB::SelectFull(const CString & strSql, std::vector<CRecordItem> & vec_rditem)
{
	return Select(strSql, vec_rditem);
}


int CRecordItemDB::GetNextInsertID()
{
	int nID = 0;
	std::vector<CRecordItem> vec_rditem;
	CRecordItemDB rditemdb;
	rditemdb.Select(_T("SELECT * FROM RecordItem ORDER BY ID DESC LIMIT 1"), vec_rditem);
	if (vec_rditem.size() == 1)
		nID = vec_rditem[0].GetID() + 1;
	return nID;
}
