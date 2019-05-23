#include "stdafx.h"
#include "CWndItemDB.h"


CWndItemDB::CWndItemDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Data.mdb");
	CreateTable();
}


CWndItemDB::~CWndItemDB()
{
}


void CWndItemDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE WndItem (\
				ID INT PRIMARY KEY NOT NULL,\
				DataItemID INT,\
				WndInfoID INT\
				)");
	zsql.ExecSQL(strSql);
}


bool CWndItemDB::Insert(const CWndItem & wnditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO WndItem VALUES (%d,%d,%d)"), wnditem.GetID(), wnditem.GetDataItemID(), wnditem.GetWndInfoID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndItemDB::Update(const CWndItem & wnditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE WndItem SET DataItemID=%d,WndInfoID=%d WHERE ID=%d"), wnditem.GetDataItemID(), wnditem.GetWndInfoID(), wnditem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndItemDB::Delete(const CWndItem & wnditem)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM WndItem WHERE ID=%d"), wnditem.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndItemDB::Select(const CString & strSql, std::list<CWndItem> & list_wnditem)
{
	list_wnditem.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 3)
		{
			for (int i = 1; i < nRow; ++i)
			{
				CWndItem wnditem(_ttoi(vec2_strData[i][0]), _ttoi(vec2_strData[i][1]), _ttoi(vec2_strData[i][2]));
				list_wnditem.push_back(wnditem);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


int CWndItemDB::GetNextInsertID()
{
	int nID = 0;
	std::list<CWndItem> list_wnditem;
	CWndItemDB wnditemdb;
	wnditemdb.Select(_T("SELECT * FROM WndItem ORDER BY ID DESC LIMIT 1"), list_wnditem);
	if (list_wnditem.size() == 1)
		nID = list_wnditem.front().GetID() + 1;
	return nID;
}