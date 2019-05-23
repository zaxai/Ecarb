// CBasicWnd.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CBasicWnd.h"
#include "afxdialogex.h"
#include "CPasswordInput.h"


// CBasicWnd 对话框
const COLORREF crBlank = ::GetSysColor(COLOR_WINDOW);
const COLORREF crRight = RGB(0, 255, 0);
const COLORREF crWrong = RGB(255, 0, 0);
const int nTimerUpdateUI = 0;
const int nTimerSpan = 500;

IMPLEMENT_DYNAMIC(CBasicWnd, CDialogEx)

CBasicWnd::CBasicWnd(CWndInfo * p_wndinfo,CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BASICWND, pParent)
	, m_p_listItem(nullptr)
	, m_p_btnRecord(nullptr)
	, m_p_btnRead(nullptr)
	, m_p_btnWrite(nullptr)
	, m_p_btnZoomIn(nullptr)
	, m_p_btnZoomOut(nullptr)
	, m_p_wndinfo(p_wndinfo)
	, m_bIsInitFinish(false)
	, m_bIsRecording(false)
	, m_nRecordID(-1)
	, m_dwTimeStart(0)
	, m_bIsSavingRecords(false)
{

}

CBasicWnd::~CBasicWnd()
{
	StopSave();
	delete m_p_listItem;
	delete m_p_btnRecord;
	delete m_p_btnRead;
	delete m_p_btnWrite;
	delete m_p_btnZoomIn;
	delete m_p_btnZoomOut;
	for (CStatic * p : m_list_p_static)
		delete p;
}

void CBasicWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBasicWnd, CDialogEx)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OPERATE_ITEM, &CBasicWnd::OnOperateItem)
	ON_UPDATE_COMMAND_UI(ID_OPERATE_ITEM, &CBasicWnd::OnUpdateOperateItem)
	ON_MESSAGE(WM_MSGRECVPRO, &CBasicWnd::OnMsgrecvpro)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CBasicWnd::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CBasicWnd::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CBasicWnd::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMIN, &CBasicWnd::OnBnClickedButtonZoomin)
	ON_BN_CLICKED(IDC_BUTTON_ZOOMOUT, &CBasicWnd::OnBnClickedButtonZoomout)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBasicWnd 消息处理程序


BOOL CBasicWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitData();
	InitUI();
	m_bIsInitFinish = true;
	MoveWindow(m_p_wndinfo->GetRect());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBasicWnd::InitData()
{
	for (const CWndItem & wnditem : m_p_wndinfo->GetWndItem())
	{
		CDataItem dataitem;
		if (CDataItemDB::GetDataItem(wnditem.GetDataItemID(), dataitem))
			m_list_dataitem.push_back(dataitem);//插入dataitem内存
		else
		{
			g_wnditemdb.Delete(wnditem);//删除wnditem数据库
			m_p_wndinfo->GetWndItem().remove(wnditem);//删除wnditem内存
		}
	}
	int i = 0;
	for (CDataItem & dataitem : m_list_dataitem)
	{
		m_list_commitem.push_back(CCommItem(this, &dataitem, i));//插入commitem内存
		++i;
	}
}


void CBasicWnd::InitUI()
{
	CString strTitle;
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	{
		strTitle = m_p_wndinfo->GetName() + _T("-Calibrate");
		InitButtonCalibrate();
		InitButtonZoom();
		InitList();
	}
	break;
	case CWndInfo::TYPE_MONITOR:
	{
		strTitle = m_p_wndinfo->GetName() + _T("-Monitor");
		InitButtonRecord();
		InitButtonZoom();
		InitList();
	}
	break;
	case CWndInfo::TYPE_DASHBOARD:
	{
		strTitle = m_p_wndinfo->GetName() + _T("-Dashboard");
		InitButtonRecord();
		InitStatic();
	}
	break;
	case CWndInfo::TYPE_WAVE:
	{
		strTitle = m_p_wndinfo->GetName() + _T("-Wave");
		InitButtonRecord();
		InitStatic();
	}
	break;
	case CWndInfo::TYPE_STATUSLED:
	{
		strTitle = m_p_wndinfo->GetName() + _T("-Status LED");
		InitButtonRecord();
		InitStatic();
	}
	break;
	}
	SetWindowText(strTitle);
}


