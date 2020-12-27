// CComPort.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CComPort.h"
#include "afxdialogex.h"


// CComPort 对话框

IMPLEMENT_DYNAMIC(CComPort, CDialogEx)

CComPort::CComPort(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPORT, pParent)
{

}

CComPort::~CComPort()
{
}

void CComPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_comboComPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudRate);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_comboParity);
	DDX_Control(pDX, IDC_COMBO_BYTESIZE, m_comboByteSize);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_comboStopBits);
}


BEGIN_MESSAGE_MAP(CComPort, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COMPORT, &CComPort::OnBnClickedButtonComport)
END_MESSAGE_MAP()


// CComPort 消息处理程序


BOOL CComPort::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CComPort::InitUI()
{
	CString str;
	for (int i = 0; i < 100; ++i)
	{
		ZDevicePort zdp;
		str.Format(_T("COM%d"), i + 1);
		if (zdp.OpenPort(str))
			m_comboComPort.AddString(str);
	}
	if (m_comboComPort.GetCount())
		m_comboComPort.SetCurSel(0);
	CString sz_strBaudRate[] = { _T("300"),_T("600"),_T("1200"),_T("2400"),_T("4800"),_T("9600"), _T("19200"),_T("38400"),_T("57600"),_T("76800"),_T("115200") };
	for (const CString & str : sz_strBaudRate)
		m_comboBaudRate.AddString(str);
	m_comboBaudRate.SetCurSel(6);
	CString sz_strParity[] = { _T("None"),_T("Even"),_T("Odd"),_T("Mark"),_T("Space") };
	for (const CString & str : sz_strParity)
		m_comboParity.AddString(str);
	m_comboParity.SetCurSel(1);
	CString sz_strByteSize[] = { _T("5"),_T("6"),_T("7"),_T("8") };
	for (const CString & str : sz_strByteSize)
		m_comboByteSize.AddString(str);
	m_comboByteSize.SetCurSel(3);
	CString sz_strStopBits[] = { _T("1"),_T("1.5"),_T("2") };
	for (const CString & str : sz_strStopBits)
		m_comboStopBits.AddString(str);
	m_comboStopBits.SetCurSel(0);
	SetDlgItemText(IDC_EDIT_TIMEOUTSEND, _T("1000"));
	SetDlgItemText(IDC_EDIT_TIMEOUTRECV, _T("1000"));
}

void CComPort::OnBnClickedButtonComport()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool bIsOpen = false;
	if (!bIsOpen)
	{
		CString strComPort, strBaudRate, strParity, strByteSize, strStopBits;
		m_comboComPort.GetWindowText(strComPort);
		m_comboBaudRate.GetWindowText(strBaudRate);
		m_comboParity.GetWindowText(strParity);
		m_comboByteSize.GetWindowText(strByteSize);
		m_comboStopBits.GetWindowText(strStopBits);
		int nTimeoutSend, nTimeoutRecv;
		nTimeoutSend = GetDlgItemInt(IDC_EDIT_TIMEOUTSEND);
		nTimeoutRecv = GetDlgItemInt(IDC_EDIT_TIMEOUTRECV);
		if (g_zdp.OpenPort(strComPort))
		{
			g_zdp.SetPortState(strBaudRate, strParity, strByteSize, strStopBits);
			g_zdp.SetTimeOutSend(nTimeoutSend);
			g_zdp.SetTimeOutRecv(nTimeoutRecv);
			SetDlgItemText(IDC_BUTTON_COMPORT, _T("Close"));
			m_comboComPort.EnableWindow(FALSE);
			m_comboBaudRate.EnableWindow(FALSE);
			m_comboParity.EnableWindow(FALSE);
			m_comboByteSize.EnableWindow(FALSE);
			m_comboStopBits.EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TIMEOUTSEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TIMEOUTRECV)->EnableWindow(FALSE);
			bIsOpen = !bIsOpen;
		}
		else
		{
			AfxMessageBox(_T("Open error,check if the serial prot is occupied"));
		}
	}
	else
	{
		g_zdp.StopComm();
		g_zdp.ClosePort();
		SetDlgItemText(IDC_BUTTON_COMPORT, _T("Open"));
		m_comboComPort.EnableWindow(TRUE);
		m_comboBaudRate.EnableWindow(TRUE);
		m_comboParity.EnableWindow(TRUE);
		m_comboByteSize.EnableWindow(TRUE);
		m_comboStopBits.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUTSEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIMEOUTRECV)->EnableWindow(TRUE);
		bIsOpen = !bIsOpen;
	}
}
