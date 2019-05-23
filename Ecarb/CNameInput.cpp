// CNameInput.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CNameInput.h"
#include "afxdialogex.h"


// CNameInput 对话框

IMPLEMENT_DYNAMIC(CNameInput, CDialogEx)

CNameInput::CNameInput(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NAMEINPUT, pParent)
{

}

CNameInput::~CNameInput()
{
}

void CNameInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNameInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNameInput::OnBnClickedOk)
END_MESSAGE_MAP()


// CNameInput 消息处理程序


BOOL CNameInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(m_strTitle);
	SetDlgItemText(IDC_EDIT_NAME, m_strName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CNameInput::SetParam(const CString & strTitle, const CString & strName)
{
	m_strTitle = strTitle;
	m_strName = strName;
}


void CNameInput::GetParam(CString & strTitle, CString & strName)
{
	strTitle = m_strTitle;
	strName = m_strName;
}



void CNameInput::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT_NAME, m_strName);
	CDialogEx::OnOK();
}