void CBasicWnd::ClearUI()
{
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	case CWndInfo::TYPE_MONITOR:
	{
		m_p_listItem->DeleteAllItems();//删除dataitem界面
	}
	break;
	case CWndInfo::TYPE_DASHBOARD:
	case CWndInfo::TYPE_WAVE:
	case CWndInfo::TYPE_STATUSLED:
	{
		for (CStatic * p : m_list_p_static)
		{
			p->DestroyWindow();//删除dataitem界面
			delete p;
		}
		m_list_p_static.clear();
	}
	break;
	}
}


void CBasicWnd::InsertUI(int nIndex, const CDataItem & dataitem)
{
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	case CWndInfo::TYPE_MONITOR:
	{
		InsertList(nIndex, dataitem);//插入dataitem界面
	}
	break;
	case CWndInfo::TYPE_DASHBOARD:
	case CWndInfo::TYPE_WAVE:
	case CWndInfo::TYPE_STATUSLED:
	{
		InsertStatic(nIndex, dataitem);//插入dataitem界面
	}
	break;
	}
}


void CBasicWnd::UpdateUI(int nIndex, const CDataItem & dataitem)
{
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	case CWndInfo::TYPE_MONITOR:
	{
		UpdateList(nIndex,dataitem);//更新dataitem界面
	}
	break;
	case CWndInfo::TYPE_DASHBOARD:
	case CWndInfo::TYPE_WAVE:
	case CWndInfo::TYPE_STATUSLED:
	{
		UpdateStatic(nIndex);//更新dataitem界面
	}
	break;
	}
}


void CBasicWnd::DeleteUI(int nIndex, const CDataItem & dataitem)
{
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	case CWndInfo::TYPE_MONITOR:
	{
		DeleteList(nIndex, dataitem);//删除dataitem界面
	}
	break;
	case CWndInfo::TYPE_DASHBOARD:
	case CWndInfo::TYPE_WAVE:
	case CWndInfo::TYPE_STATUSLED:
	{
		DeleteStatic(nIndex, dataitem);//删除dataitem界面
	}
	break;
	}
}


void CBasicWnd::InitButtonRecord()
{
	m_p_btnRecord = new CButton();
	m_p_btnRecord->Create(_T("Record"), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX| WS_TABSTOP, CRect(10, 10, 100, 30), this, IDC_BUTTON_RECORD);
	m_p_btnRecord->SetFont(GetFont());
}


