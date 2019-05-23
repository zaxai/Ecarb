// CPasswordInput.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CPasswordInput.h"
#include "afxdialogex.h"


// CPasswordInput 对话框

IMPLEMENT_DYNAMIC(CPasswordInput, CDialogEx)

CPasswordInput::CPasswordInput(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PASSWORDINPUT, pParent)
{

}

CPasswordInput::~CPasswordInput()
{
}

void CPasswordInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPasswordInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPasswordInput::OnBnClickedOk)
END_MESSAGE_MAP()


// CPasswordInput 消息处理程序


BOOL CPasswordInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPasswordInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPassword;
	GetDlgItemText(IDC_EDIT_PASSWORD, strPassword);
	if (strPassword == _T("carburetor"))
	{
		g_strPassword = strPassword;
	}
	else
	{
		AfxMessageBox(_T("Password Error"));
		return;
	}
	CDialogEx::OnOK();
}


bool CPasswordInput::CheckPassword()
{
	if (g_strPassword == _T("carburetor"))
		return true;
	CPasswordInput pswdinput;
	if (pswdinput.DoModal() == IDOK)
		return true;
	return false;
}