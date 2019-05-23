#include "stdafx.h"
#include "CPageDB.h"


CPageDB::CPageDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Data.mdb");
	CreateTable();
}


CPageDB::~CPageDB()
{
}


void CPageDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE Page (\
				ID INT PRIMARY KEY NOT NULL,\
				Name NVARCHAR(100)\
				)");
	zsql.ExecSQL(strSql);
}


bool CPageDB::Insert(const CPage & page)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO Page VALUES (%d,'%s')"), page.GetID(), page.GetName());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CPageDB::InsertFull(CPage & page)
{
	for (CWndInfo & wndinfo : page.GetWndInfo())
		m_wndinfodb.InsertFull(wndinfo);
	return Insert(page);
}


bool CPageDB::Update(const CPage & page)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE Page SET Name='%s' WHERE ID=%d"), page.GetName(), page.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CPageDB::UpdateFull(CPage & page)
{
	for (CWndInfo & wndinfo : page.GetWndInfo())
		m_wndinfodb.UpdateFull(wndinfo);
	return Update(page);
}


bool CPageDB::Delete(const CPage & page)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM Page WHERE ID=%d"), page.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CPageDB::DeleteFull(CPage & page)
{
	for (CWndInfo & wndinfo : page.GetWndInfo())
		m_wndinfodb.DeleteFull(wndinfo);
	return Delete(page);
}


bool CPageDB::Select(const CString & strSql, std::list<CPage> & list_page)
{
	list_page.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 2)
		{
			for (int i = 1; i < nRow; ++i)
			{
				std::list<CWndInfo> list_wndinfo;
				CString strSunSql;
				strSunSql.Format(_T("SELECT * FROM WndInfo WHERE PageID=%d ORDER BY ID"), _ttoi(vec2_strData[i][0]));
				m_wndinfodb.SelectFull(strSunSql, list_wndinfo);
				CPage page(_ttoi(vec2_strData[i][0]), vec2_strData[i][1], list_wndinfo);
				list_page.push_back(page);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CPageDB::SelectFull(const CString & strSql, std::list<CPage> & list_page)
{
	return Select(strSql, list_page);
}


int CPageDB::GetNextInsertID()
{
	int nID = 0;
	std::list<CPage> list_page;
	CPageDB pagedb;
	pagedb.Select(_T("SELECT * FROM Page ORDER BY ID DESC LIMIT 1"), list_page);
	if (list_page.size() == 1)
		nID = list_page.front().GetID()+1;
	return nID;
}