void CBasicWnd::InitButtonCalibrate()
{
	m_p_btnRead = new CButton();
	m_p_btnRead->Create(_T("Read"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, CRect(10, 10, 60, 30), this, IDC_BUTTON_READ);
	m_p_btnRead->SetFont(GetFont());
	m_p_btnWrite = new CButton();
	m_p_btnWrite->Create(_T("Write"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, CRect(60, 10, 110, 30), this, IDC_BUTTON_WRITE);
	m_p_btnWrite->SetFont(GetFont());
}


void CBasicWnd::InitButtonZoom()
{
	CRect rc;
	m_p_btnZoomIn = new CButton();
	m_p_btnZoomIn->Create(_T("》"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON| WS_TABSTOP, rc, this, IDC_BUTTON_ZOOMIN);
	m_p_btnZoomIn->SetFont(GetFont());
	m_p_btnZoomOut = new CButton();
	m_p_btnZoomOut->Create(_T("《"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON| WS_TABSTOP, rc, this, IDC_BUTTON_ZOOMOUT);
	m_p_btnZoomOut->SetFont(GetFont());
}


void CBasicWnd::InitList()
{
	CRect rc;
	m_p_listItem = new ZListCtrl(this);
	m_p_listItem->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP, rc, this, IDC_LIST_DATAITEM);
	m_p_listItem->SetExtendedStyle(m_p_listItem->GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_p_listItem->InsertColumn(0, _T("Name"), LVCFMT_CENTER, 80, 0);
	m_p_listItem->InsertColumn(1, _T("Start address"), LVCFMT_CENTER, 100, 1);
	m_p_listItem->InsertColumn(2, _T("Data"), LVCFMT_CENTER, 80, 2);
	m_p_listItem->InsertColumn(3, _T("Unit"), LVCFMT_CENTER, 80, 3);
	m_p_listItem->EnableEdit(TRUE);
	int i = 0;
	for (const CDataItem & dataitem : m_list_dataitem)
	{
		InsertList(i, dataitem);
		++i;
	}
}


void CBasicWnd::InsertList(int nIndex, const CDataItem & dataitem)
{
	CString strStartAddr,strTgtData;
	strStartAddr = _T("0x")+dataitem.GetRegStartAddr();
	strTgtData = ZUtil::GetDoubleString(dataitem.GetTgtData(), dataitem.GetDecimalDigits());
	m_p_listItem->InsertItem(nIndex, dataitem.GetName());
	m_p_listItem->SetItemText(nIndex, 1, strStartAddr);
	m_p_listItem->SetItemText(nIndex, 2, strTgtData);
	m_p_listItem->SetItemText(nIndex, 3, dataitem.GetUnit());
	if (m_p_wndinfo->GetWndType() == CWndInfo::TYPE_CALIBRATE)
		m_p_listItem->SetItemEnableFlag(nIndex, 2, TRUE);
}


void CBasicWnd::UpdateList(int nIndex, const CDataItem & dataitem)
{
	CString strStartAddr, strTgtData;
	strStartAddr = _T("0x") + dataitem.GetRegStartAddr();
	strTgtData = ZUtil::GetDoubleString(dataitem.GetTgtData(), dataitem.GetDecimalDigits());
	m_p_listItem->SetItemText(nIndex, 0, dataitem.GetName());
	m_p_listItem->SetItemText(nIndex, 1, strStartAddr);
	m_p_listItem->SetItemText(nIndex, 2, strTgtData);
	m_p_listItem->SetItemText(nIndex, 3, dataitem.GetUnit());
}


void CBasicWnd::DeleteList(int nIndex, const CDataItem & dataitem)
{
	m_p_listItem->DeleteItem(nIndex);
}


void CBasicWnd::InitStatic()
{
	int i = 0;
	for (const CDataItem & dataitem : m_list_dataitem)
	{
		InsertStatic(i, dataitem);
		++i;
	}
}


void CBasicWnd::InsertStatic(int nIndex, const CDataItem & dataitem)
{
	CStatic * p = nullptr;
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_DASHBOARD:
		p = new CDashboardStatic(&dataitem);
		break;
	case CWndInfo::TYPE_WAVE:
		p = new CWaveStatic(&dataitem);
		break;
	case CWndInfo::TYPE_STATUSLED:
		p = new CStatusLEDStatic(&dataitem);
		break;
	}
	CRect rc;
	p->Create(_T("Static"), WS_CHILD | WS_VISIBLE | WS_BORDER,rc,this, IDC_STATIC_START+nIndex);
	m_list_p_static.push_back(p);
	PostMessage(WM_SIZE);
}


void CBasicWnd::UpdateStatic(int nIndex)
{
	auto it = m_list_p_static.begin();
	while (nIndex--)
		++it;
	(*it)->Invalidate();
}


void CBasicWnd::DeleteStatic(int nIndex, const CDataItem & dataitem)
{
	auto it = m_list_p_static.begin();
	while (nIndex--)
		++it;
	(*it)->DestroyWindow();
	delete (*it);
	m_list_p_static.erase(it);
	PostMessage(WM_SIZE);
}


void CBasicWnd::SetWndInfo(CWndInfo * p_wndinfo)
{
	m_p_wndinfo = p_wndinfo;
}


CWndInfo * CBasicWnd::GetWndInfo()
{
	return m_p_wndinfo;
}


int CBasicWnd::GetPageID() const
{
	return m_p_wndinfo->GetPageID();
}


std::list<CCommItem> & CBasicWnd::GetCommItem()
{
	return m_list_commitem;
}


void CBasicWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_BASICWND, point.x, point.y, this, TRUE);
}


int CBasicWnd::GetItemType(int nWndType)
{
	int nItemType = CDataItem::TYPE_CALIBRATE;
	switch (m_p_wndinfo->GetWndType())
	{
	case CWndInfo::TYPE_CALIBRATE:
	{
		nItemType = CDataItem::TYPE_CALIBRATE;
	}
	break;
	case CWndInfo::TYPE_MONITOR:
	case CWndInfo::TYPE_DASHBOARD:
	case CWndInfo::TYPE_WAVE:
	case CWndInfo::TYPE_STATUSLED:
	{
		nItemType = CDataItem::TYPE_MONITOR;
	}
	break;
	}
	return nItemType;
}


void CBasicWnd::OnOperateItem()
{
	// TODO: 在此添加命令处理程序代码
	std::list<CDataItem> list_itemDB;
	CString strSql;
	int nItemType;
	nItemType=GetItemType(m_p_wndinfo->GetWndType());
	strSql.Format(_T("SELECT * FROM DataItem WHERE ItemType=%d ORDER BY ID"), nItemType);
	g_dataitemdb.Select(strSql, list_itemDB);
	CItemOperate itemope;
	itemope.SetParam(m_list_dataitem, list_itemDB);
	if (itemope.DoModal() == IDOK)
	{
		itemope.GetParam(m_list_dataitem, list_itemDB);//更新dataitem内存
		for (const CWndItem & wnditem : m_p_wndinfo->GetWndItem())
			g_wnditemdb.Delete(wnditem);//删除wnditem数据库
		m_p_wndinfo->GetWndItem().clear();//删除wnditem内存
		ClearUI();//删除dataitem界面
		m_list_commitem.clear();//删除commitem内存
		int i = 0;
		for (CDataItem & dataitem : m_list_dataitem)
		{
			CWndItem wnditem(CWndItemDB::GetNextInsertID(),dataitem.GetID(), m_p_wndinfo->GetID());
			if (g_wnditemdb.Insert(wnditem))//插入wnditem数据库
			{
				m_p_wndinfo->GetWndItem().push_back(wnditem);//插入wnditem内存
				InsertUI(i,dataitem);//插入dataitem界面
				m_list_commitem.push_back(CCommItem(this, &dataitem, i));//插入commitem内存
				++i;
			}
		}
	}
}


void CBasicWnd::OnUpdateOperateItem(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating|| m_bIsSavingRecords)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}



