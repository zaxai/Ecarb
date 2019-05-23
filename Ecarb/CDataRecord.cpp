// CDataRecord.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CDataRecord.h"
#include "afxdialogex.h"

// CDataRecord 对话框

IMPLEMENT_DYNAMIC(CDataRecord, CDialogEx)

CDataRecord::CDataRecord(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATARECORD, pParent)
	, m_listRecord(this)
	, m_bIsDeleting(false)
{

}

CDataRecord::~CDataRecord()
{
	if (m_threadDelete.joinable())
		m_threadDelete.join();
}

void CDataRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RECORD, m_treeRecord);
	DDX_Control(pDX, IDC_STATIC_RECORD, m_staticRecord);
	DDX_Control(pDX, IDC_LIST_RECORD, m_listRecord);
}


BEGIN_MESSAGE_MAP(CDataRecord, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RECORD, &CDataRecord::OnTvnSelchangedTreeRecord)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECORD, &CDataRecord::OnNMDblclkListRecord)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OPERATE_REFRESHRECORD, &CDataRecord::OnOperateRefreshrecord)
	ON_COMMAND(ID_OPERATE_DELETERECORD, &CDataRecord::OnOperateDeleterecord)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_DELETERECORD, &CDataRecord::OnUpdateOperateDeleterecord)
END_MESSAGE_MAP()


// CDataRecord 消息处理程序


BOOL CDataRecord::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDataRecord::Refresh()
{
	if (!m_bIsDeleting)
	{
		InitData();
		InitTree();
		ExpandTree(m_treeRecord, m_treeRecord.GetRootItem());
	}
}


void CDataRecord::InitData()
{
	g_recorddb.Select(_T("SELECT * FROM Record ORDER BY ID"), m_vec_record);
}


void CDataRecord::InitTree()
{
	if (m_treeRecord.DeleteAllItems())
	{
		HTREEITEM hRoot,hFirst,hSecond;
		hRoot=m_treeRecord.InsertItem(_T("Records"));
		for (CRecord & record : m_vec_record)
		{
			hFirst = m_treeRecord.InsertItem(record.GetTimeStart()+_T("_")+record.GetWndName(), hRoot);
			m_treeRecord.SetItemData(hFirst, (DWORD_PTR)&record);
			for (CRecordItem & rditem:record.GetRecordItem())
			{
				hSecond = m_treeRecord.InsertItem(rditem.GetDataItemName(), hFirst);
				m_treeRecord.SetItemData(hSecond, (DWORD_PTR)&rditem);
			}
		}
	}
}


void CDataRecord::InitList()
{
	CRect rc;
	m_listRecord.GetClientRect(rc);
	m_listRecord.SetExtendedStyle(m_listRecord.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listRecord.InsertColumn(0, _T("Item name"), LVCFMT_CENTER, rc.Width() *3/ 10, 0);
	m_listRecord.InsertColumn(1, _T("Line color"), LVCFMT_CENTER, rc.Width() *3/ 10, 1);
	m_listRecord.InsertColumn(2, _T("Line width"), LVCFMT_CENTER, rc.Width() *3/ 10, 2);
	m_listRecord.EnableEdit(TRUE);
}


void CDataRecord::InsertList(const std::vector<CRecordItem> & vec_rditem, const std::vector<Gdiplus::Color> & vec_crItem, const std::vector<Gdiplus::REAL> & vec_fWidth)
{
	m_listRecord.DeleteAllItems();
	int i = 0;
	for (const CRecordItem & rditem : vec_rditem)
	{
		m_listRecord.InsertItem(i, rditem.GetDataItemName());
		COLORREF cr(vec_crItem[i].ToCOLORREF());
		m_listRecord.SetItemBkColor(i, 1, cr);
		CString strWidth;
		strWidth.Format(_T("%.01f"), vec_fWidth[i]);
		m_listRecord.SetItemText(i, 2, strWidth);
		m_listRecord.SetItemEnableFlag(i, 2, TRUE);
		++i;
	}
}


void CDataRecord::ExpandTree(CTreeCtrl & tree, HTREEITEM hTreeItem)
{
	if (!tree.ItemHasChildren(hTreeItem))
		return;
	HTREEITEM hNextItem = tree.GetChildItem(hTreeItem);
	while (hNextItem != NULL)
	{
		ExpandTree(tree, hNextItem);
		hNextItem = tree.GetNextItem(hNextItem, TVGN_NEXT);
	}
	tree.Expand(hTreeItem, TVE_EXPAND);
}


void CDataRecord::OnTvnSelchangedTreeRecord(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hItem, hChild,hParent;
	hItem = m_treeRecord.GetSelectedItem();
	if (hItem&&hItem != m_treeRecord.GetRootItem())
	{
		m_vec_rditem.clear();
		m_vec_crItem.clear();
		m_vec_fWidth.clear();
		hParent = m_treeRecord.GetParentItem(hItem);
		if (hParent == m_treeRecord.GetRootItem())
		{
			if (m_treeRecord.ItemHasChildren(hItem))
			{
				hChild = m_treeRecord.GetChildItem(hItem);
				while (hChild != NULL)
				{
					CRecordItem * p_rditem = (CRecordItem *)m_treeRecord.GetItemData(hChild);
					m_vec_rditem.push_back(*p_rditem);
					hChild = m_treeRecord.GetNextItem(hChild, TVGN_NEXT);
				} 
			}
		}
		else
		{
			CRecordItem * p_rditem = (CRecordItem *)m_treeRecord.GetItemData(hItem);
			m_vec_rditem.push_back(*p_rditem);
		}
		COLORREF sz_cr[6] = { RGB(255, 0, 0) ,RGB(0, 255, 0) ,RGB(0, 0, 255), RGB(255, 255, 0) ,RGB(255, 0, 255),RGB(0, 255, 255)};//红，绿，蓝，黄，紫，蓝绿
		int nSize = m_vec_rditem.size();
		for (int i = 0; i < nSize; ++i)
		{
			Gdiplus::Color cr;
			cr.SetFromCOLORREF(sz_cr[i % 6]);
			m_vec_crItem.push_back(cr);
			m_vec_fWidth.push_back(2.0f);
		}
		InsertList(m_vec_rditem, m_vec_crItem, m_vec_fWidth);
		m_staticRecord.SetRecordItem(m_vec_rditem, m_vec_crItem, m_vec_fWidth);
	}
}


void CDataRecord::OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew)
{
	m_vec_fWidth[ii.m_nItem] = Gdiplus::REAL(_ttof(strNew));
	m_staticRecord.SetRecordItem(m_vec_rditem, m_vec_crItem, m_vec_fWidth);
}


