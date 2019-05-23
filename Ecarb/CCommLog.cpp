// CCommLog.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CCommLog.h"
#include "afxdialogex.h"


// CCommLog 对话框

IMPLEMENT_DYNAMIC(CCommLog, CDialogEx)

CCommLog::CCommLog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMMLOG, pParent)
{

}

CCommLog::~CCommLog()
{
}

void CCommLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}


BEGIN_MESSAGE_MAP(CCommLog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOG, &CCommLog::OnBnClickedButtonLog)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCommLog::OnBnClickedButtonClear)
	ON_MESSAGE(WM_MSGRECVPRO, &CCommLog::OnMsgrecvpro)
END_MESSAGE_MAP()


// CCommLog 消息处理程序


BOOL CCommLog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCommLog::InitList()
{
	CRect rc;
	m_listLog.GetClientRect(rc);
	m_listLog.SetExtendedStyle(m_listLog.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listLog.InsertColumn(0, _T("Flag"), LVCFMT_CENTER, rc.Width() / 6, 0);
	m_listLog.InsertColumn(1, _T("Data frame"), LVCFMT_CENTER, rc.Width()*3 / 6, 1);
	m_listLog.InsertColumn(2, _T("Time"), LVCFMT_CENTER, rc.Width() / 6, 2);
}


void CCommLog::OnBnClickedButtonLog()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bIsLogChecking = !g_bIsLogChecking;
	if (g_bIsLogChecking)
		SetDlgItemText(IDC_BUTTON_LOG, _T("Hide log"));
	else
		SetDlgItemText(IDC_BUTTON_LOG, _T("Show log"));
}


void CCommLog::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listLog.DeleteAllItems();
}


afx_msg LRESULT CCommLog::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SENDSUCCEED:
	{
		CString * p_str = (CString *)wParam;
		InsertList(_T("Send"), *p_str);
	}
	break;
	case MSGUSER_RECVSUCCEED:
	{
		CString * p_str = (CString *)wParam;
		InsertList(_T("Receive"), *p_str);
	}
	break;
	}
	return 0;
}


void CCommLog::InsertList(const CString strFlag, const CString & strData)
{
	int nIndex = m_listLog.GetItemCount();
	CString strTime;
	SYSTEMTIME stTime;
	::GetLocalTime(&stTime);
	strTime.Format(_T("%02d:%02d:%02d.%03d"), stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds);
	m_listLog.InsertItem(nIndex,strFlag);
	m_listLog.SetItemText(nIndex, 1, DataFormat(strData));
	m_listLog.SetItemText(nIndex, 2, strTime);
	m_listLog.EnsureVisible(nIndex, FALSE);
}


CString CCommLog::DataFormat(const CString & str)
{
	CString strByte, strData;
	int nSize = str.GetLength();
	if (nSize < 2)
		return str;
	for (int i = 0; i < nSize; i = i + 2)
	{
		strByte = str.Mid(i, 2);
		strData += strByte + _T(" ");
	}
	return strData;
}