afx_msg LRESULT CBasicWnd::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_COMMSTART:
	{
		m_dwTimeStart = DWORD(HighResolutionTimeCounter::NowOfMillisecond());
		SetRecordID();
		StartRecord();
		SetTimer(nTimerUpdateUI, nTimerSpan, nullptr);
		if (m_p_btnRecord)
			m_p_btnRecord->EnableWindow(FALSE);
		if (m_p_wndinfo->GetWndType() == CWndInfo::TYPE_WAVE)
		{
			for (CStatic * p : m_list_p_static)
			{
				CWaveStatic * p_wave = (CWaveStatic *)p;
				p_wave->StartComm(m_dwTimeStart);
			}
		}
	}
	break;
	case MSGUSER_COMMSTOP:
	{
		StartSave();
		KillTimer(nTimerUpdateUI);
		if (m_p_btnRecord)
			m_p_btnRecord->EnableWindow(TRUE);
		if (m_p_wndinfo->GetWndType() == CWndInfo::TYPE_WAVE)
		{
			for (CStatic * p : m_list_p_static)
			{
				CWaveStatic * p_wave = (CWaveStatic *)p;
				p_wave->StopComm();
			}
		}
	}
	break;
	case MSGUSER_SENDSUCCEED:
	{
		if (m_p_wndinfo->GetWndType() == CWndInfo::TYPE_CALIBRATE)
			m_p_listItem->SetItemBkColor(wParam, 2, crBlank);
	}
	break;
	case MSGUSER_RECVSUCCEED:
	{
		switch (m_p_wndinfo->GetWndType())
		{
		case CWndInfo::TYPE_CALIBRATE:
		case CWndInfo::TYPE_MONITOR:
		{
			m_p_listItem->SetItemBkColor(wParam, 2, crRight);
		}
		break;
		case CWndInfo::TYPE_DASHBOARD:
		{
		}
		break;
		case CWndInfo::TYPE_WAVE:
		{
			auto itStatic = m_list_p_static.begin();
			auto itCommItem = m_list_commitem.begin();
			while (wParam--)
			{
				++itStatic;
				++itCommItem;
			}
			CWaveStatic * p = (CWaveStatic *)(*itStatic);
			p->AddTgtData(itCommItem->m_dTgtData, itCommItem->m_dwTgtTime-m_dwTimeStart, FALSE);
		}
		break;
		case CWndInfo::TYPE_STATUSLED:
		{
		}
		break;
		}
	}
	break;
	case MSGUSER_SENDFAIL:
	{
		if (m_p_wndinfo->GetWndType() == CWndInfo::TYPE_CALIBRATE)
			m_p_listItem->SetItemBkColor(wParam, 2, crBlank);
	}
	break;
	case MSGUSER_RECVFAIL:
	{
		switch (m_p_wndinfo->GetWndType())
		{
		case CWndInfo::TYPE_CALIBRATE:
		case CWndInfo::TYPE_MONITOR:
		{
			m_p_listItem->SetItemBkColor(wParam, 2, crWrong);
		}
		break;
		case CWndInfo::TYPE_DASHBOARD:
		{
		}
		break;
		case CWndInfo::TYPE_WAVE:
		{
		}
		break;
		case CWndInfo::TYPE_STATUSLED:
		{
		}
		break;
		}
	}
	break;
	case MSGUSER_UPDATEDATAITEM:
	{
		int nSize = m_list_dataitem.size();
		auto itDataItem = m_list_dataitem.begin();
		auto itCommItem = m_list_commitem.begin();
		for (int i=0;i<nSize;++i)
		{
			if (itDataItem->GetID() == wParam)
			{
				CDataItemDB::GetDataItem(itDataItem->GetID(), *itDataItem);//更新dataitem内存
				itCommItem->m_dTgtData = itDataItem->GetTgtData();//更新commitem内存
				UpdateUI(i, *itDataItem);//更新dataitem界面
				break;
			}
			++itDataItem;
			++itCommItem;
		}
	}
	break;
	case MSGUSER_DELETEDATAITEM:
	{
		if (!m_bIsSavingRecords)
		{
			int nSize = m_list_dataitem.size();
			auto itDataItem = m_list_dataitem.begin();
			auto itCommItem = m_list_commitem.begin();
			auto itWndItem = m_p_wndinfo->GetWndItem().begin();
			for (int i = 0; i < nSize; ++i)
			{
				if (itDataItem->GetID() == wParam)
				{
					if (g_wnditemdb.Delete(*itWndItem))//删除wnditem数据库
					{
						m_p_wndinfo->GetWndItem().erase(itWndItem);//删除wnditem内存
						DeleteUI(i, *itDataItem);//删除dataitem界面	
						m_list_commitem.erase(itCommItem);//删除commitem内存
						m_list_dataitem.erase(itDataItem);//删除dataitem内存
					}
					break;
				}
				else
				{
					++itDataItem;
					++itCommItem;
					++itWndItem;
				}
			}
			int j = 0;
			for (CCommItem & commitem : m_list_commitem)
			{
				commitem.m_nDataItemIndex = j;
				++j;
			}
		}
	}
	break;
	case MSGUSER_ABANDONSAVERECORD:
	{
		StopSave();
	}
	break;
	}
	return 0;
}


