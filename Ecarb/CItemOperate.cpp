// CItemOperate.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CItemOperate.h"
#include "afxdialogex.h"


// CItemOperate 对话框

IMPLEMENT_DYNAMIC(CItemOperate, CDialogEx)

CItemOperate::CItemOperate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITEMOPERATE, pParent)
{

}

CItemOperate::~CItemOperate()
{
}

void CItemOperate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WNDITEM, m_listWndItem);
	DDX_Control(pDX, IDC_LIST_DATABASEITEM, m_listDatabaseItem);
}


BEGIN_MESSAGE_MAP(CItemOperate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CItemOperate::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CItemOperate::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CItemOperate 消息处理程序


BOOL CItemOperate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CItemOperate::InitList()
{
	CRect rc;
	m_listWndItem.GetClientRect(rc);
	m_listWndItem.SetExtendedStyle(m_listWndItem.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listWndItem.InsertColumn(0, _T("项目名称"), LVCFMT_CENTER, rc.Width() / 3, 0);
	m_listWndItem.InsertColumn(1, _T("起始地址"), LVCFMT_CENTER, rc.Width() / 3, 1);
	m_listWndItem.InsertColumn(2, _T("初始数据"), LVCFMT_CENTER, rc.Width() / 3, 2);
	m_listDatabaseItem.GetClientRect(rc);
	m_listDatabaseItem.SetExtendedStyle(m_listDatabaseItem.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listDatabaseItem.InsertColumn(0, _T("项目名称"), LVCFMT_CENTER, rc.Width() / 3, 0);
	m_listDatabaseItem.InsertColumn(1, _T("起始地址"), LVCFMT_CENTER, rc.Width() / 3, 1);
	m_listDatabaseItem.InsertColumn(2, _T("初始数据"), LVCFMT_CENTER, rc.Width() / 3, 2);
	for (const CDataItem & dataitem : m_list_itemWnd)
		InsertList(m_listWndItem, dataitem);
	for(const CDataItem & dataitem : m_list_itemDB)
		InsertList(m_listDatabaseItem, dataitem);
}


void CItemOperate::SetParam(const std::list<CDataItem> & list_itemWnd, const std::list<CDataItem> & list_itemDB)
{
	m_list_itemWnd = list_itemWnd;
	m_list_itemDB = list_itemDB;
}


void CItemOperate::GetParam(std::list<CDataItem> & list_itemWnd, std::list<CDataItem> & list_itemDB)
{
	list_itemWnd = m_list_itemWnd;
	list_itemDB = m_list_itemDB;
}


void CItemOperate::InsertList(CListCtrl & list, const CDataItem & dataitem)
{
	int nIndex = list.GetItemCount();
	CString strTgtData;
	strTgtData = ZUtil::GetDoubleString(dataitem.GetTgtData(), dataitem.GetDecimalDigits());
	list.InsertItem(nIndex, dataitem.GetName());
	list.SetItemText(nIndex, 1, dataitem.GetRegStartAddr());
	list.SetItemText(nIndex, 2, strTgtData);
}

void CItemOperate::OnBnClickedButtonInsert()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSize = m_listDatabaseItem.GetItemCount();
	auto itItemDB = m_list_itemDB.begin();
	for (int i = 0; i < nSize; ++i)
	{
		if (m_listDatabaseItem.GetCheck(i))
		{
			auto itItemWnd = std::find(m_list_itemWnd.begin(), m_list_itemWnd.end(), *itItemDB);
			if (itItemWnd == m_list_itemWnd.end())
			{
				m_list_itemWnd.push_back(*itItemDB);
				InsertList(m_listWndItem, *itItemDB);
			}
		}
		++itItemDB;
	}
}


void CItemOperate::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSize = m_listWndItem.GetItemCount();
	for (int i = nSize - 1; i >= 0; --i)
	{
		if (m_listWndItem.GetCheck(i))
		{
			int nIndex = i;
			auto it = m_list_itemWnd.begin();
			while (nIndex--)
				++it;
			m_list_itemWnd.erase(it);
			m_listWndItem.DeleteItem(i);
		}
	}
}