void CDataRecord::OnNMDblclkListRecord(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMItemActivate->iItem != -1 && pNMItemActivate->iSubItem == 1)
	{
		COLORREF cr = m_vec_crItem[pNMItemActivate->iItem].ToCOLORREF();
		CColorDialog crdlg(cr);
		if (crdlg.DoModal() == IDOK)
		{
			cr = crdlg.GetColor();
			m_vec_crItem[pNMItemActivate->iItem].SetFromCOLORREF(cr);
			m_listRecord.SetItemBkColor(pNMItemActivate->iItem, 1, cr);
			m_staticRecord.SetRecordItem(m_vec_rditem, m_vec_crItem, m_vec_fWidth);
		}
	}
}


void CDataRecord::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	m_treeRecord.GetWindowRect(&rc);
	if(rc.PtInRect(point))
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_DATARECORD, point.x, point.y, this, TRUE);
}


void CDataRecord::OnOperateRefreshrecord()
{
	// TODO: 在此添加命令处理程序代码
	Refresh();
}


void CDataRecord::OnOperateDeleterecord()
{
	// TODO: 在此添加命令处理程序代码
	if (m_bIsDeleting)
	{
		AfxMessageBox(_T("The last record is being deleted in background,please wait a moment"));
		return;
	}
	if (m_threadDelete.joinable())
		m_threadDelete.join();
	HTREEITEM hItem, hParent;
	hItem = m_treeRecord.GetSelectedItem();
	if (hItem&&hItem != m_treeRecord.GetRootItem())
	{
		hParent = m_treeRecord.GetParentItem(hItem);
		if (hParent == m_treeRecord.GetRootItem())
		{
			CRecord * p_record = (CRecord *)m_treeRecord.GetItemData(hItem);
			m_treeRecord.DeleteItem(hItem);
			m_threadDelete = std::thread(&CDataRecord::DeleteDBRecord, this, *p_record);
		}
		else
		{
			CRecordItem * p_rditem = (CRecordItem *)m_treeRecord.GetItemData(hItem);
			m_treeRecord.DeleteItem(hItem);
			m_threadDelete = std::thread(&CDataRecord::DeleteDBRecordItem, this, *p_rditem);
		}
	}
}


void CDataRecord::OnUpdateOperateDeleterecord(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CDataRecord::DeleteDBRecord(CRecord & record)
{
	m_bIsDeleting = true;
	g_recorddb.DeleteFull(record);
	m_bIsDeleting = false;
}


void CDataRecord::DeleteDBRecordItem(CRecordItem & rditem)
{
	m_bIsDeleting = true;
	g_rditemdb.DeleteFull(rditem);
	m_bIsDeleting = false;
}