void CBasicWnd::OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew)
{
	int nIndex = ii.m_nItem;
	auto itDataItem = m_list_dataitem.begin();
	auto itCommItem = m_list_commitem.begin();
	while (nIndex--)
	{
		++itDataItem;
		++itCommItem;
	}
	itDataItem->SetTgtData(_ttof(strNew));
	itCommItem->m_dTgtData = _ttof(strNew);
	itCommItem->m_bIsWaitWrite = true;
	itCommItem->m_bIsWaitRead = true;
	g_dataitemdb.Update(*itDataItem);
}


void CBasicWnd::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!CPasswordInput::CheckPassword())
		return;
	if (GetParent())
		((CFrameWndEx *)GetParent())->GetActiveView()->PostMessage(WM_MSGRECVPRO, (WPARAM)this, MSGUSER_CLOSEBASICWND);
	__super::OnClose();
}


void CBasicWnd::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_bIsInitFinish)
	{
		CRect rc,rcZoomIn,rcZoomOut;
		int nZoomWidth = 30;
		GetClientRect(rc);
		rcZoomIn = CRect(rc.right-10- nZoomWidth,rc.top+10, rc.right - 10,rc.top+30);
		rcZoomOut = CRect(rcZoomIn.left - nZoomWidth, rc.top + 10, rcZoomIn.left, rc.top + 30);
		rc.top += 30;
		rc.DeflateRect(10, 10);
		switch (m_p_wndinfo->GetWndType())
		{
		case CWndInfo::TYPE_CALIBRATE:
		{
			m_p_btnZoomIn->MoveWindow(rcZoomIn);
			m_p_btnZoomIn->Invalidate();
			m_p_btnZoomOut->MoveWindow(rcZoomOut);
			m_p_btnZoomOut->Invalidate();
			m_p_listItem->MoveWindow(rc);
		}
		break;
		case CWndInfo::TYPE_MONITOR:
		{
			m_p_btnZoomIn->MoveWindow(rcZoomIn);
			m_p_btnZoomIn->Invalidate();
			m_p_btnZoomOut->MoveWindow(rcZoomOut);
			m_p_btnZoomOut->Invalidate();
			m_p_listItem->MoveWindow(rc);
		}
		break;
		case CWndInfo::TYPE_DASHBOARD:
		{
			StaticResize(rc,3,1.0);
		}
		break;
		case CWndInfo::TYPE_WAVE:
		{
			StaticResize(rc,1,2.5);
		}
		break;
		case CWndInfo::TYPE_STATUSLED:
		{
			StaticResize(rc,5,1.0);
		}
		break;
		}
	}
}


