#include "StdAfx.h"
#include "ZSqlite3.h"


ZSqlite3::ZSqlite3(void)
	: m_p_sqlite3(nullptr)
{
}


ZSqlite3::ZSqlite3(ZSqlite3 && zsql)
	: m_p_sqlite3(zsql.m_p_sqlite3)
{
	zsql.m_p_sqlite3 = nullptr;
}


ZSqlite3 & ZSqlite3::operator=(ZSqlite3 && zsql)
{
	if (this != &zsql)
	{
		CloseDB();
		m_p_sqlite3 = zsql.m_p_sqlite3;
		zsql.m_p_sqlite3 = nullptr;
	}
	return *this;
}


ZSqlite3::~ZSqlite3(void)
{
	CloseDB();
}


bool ZSqlite3::OpenDB(const CString & in_strPath)
{
	CloseDB();
	if (!m_p_sqlite3)
	{
		char szPath[MAX_PATH] = { 0 };
		if (!ZUtil::TtoUTF8(in_strPath, szPath, MAX_PATH))
			return false;
		if (SQLITE_OK == sqlite3_open(szPath, &m_p_sqlite3))
			return true;
	}
	return false;
}


bool ZSqlite3::CloseDB(void)
{
	if(m_p_sqlite3)
	{
		if(SQLITE_OK==sqlite3_close(m_p_sqlite3))
		{
			m_p_sqlite3=nullptr;
			return true;
		}
		else
			return false;
	}
	return true;
}


int ZSqlite3::ExecSQL(const CString & in_strSQL, CString * out_p_strErrMsg)
{
	if(m_p_sqlite3==nullptr)
		return ERROR_INVALIDSQLITE;
	int nLength=in_strSQL.GetLength();
	nLength=6*nLength+1;
	char * p_cSQL=new char[nLength];
	if(p_cSQL==nullptr)
		return ERROR_OTHER;
	if (!ZUtil::TtoUTF8(in_strSQL, p_cSQL, nLength))
	{
		delete[]p_cSQL;
		return ERROR_OTHER;
	}
	char * p_cErrMsg=nullptr;
	int nRtn=sqlite3_exec(m_p_sqlite3,p_cSQL,nullptr,nullptr,&p_cErrMsg);
	delete []p_cSQL;
	if(nRtn!=SQLITE_OK)
	{
		if(out_p_strErrMsg)
		{
			ZUtil::UTF8toT(p_cErrMsg,out_p_strErrMsg->GetBuffer(strlen(p_cErrMsg)+1),strlen(p_cErrMsg)+1);
			out_p_strErrMsg->ReleaseBuffer();
		}
		sqlite3_free(p_cErrMsg);
		return ERROR_EXEC;
	}
	sqlite3_free(p_cErrMsg);
	return ERROR_OK;
}


int ZSqlite3::GetTable(const CString & in_strSQL, std::vector<std::vector <CString>> & out_vec2_strData, CString * out_p_strErrMsg)
{
	out_vec2_strData.clear();
	if(m_p_sqlite3==nullptr)
		return ERROR_INVALIDSQLITE;
	int nLength=in_strSQL.GetLength();
	nLength=6*nLength+1;
	char * p_cSQL=new char[nLength];
	if(p_cSQL==nullptr)
		return ERROR_OTHER;
	if (!ZUtil::TtoUTF8(in_strSQL, p_cSQL, nLength))
	{
		delete[]p_cSQL;
		return ERROR_OTHER;
	}
	char * * p_cResult=nullptr;
	int nRow=0,nColumn=0;
	char * p_cErrMsg=nullptr;
	int nRtn= sqlite3_get_table(m_p_sqlite3,p_cSQL,&p_cResult,&nRow,&nColumn,&p_cErrMsg);
	delete []p_cSQL;
	if(nRow)
	{
		for(int i=0;i<nRow+1;++i)
		{
			std::vector <CString> vec_strRow;
			for(int j=0;j<nColumn;++j)
			{
				CString str;
				if (p_cResult[i*nColumn + j])
				{
					ZUtil::UTF8toT(p_cResult[i*nColumn + j], str.GetBuffer(strlen(p_cResult[i*nColumn + j]) + 1), strlen(p_cResult[i*nColumn + j]) + 1);
					str.ReleaseBuffer();
				}
				vec_strRow.push_back(str);
			}
			out_vec2_strData.push_back(vec_strRow);
		}
	}
	sqlite3_free_table(p_cResult);
	if(nRtn!=SQLITE_OK)
	{
		if(out_p_strErrMsg)
		{
			ZUtil::UTF8toT(p_cErrMsg,out_p_strErrMsg->GetBuffer(strlen(p_cErrMsg)+1),strlen(p_cErrMsg)+1);
			out_p_strErrMsg->ReleaseBuffer();
		}
		sqlite3_free(p_cErrMsg);
		return ERROR_GETTABLE;
	}
	sqlite3_free(p_cErrMsg);
	return ERROR_OK;
}
