// CEcarbActivation.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CEcarbActivation.h"
#include "afxdialogex.h"


// CEcarbActivation 对话框

IMPLEMENT_DYNAMIC(CEcarbActivation, CDialogEx)

CEcarbActivation::CEcarbActivation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ECARBACTIVATION, pParent)
{

}

CEcarbActivation::~CEcarbActivation()
{
}

void CEcarbActivation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEcarbActivation, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CEcarbActivation::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDOK, &CEcarbActivation::OnBnClickedOk)
END_MESSAGE_MAP()


// CEcarbActivation 消息处理程序


BOOL CEcarbActivation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strSerialNumber = CActivation::GetSerialNumber();
	m_strActivationFile = ZUtil::GetExeCatalogPath() + _T("\\Ecarb.af");
	SetDlgItemText(IDC_EDIT_SERIALNUMBER, m_strSerialNumber);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CEcarbActivation::OnBnClickedButtonImport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter;
	strFilter = _T("AF files(*.af)|*.af|All files(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("af"), NULL, 0, strFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		CString strPath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_FILEPATH, strPath);
		if (strPath != m_strActivationFile)
			::CopyFile(strPath, m_strActivationFile, FALSE);
	}
}


void CEcarbActivation::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_strSerialNumber.IsEmpty())
	{
		AfxMessageBox(_T("Activate failed,failed to get serial number"));
		return;
	}
	if (!CActivation::Verify(m_strSerialNumber, m_strActivationFile))
	{
		AfxMessageBox(_T("Activate failed,please try to use another activation file"));
		return;
	}
	CDialogEx::OnOK();
}