void CBasicWnd::ListResize(bool bIsZoomIn)
{
	const int nWidthSpan = 20;
	int nColumn = m_p_listItem->GetHeaderCtrl()->GetItemCount();
	if (bIsZoomIn)
	{
		for (int i = 0; i < nColumn; ++i)
			m_p_listItem->SetColumnWidth(i, m_p_listItem->GetColumnWidth(i)+ nWidthSpan);
	}
	else
	{
		bool bCanZoomOut = true;
		for (int i = 0; i < nColumn; ++i)
		{
			if (m_p_listItem->GetColumnWidth(i) <= nWidthSpan)
			{
				bCanZoomOut = false;
				break;
			}
		}
		if (bCanZoomOut)
		{
			for (int i = 0; i < nColumn; ++i)
				m_p_listItem->SetColumnWidth(i, m_p_listItem->GetColumnWidth(i) - nWidthSpan);
		}
	}
}


void CBasicWnd::StaticResize(CRect & rc, int nMaxColumn,double dRatioWidthToHeight)
{
	if (m_list_p_static.size())
	{
		int nRow, nColumn;
		nRow = m_list_p_static.size() / nMaxColumn;
		nColumn = (int)m_list_p_static.size() >= nMaxColumn ? nMaxColumn : m_list_p_static.size() % nMaxColumn;
		if (m_list_p_static.size() % nMaxColumn)
			++nRow;
		int nWidth,nHeight;
		if (rc.Width() / nColumn / dRatioWidthToHeight > rc.Height() / nRow)
		{
			nHeight = rc.Height() / nRow;
			nWidth = int(nHeight * dRatioWidthToHeight);
		}	
		else
		{
			nWidth = rc.Width() / nColumn;
			nHeight = int(nWidth / dRatioWidthToHeight);
		}
		int i = 0;
		for (CStatic * p : m_list_p_static)
		{
			CRect rcItem(rc.left + (i % nColumn) * nWidth, rc.top + i / nMaxColumn * nHeight, rc.left + (i % nColumn) * nWidth + nWidth, rc.top + i / nMaxColumn * nHeight + nHeight);
			p->MoveWindow(rcItem);
			p->Invalidate();
			++i;
		}
	}
}


void CBasicWnd::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetWindowRect(rc);
	m_p_wndinfo->SetRect(rc);//更新wndinfo内存
	g_wndinfodb.UpdateFull(*m_p_wndinfo);//更新wndinfo数据库
}


void CBasicWnd::OnBnClickedButtonRecord()
{
	m_bIsRecording = m_p_btnRecord->GetCheck();
}


void CBasicWnd::OnBnClickedButtonRead()
{
	int i = 0;
	for (CCommItem & commitem : m_list_commitem)
	{
		commitem.m_bIsWaitWrite = false;
		commitem.m_bIsWaitRead = true;
		m_p_listItem->SetItemBkColor(i, 2, crBlank);
		++i;
	}
}


void CBasicWnd::OnBnClickedButtonWrite()
{
	int i = 0;
	for (CCommItem & commitem : m_list_commitem)
	{
		commitem.m_bIsWaitWrite = true;
		commitem.m_bIsWaitRead = true;
		m_p_listItem->SetItemBkColor(i, 2, crBlank);
		++i;
	}
}


