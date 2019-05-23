#pragma once
#include "CRecordItem.h"
class CRecord
{
private:
	int m_nID;
	CString m_strWndName;
	int m_nWndType;
	CString m_strTimeStart;
	std::vector<CRecordItem> m_vec_rditem;
public:
	CRecord();
	CRecord(int nID, const CString & strWndName, int nWndType, const CString & strTimeStart,const std::vector<CRecordItem> & vec_rditem);
	virtual ~CRecord();
	int GetID() const;
	CString GetWndName() const;
	int GetWndType() const;
	CString GetTimeStart() const;
	std::vector<CRecordItem> & GetRecordItem();
};

