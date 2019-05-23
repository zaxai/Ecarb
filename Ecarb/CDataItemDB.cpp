#include "stdafx.h"
#include "CDataItemDB.h"


CDataItemDB::CDataItemDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Data.mdb");
	CreateTable();
}


CDataItemDB::~CDataItemDB()
{
}


void CDataItemDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE DataItem (\
				ID INT PRIMARY KEY NOT NULL,\
				ItemType INT,\
				Name NVARCHAR(100),\
				Unit NVARCHAR(100),\
				RegStartAddr NVARCHAR(100),\
				RegNum INT,\
				DecimalDigits INT,\
				Multiplier DOUBLE,\
				Constant DOUBLE,\
				TgtData DOUBLE,\
				MaxData DOUBLE,\
				MinData DOUBLE\
				)");
	zsql.ExecSQL(strSql);
}


bool CDataItemDB::Insert(const CDataItem & dataitem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO DataItem VALUES (%d,%d,'%s','%s','%s',%d,%d,%f,%f,%f,%f,%f)"), dataitem.GetID(), dataitem.GetItemType(), dataitem.GetName(), dataitem.GetUnit(), dataitem.GetRegStartAddr(), dataitem.GetRegNum(), dataitem.GetDecimalDigits(), dataitem.GetMultiplier(), dataitem.GetConstant(),dataitem.GetTgtData(), dataitem.GetMaxData(), dataitem.GetMinData());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CDataItemDB::Update(const CDataItem & dataitem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE DataItem SET ItemType=%d,Name='%s',Unit='%s',RegStartAddr='%s',RegNum=%d,DecimalDigits=%d,Multiplier=%f,Constant=%f,TgtData=%f,MaxData=%f,MinData=%f WHERE ID=%d"), dataitem.GetItemType(), dataitem.GetName(), dataitem.GetUnit(), dataitem.GetRegStartAddr(), dataitem.GetRegNum(), dataitem.GetDecimalDigits(), dataitem.GetMultiplier(), dataitem.GetConstant(),dataitem.GetTgtData(), dataitem.GetMaxData(), dataitem.GetMinData(), dataitem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CDataItemDB::Delete(const CDataItem & dataitem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM DataItem WHERE ID=%d"), dataitem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CDataItemDB::Select(const CString & strSql, std::vector<CDataItem> & vec_dataitem)
{
	vec_dataitem.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 12)
		{
			for (int i = 1; i < nRow; ++i)
			{
				CDataItem dataitem(_ttoi(vec2_strData[i][0]), _ttoi(vec2_strData[i][1]),vec2_strData[i][2], vec2_strData[i][3], vec2_strData[i][4], _ttoi(vec2_strData[i][5]), _ttoi(vec2_strData[i][6]), _ttof(vec2_strData[i][7]), _ttof(vec2_strData[i][8]), _ttof(vec2_strData[i][9]), _ttof(vec2_strData[i][10]), _ttof(vec2_strData[i][11]));
				vec_dataitem.push_back(dataitem);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CDataItemDB::Select(const CString & strSql, std::list<CDataItem> & list_dataitem)
{
	list_dataitem.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 12)
		{
			for (int i = 1; i < nRow; ++i)
			{
				CDataItem dataitem(_ttoi(vec2_strData[i][0]), _ttoi(vec2_strData[i][1]), vec2_strData[i][2], vec2_strData[i][3], vec2_strData[i][4], _ttoi(vec2_strData[i][5]), _ttoi(vec2_strData[i][6]), _ttof(vec2_strData[i][7]), _ttof(vec2_strData[i][8]), _ttof(vec2_strData[i][9]), _ttof(vec2_strData[i][10]), _ttof(vec2_strData[i][11]));
				list_dataitem.push_back(dataitem);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


int CDataItemDB::GetNextInsertID()
{
	int nID = 0;
	std::vector<CDataItem> vec_dataitem;
	CDataItemDB dataitemdb;
	dataitemdb.Select(_T("SELECT * FROM DataItem ORDER BY ID DESC LIMIT 1"), vec_dataitem);
	if (vec_dataitem.size() == 1)
		nID = vec_dataitem[0].GetID() + 1;
	return nID;
}


bool CDataItemDB::GetDataItem(int nDataItemID, CDataItem & dataitem)
{
	std::vector<CDataItem> vec_dataitem;
	CDataItemDB dataitemdb;
	CString strSql;
	strSql.Format(_T("SELECT * FROM DataItem WHERE ID=%d"),nDataItemID);
	dataitemdb.Select(strSql, vec_dataitem);
	if (vec_dataitem.size() == 1)
	{
		dataitem = vec_dataitem[0];
		return true;
	}
	else
		return false;
}