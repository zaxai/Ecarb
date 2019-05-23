#pragma once
#include "CDataItem.h"
#include <vector>
class CCommItem
{
public:
	CWnd * m_p_wnd;
	CDataItem * m_p_dataitem;
	int m_nDataItemIndex;
	bool m_bIsWaitWrite;
	bool m_bIsWaitRead;
	CString m_strSendData;
	CString m_strRecvData;
	double m_dTgtData;
	DWORD m_dwTgtTime;
	bool m_bIsRecording;
	int m_nRecordItemID;
	std::vector<double> m_vec_dTgtData;
	std::vector<DWORD> m_vec_dwTgtTime;
public:
	CCommItem();
	CCommItem(CWnd * p_wnd, CDataItem * dataitem, int nDataItemIndex);
	virtual ~CCommItem();
};

