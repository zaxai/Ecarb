#include "stdafx.h"
#include "CCommItem.h"


CCommItem::CCommItem()
	: m_p_wnd(nullptr)
	, m_p_dataitem(nullptr)
	, m_nDataItemIndex(0)
	, m_bIsWaitWrite(false)
	, m_bIsWaitRead(false)
	, m_strSendData(_T(""))
	, m_strRecvData(_T(""))
	, m_dTgtData(0.0)
	, m_dwTgtTime(0)
	, m_bIsRecording(false)
	, m_nRecordItemID(0)
{
}


CCommItem::CCommItem(CWnd * p_wnd, CDataItem * dataitem, int nDataItemIndex)
	: m_p_wnd(p_wnd)
	, m_p_dataitem(dataitem)
	, m_nDataItemIndex(nDataItemIndex)
	, m_bIsWaitWrite(false)
	, m_bIsWaitRead(false)
	, m_strSendData(_T(""))
	, m_strRecvData(_T(""))
	, m_dTgtData(0.0)
	, m_dwTgtTime(0)
	, m_bIsRecording(false)
	, m_nRecordItemID(0)
{
	m_dTgtData = dataitem->GetTgtData();
}


CCommItem::~CCommItem()
{
}