void CBasicWnd::OnBnClickedButtonZoomin()
{
	ListResize(true);
}


void CBasicWnd::OnBnClickedButtonZoomout()
{
	ListResize(false);
}


void CBasicWnd::SetRecordID()
{
	if (g_bIsCommunicating&&m_bIsRecording)
	{
		if (m_nRecordID == -1)
		{
			m_nRecordID = g_nRecordID++;
		}
		else
		{
			std::vector<CRecord> vec_record;
			CString strSql;
			strSql.Format(_T("SELECT * FROM Record WHERE ID=%d"), m_nRecordID);
			g_recorddb.SelectFull(strSql, vec_record);
			if (vec_record.size())
				m_nRecordID = g_nRecordID++;
		}
	}
}


void CBasicWnd::StartRecord()
{
	for (CCommItem & commitem : m_list_commitem)
	{
		commitem.m_vec_dTgtData.clear();
		commitem.m_vec_dwTgtTime.clear();
		commitem.m_vec_dTgtData.reserve(10240);
		commitem.m_vec_dwTgtTime.reserve(10240);
		commitem.m_bIsRecording = m_bIsRecording;
		if (commitem.m_bIsRecording)
		{
			CDataItem & dataitem = *(commitem.m_p_dataitem);
			std::vector<CRecordItem> vec_rditem;
			CRecord record(m_nRecordID, m_p_wndinfo->GetName(), m_p_wndinfo->GetWndType(), _T(""), vec_rditem);
			g_recorddb.Insert(record);
			std::vector<CRecordData> vec_rddata;
			commitem.m_nRecordItemID = CRecordItemDB::GetNextInsertID();
			CRecordItem rditem(commitem.m_nRecordItemID, dataitem.GetID(), dataitem.GetItemType(), dataitem.GetName(), dataitem.GetUnit(), dataitem.GetRegStartAddr(), dataitem.GetRegNum(), dataitem.GetDecimalDigits(), dataitem.GetMultiplier(), dataitem.GetConstant(), dataitem.GetMaxData(), dataitem.GetMinData(), record.GetID(), vec_rddata);
			g_rditemdb.Insert(rditem);
		}
	}
}


void CBasicWnd::SaveRecord()
{
	m_bIsSavingRecords = true;
	for (CCommItem & commitem : m_list_commitem)
	{
		if (commitem.m_bIsRecording)
			g_nTotalRecords += commitem.m_vec_dTgtData.size();
	}
	for (CCommItem & commitem : m_list_commitem)
	{
		if (commitem.m_bIsRecording)
		{
			std::lock_guard<std::mutex> guard(g_mutex);
			int nSize = commitem.m_vec_dTgtData.size();
			std::vector<CRecordData> vec_rddata;
			vec_rddata.reserve(nSize);
			for (int i = 0; i < nSize; ++i)
			{
				vec_rddata.push_back(CRecordData(g_nRecordDataID, (commitem.m_vec_dTgtData)[i], (commitem.m_vec_dwTgtTime)[i] - m_dwTimeStart, commitem.m_nRecordItemID));
				++g_nRecordDataID;
			}
			g_rddatadb.Insert(vec_rddata);
			g_nSavedRecords += nSize;
			if (GetParent())
				GetParent()->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_UPDATEPROGRESS);
			if (!m_bIsSavingRecords)
				return;
		}
	}
	m_bIsSavingRecords = false;
}


void CBasicWnd::StartSave()
{
	if (m_threadSaveRecord.joinable())
		m_threadSaveRecord.join();
	m_threadSaveRecord = std::thread(&CBasicWnd::SaveRecord, this);
}


void CBasicWnd::StopSave()
{
	m_bIsSavingRecords = false;
	if (m_threadSaveRecord.joinable())
		m_threadSaveRecord.join();
}


void CBasicWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case nTimerUpdateUI:
	{
		int nSize = m_list_dataitem.size();
		auto itDataItem = m_list_dataitem.begin();
		auto itCommItem = m_list_commitem.begin();
		for (int i = 0; i < nSize; ++i)
		{
			itDataItem->SetTgtData(itCommItem->m_dTgtData);
			UpdateUI(i, *itDataItem);
			++itDataItem;
			++itCommItem;
		}
	}
	break;
	}
	__super::OnTimer(nIDEvent);
}
