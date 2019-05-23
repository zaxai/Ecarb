#include "stdafx.h"
#include "CWndInfoDB.h"


CWndInfoDB::CWndInfoDB()
	: m_strPathDB(_T(""))
{
	m_strPathDB = ZUtil::GetExeCatalogPath() + _T("\\Data.mdb");
	CreateTable();
}


CWndInfoDB::~CWndInfoDB()
{
}


void CWndInfoDB::CreateTable()
{
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql;
	strSql = _T("CREATE TABLE WndInfo (\
				ID INT PRIMARY KEY NOT NULL,\
				Name NVARCHAR(100),\
				WndType INT,\
				Left INT,\
				Top INT,\
				Right INT,\
				Bottom INT,\
				PageID INT\
				)");
	zsql.ExecSQL(strSql);
}


bool CWndInfoDB::Insert(const CWndInfo & wndinfo)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("INSERT INTO WndInfo VALUES (%d,'%s',%d,%d,%d,%d,%d,%d)"), wndinfo.GetID(), wndinfo.GetName(), wndinfo.GetWndType(), wndinfo.GetLeft(), wndinfo.GetTop(), wndinfo.GetRight(), wndinfo.GetBottom(), wndinfo.GetPageID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndInfoDB::InsertFull(CWndInfo & wndinfo)
{
	for (const CWndItem & wnditem : wndinfo.GetWndItem())
		m_wnditemdb.Insert(wnditem);
	return Insert(wndinfo);
}


bool CWndInfoDB::Update(const CWndInfo & wndinfo)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("UPDATE WndInfo SET Name='%s',WndType=%d,Left=%d,Top=%d,Right=%d,Bottom=%d,PageID=%d WHERE ID=%d"), wndinfo.GetName(), wndinfo.GetWndType(), wndinfo.GetLeft(), wndinfo.GetTop(), wndinfo.GetRight(), wndinfo.GetBottom(), wndinfo.GetPageID(), wndinfo.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndInfoDB::UpdateFull(CWndInfo & wndinfo)
{
	for (const CWndItem & wnditem : wndinfo.GetWndItem())
		m_wnditemdb.Update(wnditem);
	return Update(wndinfo);
}


bool CWndInfoDB::Delete(const CWndInfo & wndinfo)
{
	CreateTable();
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strSql, strError;
	strSql.Format(_T("DELETE FROM WndInfo WHERE ID=%d"), wndinfo.GetID());
	if (zsql.ExecSQL(strSql, &strError) == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndInfoDB::DeleteFull(CWndInfo & wndinfo)
{
	for (const CWndItem & wnditem : wndinfo.GetWndItem())
		m_wnditemdb.Delete(wnditem);
	return Delete(wndinfo);
}


bool CWndInfoDB::Select(const CString & strSql, std::list<CWndInfo> & list_wndinfo)
{
	list_wndinfo.clear();
	std::vector<std::vector <CString>> vec2_strData;
	ZSqlite3 zsql;
	zsql.OpenDB(m_strPathDB);
	CString strError;
	int nRtn = zsql.GetTable(strSql, vec2_strData, &strError);
	int nRow = vec2_strData.size();
	if (nRow)
	{
		int nColumn = vec2_strData[0].size();
		if (nColumn == 8)
		{
			for (int i = 1; i < nRow; ++i)
			{
				std::list<CWndItem> list_wnditem;
				CString strSunSql;
				strSunSql.Format(_T("SELECT * FROM WndItem WHERE WndInfoID=%d ORDER BY ID"), _ttoi(vec2_strData[i][0]));
				m_wnditemdb.Select(strSunSql, list_wnditem);
				CWndInfo wndinfo(_ttoi(vec2_strData[i][0]), vec2_strData[i][1], _ttoi(vec2_strData[i][2]), _ttoi(vec2_strData[i][3]), _ttoi(vec2_strData[i][4]), _ttoi(vec2_strData[i][5]), _ttoi(vec2_strData[i][6]), _ttoi(vec2_strData[i][7]), list_wnditem);
				list_wndinfo.push_back(wndinfo);
			}
		}
	}
	if (nRtn == ZSqlite3::ERROR_OK)
		return true;
	else
		return false;
}


bool CWndInfoDB::SelectFull(const CString & strSql, std::list<CWndInfo> & list_wndinfo)
{
	return Select(strSql, list_wndinfo);
}


int CWndInfoDB::GetNextInsertID()
{
	int nID = 0;
	std::list<CWndInfo> list_wndinfo;
	CWndInfoDB wndinfodb;
	wndinfodb.Select(_T("SELECT * FROM WndInfo ORDER BY ID DESC LIMIT 1"), list_wndinfo);
	if (list_wndinfo.size() == 1)
		nID = list_wndinfo.front().GetID() + 1;
	return nID;
}