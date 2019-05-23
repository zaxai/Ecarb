
// EcarbView.cpp: CEcarbView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Ecarb.h"
#endif

#include "EcarbDoc.h"
#include "EcarbView.h"
#include "CPasswordInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEcarbView

IMPLEMENT_DYNCREATE(CEcarbView, CFormView)

BEGIN_MESSAGE_MAP(CEcarbView, CFormView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEcarbView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SET_COMPORT, &CEcarbView::OnSetComport)
	ON_COMMAND(ID_SET_ITEMDB, &CEcarbView::OnSetItemdb)
	ON_COMMAND(ID_EDIT_ADDPAGE, &CEcarbView::OnEditAddpage)
	ON_COMMAND(ID_EDIT_DELETEPAGE, &CEcarbView::OnEditDeletepage)
	ON_COMMAND(ID_EDIT_ADDCALIBRATEWND, &CEcarbView::OnEditAddcalibratewnd)
	ON_COMMAND(ID_EDIT_ADDMONITORWND, &CEcarbView::OnEditAddmonitorwnd)
	ON_COMMAND(ID_EDIT_ADDDASHBOARDWND, &CEcarbView::OnEditAdddashboardwnd)
	ON_COMMAND(ID_EDIT_ADDWAVEWND, &CEcarbView::OnEditAddwavewnd)
	ON_COMMAND(ID_EDIT_ADDSTATUSLEDWND, &CEcarbView::OnEditAddstatusledwnd)
	ON_COMMAND(ID_CHECK_ITEMDB, &CEcarbView::OnCheckItemdb)
	ON_COMMAND(ID_CHECK_COMMLOG, &CEcarbView::OnCheckCommlog)
	ON_COMMAND(ID_CHECK_DATARECORD, &CEcarbView::OnCheckDatarecord)
	ON_COMMAND(ID_COMM_START, &CEcarbView::OnCommStart)
	ON_COMMAND(ID_COMM_STOP, &CEcarbView::OnCommStop)
	ON_COMMAND(ID_COMM_MODEMASTERSLAVE, &CEcarbView::OnCommModemasterslave)
	ON_COMMAND(ID_COMM_MODEAUTOREPORT, &CEcarbView::OnCommModeautoreport)
	ON_UPDATE_COMMAND_UI(ID_SET_ITEMDB, &CEcarbView::OnUpdateSetItemdb)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDPAGE, &CEcarbView::OnUpdateEditAddpage)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETEPAGE, &CEcarbView::OnUpdateEditDeletepage)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDCALIBRATEWND, &CEcarbView::OnUpdateEditAddcalibratewnd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDMONITORWND, &CEcarbView::OnUpdateEditAddmonitorwnd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDDASHBOARDWND, &CEcarbView::OnUpdateEditAdddashboardwnd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDWAVEWND, &CEcarbView::OnUpdateEditAddwavewnd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDSTATUSLEDWND, &CEcarbView::OnUpdateEditAddstatusledwnd)
	ON_UPDATE_COMMAND_UI(ID_COMM_START, &CEcarbView::OnUpdateCommStart)
	ON_UPDATE_COMMAND_UI(ID_COMM_STOP, &CEcarbView::OnUpdateCommStop)
	ON_UPDATE_COMMAND_UI(ID_COMM_MODEMASTERSLAVE, &CEcarbView::OnUpdateCommModemasterslave)
	ON_UPDATE_COMMAND_UI(ID_COMM_MODEAUTOREPORT, &CEcarbView::OnUpdateCommModeautoreport)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CEcarbView::OnTcnSelchangeTabPage)
	ON_MESSAGE(WM_MSGRECVPRO, &CEcarbView::OnMsgrecvpro)
END_MESSAGE_MAP()

// CEcarbView 构造/析构

CEcarbView::CEcarbView() noexcept
	: CFormView(IDD_ECARB_FORM)
	, m_p_itemdatabase(nullptr)
	, m_p_comport(nullptr)
	, m_p_itemdbcheck(nullptr)
	, m_p_commlog(nullptr)
	, m_p_datarecord(nullptr)
	, m_bIsModeMasterSlave(true)
	, m_bEnableSendCalibrate(true)
	, m_dwTimeSendCalibrate(0)
{
	// TODO: 在此处添加构造代码

}

CEcarbView::~CEcarbView()
{
	for (CBasicWnd * p : m_list_p_basicwnd)
		delete p;
	delete m_p_itemdatabase;
	delete m_p_comport;
	delete m_p_itemdbcheck;
	delete m_p_commlog;
	delete m_p_datarecord;
}

void CEcarbView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_tabPage);
}

BOOL CEcarbView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CEcarbView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	InitTab();
}


// CEcarbView 打印


void CEcarbView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEcarbView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEcarbView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEcarbView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CEcarbView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 在此处添加自定义打印代码
}

void CEcarbView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEcarbView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	CPoint pt(point);
	m_tabPage.ScreenToClient(&pt);
	CRect rcItem;
	bool bIsInItem = false;
	int nCount = m_tabPage.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		m_tabPage.GetItemRect(i, &rcItem);
		if (rcItem.PtInRect(pt))
		{
			bIsInItem = true;
			break;
		}
	}
	if (bIsInItem)
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT_PAGE, point.x, point.y, this, TRUE);
	else
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT_WND, point.x, point.y, this, TRUE);
#endif
}


// CEcarbView 诊断

#ifdef _DEBUG
void CEcarbView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEcarbView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEcarbDoc* CEcarbView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEcarbDoc)));
	return (CEcarbDoc*)m_pDocument;
}
#endif //_DEBUG


// CEcarbView 消息处理程序


afx_msg LRESULT CEcarbView::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_CLOSEAPP:
	{
		if (g_bIsCommunicating)
		{
			StopAutoReport();
			StopComm();
		}
	}
	break;
	case MSGUSER_SENDRESULT:
	{
		if (m_bIsModeMasterSlave)
		{
			g_zdp.WaitSendComplete();
			if (g_bIsCommunicating&&wParam != ZDevicePort::ERROR_SEND_STOP)
				g_zdp.Recv(&g_modbus, this);
		}
	}
	break;
	case MSGUSER_RECVRESULT:
	{
		g_zdp.WaitRecvComplete();
		if (g_bIsCommunicating&&wParam != ZDevicePort::ERROR_RECV_STOP)
			StartComm();
	}
	break;
	case MSGUSER_CLOSEBASICWND:
	{
		CBasicWnd * p = (CBasicWnd *)wParam;
		CWndInfo & wndinfo = *(p->GetWndInfo());
		if (g_wndinfodb.DeleteFull(wndinfo))//删除wndinfo数据库
		{
			m_list_p_basicwnd.remove(p);//删除basicwnd内存
			p->DestroyWindow();//删除basicwnd界面
			delete p;
			for (CPage & page : g_list_page)
			{
				if (page.GetID() == wndinfo.GetPageID())
				{
					page.GetWndInfo().remove(wndinfo);//删除wndinfo内存
					break;
				}
			}
			
		}
	}
	break;
	case MSGUSER_UPDATEDATAITEM:
	case MSGUSER_DELETEDATAITEM:
	{
		for(CBasicWnd * p: m_list_p_basicwnd)
			p->PostMessage(WM_MSGRECVPRO, wParam,lParam);
	}
	break;
	}
	return 0;
}


void CEcarbView::InitTab()
{
	// TODO: 在此处添加实现代码.
	g_pagedb.SelectFull(_T("SELECT * FROM Page ORDER BY ID"), g_list_page);//从page数据库获取page内存
	int nSize = g_list_page.size();
	auto itPage = g_list_page.begin();
	for (int i = 0; i < nSize; ++i)
	{
		m_tabPage.InsertItem(i, itPage->GetName());//插入page界面
		for (CWndInfo & wndinfo : itPage->GetWndInfo())
		{
			CBasicWnd * p = new CBasicWnd(&wndinfo);
			p->Create(IDD_BASICWND, this);//插入basicwnd界面
			m_list_p_basicwnd.push_back(p);//插入basicwnd内存
		}
		++itPage;
	}
	int nCount = m_tabPage.GetItemCount();
	if (nCount)
	{
		m_tabPage.SetCurSel(0);
		for (CBasicWnd * p : m_list_p_basicwnd)
		{
			if (p->GetPageID() == g_list_page.front().GetID())
				p->ShowWindow(SW_SHOW);
		}
	}
}


void CEcarbView::OnSetComport()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_comport)
	{
		m_p_comport = new CComPort();
		m_p_comport->Create(IDD_COMPORT, this);
	}
	m_p_comport->ShowWindow(SW_SHOW);
}


void CEcarbView::OnSetItemdb()
{
	// TODO: 在此添加命令处理程序代码
	if (!CPasswordInput::CheckPassword())
		return;
	if (!m_p_itemdatabase)
	{
		m_p_itemdatabase = new CItemDatabase();
		m_p_itemdatabase->Create(IDD_ITEMDATABASE, this);
	}
	m_p_itemdatabase->ShowWindow(SW_SHOW);
}


void CEcarbView::OnEditAddpage()
{
	// TODO: 在此添加命令处理程序代码
	if (!CPasswordInput::CheckPassword())
		return;
	CString strTitle(_T("Input the name of page")), strName(_T("unnamed"));
	CNameInput nameinput;
	nameinput.SetParam(strTitle, strName);
	if (nameinput.DoModal() == IDOK)
	{
		nameinput.GetParam(strTitle, strName);
		std::list<CWndInfo> list_wndinfo;
		CPage page(CPageDB::GetNextInsertID(), strName, list_wndinfo);
		if (g_pagedb.InsertFull(page))//插入page数据库
		{
			g_list_page.push_back(page);//插入page内存
			int nIndex = g_list_page.size() - 1;
			m_tabPage.InsertItem(nIndex, page.GetName());//插入page界面
		}
	}
}


void CEcarbView::OnEditDeletepage()
{
	// TODO: 在此添加命令处理程序代码
	if (!CPasswordInput::CheckPassword())
		return;
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		if (g_pagedb.DeleteFull((*itPage)))//删除page数据库
		{
			for (auto it= m_list_p_basicwnd.begin();it!= m_list_p_basicwnd.end();)
			{
				if ((*it)->GetPageID() == itPage->GetID())
				{
					(*it)->DestroyWindow();//删除basicwnd界面
					delete (*it);
					it = m_list_p_basicwnd.erase(it);//删除basicwnd内存
				}
				else
					++it;
			}
			g_list_page.erase(itPage);//删除page内存
			m_tabPage.DeleteItem(nSel);//删除page界面
		}
	}
}


void CEcarbView::OnEditAddcalibratewnd()
{
	// TODO: 在此添加命令处理程序代码
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		AddWnd(CWndInfo::TYPE_CALIBRATE,*itPage);
	}
	else
		AfxMessageBox(_T("Please select one page first"));
}


void CEcarbView::OnEditAddmonitorwnd()
{
	// TODO: 在此添加命令处理程序代码
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		AddWnd(CWndInfo::TYPE_MONITOR, *itPage);
	}
	else
		AfxMessageBox(_T("Please select one page first"));
}


void CEcarbView::OnEditAdddashboardwnd()
{
	// TODO: 在此添加命令处理程序代码
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		AddWnd(CWndInfo::TYPE_DASHBOARD, *itPage);
	}
	else
		AfxMessageBox(_T("Please select one page first"));
}


void CEcarbView::OnEditAddwavewnd()
{
	// TODO: 在此添加命令处理程序代码
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		AddWnd(CWndInfo::TYPE_WAVE, *itPage);
	}
	else
		AfxMessageBox(_T("Please select one page first"));
}


void CEcarbView::OnEditAddstatusledwnd()
{
	// TODO: 在此添加命令处理程序代码
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		AddWnd(CWndInfo::TYPE_STATUSLED, *itPage);
	}
	else
		AfxMessageBox(_T("Please select one page first"));
}


void CEcarbView::OnCheckItemdb()
{
	// TODO: 在此添加命令处理程序代码
	if (!CPasswordInput::CheckPassword())
		return;
	if (!m_p_itemdbcheck)
	{
		m_p_itemdbcheck = new CItemDBCheck();
		m_p_itemdbcheck->Create(IDD_ITEMDBCHECK, this);
	}
	m_p_itemdbcheck->ShowWindow(SW_SHOW);
}


void CEcarbView::OnCheckCommlog()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_commlog)
	{
		m_p_commlog = new CCommLog();
		m_p_commlog->Create(IDD_COMMLOG, this);
	}
	m_p_commlog->ShowWindow(SW_SHOW);
}


void CEcarbView::OnCheckDatarecord()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_p_datarecord)
	{
		m_p_datarecord = new CDataRecord();
		m_p_datarecord->Create(IDD_DATARECORD, this);
	}
	m_p_datarecord->ShowWindow(SW_SHOW);
	m_p_datarecord->Refresh();
}


void CEcarbView::OnCommStart()
{
	// TODO: 在此添加命令处理程序代码
	if (g_nTotalRecords != g_nSavedRecords)
	{
		int nRtn=MessageBox(_T("Records are saving,do you still want to continue the communication"), nullptr, MB_OK| MB_OKCANCEL);
		if (nRtn == IDOK)
		{
			for (CBasicWnd * p : m_list_p_basicwnd)
			{
				p->SendMessage(WM_MSGRECVPRO, 0, MSGUSER_ABANDONSAVERECORD);
			}
		}
		else
			return;
	}
	if (g_zdp.GetPort().IsEmpty())
	{
		AfxMessageBox(_T("The serial port has not been opened"));
		return;
	}
	if (!InitComm())
	{
		AfxMessageBox(_T("There is no monitor item"));
		return;
	}
	if (!StartAutoReport())
	{
		AfxMessageBox(_T("There are too many monitor items under auto report mode"));
		return;
	}
	StartComm();
	if(m_p_itemdatabase)
		m_p_itemdatabase->ShowWindow(SW_HIDE);
	for (CBasicWnd * p : m_list_p_basicwnd)
	{
		p->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_COMMSTART);
		CMenu * p_menu = p->GetSystemMenu(FALSE);
		if (p_menu)
			p_menu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
	}
	g_bIsCommunicating = true;
}


void CEcarbView::OnCommStop()
{
	// TODO: 在此添加命令处理程序代码
	StopAutoReport();
	StopComm();
	g_nTotalRecords = g_nSavedRecords = 0;
	for (CBasicWnd * p : m_list_p_basicwnd)
	{
		p->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_COMMSTOP);
		CMenu * p_menu = p->GetSystemMenu(FALSE);
		if (p_menu)
			p_menu->EnableMenuItem(SC_CLOSE, MF_ENABLED);
	}
	g_bIsCommunicating = false;
}


void CEcarbView::OnCommModemasterslave()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsModeMasterSlave = true;
}


void CEcarbView::OnCommModeautoreport()
{
	// TODO: 在此添加命令处理程序代码
	m_bIsModeMasterSlave = false;
}


void CEcarbView::OnUpdateSetItemdb(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAddpage(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditDeletepage(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAddcalibratewnd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAddmonitorwnd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAdddashboardwnd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAddwavewnd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateEditAddstatusledwnd(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateCommStart(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateCommStop(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (g_bIsCommunicating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


void CEcarbView::OnUpdateCommModemasterslave(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_bIsModeMasterSlave)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnUpdateCommModeautoreport(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (m_bIsModeMasterSlave)
		pCmdUI->SetCheck(FALSE);
	else
		pCmdUI->SetCheck(TRUE);
	if (g_bIsCommunicating)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CEcarbView::OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nSel, nIndex;
	nSel = nIndex = m_tabPage.GetCurSel();
	if (nSel != -1)
	{
		auto itPage = g_list_page.begin();
		while (nIndex--)
			++itPage;
		for (CBasicWnd * p : m_list_p_basicwnd)
		{
			if (p->GetPageID() == itPage->GetID())
				p->ShowWindow(SW_SHOW);
			else
				p->ShowWindow(SW_HIDE);
		}
	}
}


void CEcarbView::AddWnd(int nWndType,CPage & page)
{
	if (!CPasswordInput::CheckPassword())
		return;
	CString strTitle, strName(_T("unnamed"));
	switch (nWndType)
	{
	case CWndInfo::TYPE_CALIBRATE:strTitle=_T("Input the name of calibrate window"); break;
	case CWndInfo::TYPE_MONITOR:strTitle = _T("Input the name of monitor window"); break;
	case CWndInfo::TYPE_DASHBOARD:strTitle = _T("Input the name of dashboard window"); break;
	case CWndInfo::TYPE_WAVE:strTitle = _T("Input the name of wave window"); break;
	case CWndInfo::TYPE_STATUSLED:strTitle = _T("Input the name of status LED window"); break;
	}
	CNameInput nameinput;
	nameinput.SetParam(strTitle, strName);
	if (nameinput.DoModal() == IDOK)
	{
		nameinput.GetParam(strTitle, strName);
		CRect rc;
		GetWindowRect(rc);
		std::list<CWndItem> list_wnditem;
		CWndInfo wndinfo(CWndInfoDB::GetNextInsertID(), strName, nWndType,rc.left,rc.top,rc.left+400,rc.top+200, page.GetID(), list_wnditem);
		if (g_wndinfodb.InsertFull(wndinfo))//插入wndinfo数据库
		{
			page.GetWndInfo().push_back(wndinfo);//插入wndinfo内存
			CBasicWnd * p = new CBasicWnd(&(page.GetWndInfo().back()));
			p->Create(IDD_BASICWND, this);//插入basicwnd界面
			p->ShowWindow(SW_SHOW);
			m_list_p_basicwnd.push_back(p);//插入basicwnd内存
		}
	}
}


bool CEcarbView::InitComm()
{
	g_vec_p_ciCalibrate.clear();
	g_vec_p_ciMonitor.clear();
	CString strRegStartAddr;
	int nRegNum = 0;
	for (CBasicWnd * p : m_list_p_basicwnd)
	{
		for (CCommItem & commitem : p->GetCommItem())
		{
			commitem.m_bIsWaitRead = false;
			commitem.m_bIsWaitWrite = false;
			if (commitem.m_p_dataitem->GetItemType() == CDataItem::TYPE_CALIBRATE)
				g_vec_p_ciCalibrate.push_back(&commitem);
			else
			{
				g_vec_p_ciMonitor.push_back(&commitem);
				strRegStartAddr += commitem.m_p_dataitem->GetRegStartAddr();
				++nRegNum;
			}
		}
	}
	g_it_p_ciMonitor = g_vec_p_ciMonitor.begin();
	m_dataitemAutoReport.SetRegNum(nRegNum);
	m_dataitemAutoReport.SetRegStartAddr(strRegStartAddr);
	m_commitemAutoReport = CCommItem(nullptr, &m_dataitemAutoReport, 0);
	return !g_vec_p_ciMonitor.empty();
}


bool CEcarbView::StartAutoReport()
{
	if (!m_bIsModeMasterSlave)
	{
		CCommItem & commitem = m_commitemAutoReport;
		if (commitem.m_p_dataitem->GetRegStartAddr().GetLength() > 124 * 2)
			return false;
		m_bEnableSendCalibrate = true;
		CString strRegNum;
		strRegNum.Format(_T("%02X"), commitem.m_p_dataitem->GetRegNum());
		g_modbus.Init(_T("FF"), _T("41"), _T("01"), commitem.m_p_dataitem->GetRegStartAddr(), strRegNum, _T(""));
		g_modbus.DataEncode(commitem.m_strSendData);
		m_archeck.SetLength(commitem.m_strSendData.GetLength());
		m_strAutoReportRemainData.Empty();
		g_zdp.PurgePort();
		g_zdp.Send(commitem.m_strSendData, this);
	}
	return true;
}


void CEcarbView::StopAutoReport()
{
	if (!m_bIsModeMasterSlave)
	{
		CCommItem & commitem = m_commitemAutoReport;
		CString strRegNum;
		strRegNum.Format(_T("%02X"), commitem.m_p_dataitem->GetRegNum());
		g_modbus.Init(_T("FF"), _T("41"), _T("02"), commitem.m_p_dataitem->GetRegStartAddr(), strRegNum, _T(""));
		g_modbus.DataEncode(commitem.m_strSendData);
		g_zdp.StopSend();
		g_zdp.Send(commitem.m_strSendData, this);
		g_zdp.WaitSendComplete();
	}
}


std::vector<CCommItem *>::iterator CEcarbView::GetNextCalibrateCommItem()
{
	auto it = g_vec_p_ciCalibrate.begin();
	while (it != g_vec_p_ciCalibrate.end())
	{
		if ((*it)->m_bIsWaitWrite)
			break;
		else if ((*it)->m_bIsWaitRead)
			break;
		else
			++it;
	}
	return it;
}


std::vector<CCommItem *>::iterator CEcarbView::GetNextMonitorCommItem()
{
	auto it = g_it_p_ciMonitor;
	++g_it_p_ciMonitor;
	if (g_it_p_ciMonitor == g_vec_p_ciMonitor.end())
		g_it_p_ciMonitor = g_vec_p_ciMonitor.begin();
	return it;
}


void CEcarbView::StartComm()
{
	if (m_bIsModeMasterSlave)
	{
		g_it_p_commitem = GetNextCalibrateCommItem();
		if (g_it_p_commitem == g_vec_p_ciCalibrate.end())
			g_it_p_commitem = GetNextMonitorCommItem();
		CCommItem & commitem = *(*g_it_p_commitem);
		CString strAutoEnable, strRegStartAddr, strRegNum, strTgtData;
		int nTgtData;
		strRegStartAddr = commitem.m_p_dataitem->GetRegStartAddr();
		strRegNum.Format(_T("%04X"), commitem.m_p_dataitem->GetRegNum());
		if (commitem.m_p_dataitem->GetMultiplier())
			nTgtData = int((commitem.m_p_dataitem->GetTgtData() - commitem.m_p_dataitem->GetConstant()) / commitem.m_p_dataitem->GetMultiplier());
		strTgtData.Format(_T("%04X"), nTgtData);
		switch (commitem.m_p_dataitem->GetItemType())
		{
		case CDataItem::TYPE_CALIBRATE:
		{
			if (commitem.m_bIsWaitWrite)
			{
				g_modbus.Init(_T("FF"), _T("06"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
				commitem.m_bIsWaitWrite = false;
			}
			else if (commitem.m_bIsWaitRead)
			{
				g_modbus.Init(_T("FF"), _T("03"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
				commitem.m_bIsWaitRead = false;
			}
		}
		break;
		case CDataItem::TYPE_MONITOR:
		{
			g_modbus.Init(_T("FF"), _T("04"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
		}
		break;
		}
		g_modbus.DataEncode(commitem.m_strSendData);
		g_zdp.PurgePort();
		g_zdp.Send(commitem.m_strSendData, this);
	}
	else
	{
		if (m_bEnableSendCalibrate)
		{
			g_it_p_commitem = GetNextCalibrateCommItem();
			if (g_it_p_commitem != g_vec_p_ciCalibrate.end())
			{
				CCommItem & commitem = *(*g_it_p_commitem);
				CString strAutoEnable, strRegStartAddr, strRegNum, strTgtData;
				int nTgtData;
				strRegStartAddr = commitem.m_p_dataitem->GetRegStartAddr();
				strRegNum.Format(_T("%04X"), commitem.m_p_dataitem->GetRegNum());
				if (commitem.m_p_dataitem->GetMultiplier())
					nTgtData = int((commitem.m_p_dataitem->GetTgtData() - commitem.m_p_dataitem->GetConstant()) / commitem.m_p_dataitem->GetMultiplier());
				strTgtData.Format(_T("%04X"), nTgtData);
				switch (commitem.m_p_dataitem->GetItemType())
				{
				case CDataItem::TYPE_CALIBRATE:
				{
					if (commitem.m_bIsWaitWrite)
					{
						g_modbus.Init(_T("FF"), _T("06"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
						commitem.m_bIsWaitWrite = false;
					}
					else if (commitem.m_bIsWaitRead)
					{
						g_modbus.Init(_T("FF"), _T("03"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
						commitem.m_bIsWaitRead = false;
					}
				}
				break;
				case CDataItem::TYPE_MONITOR:
				{
					g_modbus.Init(_T("FF"), _T("04"), strAutoEnable, strRegStartAddr, strRegNum, strTgtData);
				}
				break;
				}
				g_modbus.DataEncode(commitem.m_strSendData);
				g_zdp.WaitSendComplete();
				g_zdp.Send(commitem.m_strSendData, this);
				m_dwTimeSendCalibrate = DWORD(HighResolutionTimeCounter::NowOfMillisecond());
				m_bEnableSendCalibrate = false;
			}
		}
		g_zdp.Recv(&m_archeck, this);
	}
}


void CEcarbView::StopComm()
{
	g_zdp.StopComm();
}


void CEcarbView::OnSendResult(int nResult, CString strSendData)
{
	if (m_bIsModeMasterSlave)
	{
		CCommItem & commitem = *(*g_it_p_commitem);
		if (nResult == ZDevicePort::ERROR_OK)
		{
			commitem.m_strSendData = strSendData;
			if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
				m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&commitem.m_strSendData), MSGUSER_SENDSUCCEED);
			if (commitem.m_p_wnd->GetSafeHwnd())
				commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_SENDSUCCEED);
		}
		else
		{
			if (commitem.m_p_wnd->GetSafeHwnd())
				commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_SENDFAIL);
		}
	}
	else
	{
		std::vector<CString> vec_strFixedData, vec_strValue;
		if (g_modbus.DataDecode(strSendData, vec_strFixedData, vec_strValue) == ZModbus::ERROR_OK)
		{
			if (vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("03") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("04") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("06"))
			{
				CCommItem & commitem = *(*g_it_p_commitem);
				commitem.m_strSendData = strSendData;
				if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
					m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&commitem.m_strSendData), MSGUSER_SENDSUCCEED);
			}
			else if (vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("41"))
			{
				CCommItem & commitem = m_commitemAutoReport;
				commitem.m_strSendData = strSendData;
				if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
					m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&commitem.m_strSendData), MSGUSER_SENDSUCCEED);
			}
		}
	}
	PostMessage(WM_MSGRECVPRO, (WPARAM)nResult, MSGUSER_SENDRESULT);
}


void CEcarbView::OnRecvResult(int nResult, CString strRecvData)
{
	if (m_bIsModeMasterSlave)
	{
		CCommItem & commitem = *(*g_it_p_commitem);
		if (nResult == ZDevicePort::ERROR_OK)
		{
			commitem.m_strRecvData = strRecvData;
			if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
				m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&commitem.m_strRecvData), MSGUSER_RECVSUCCEED);
			std::vector<CString> vec_strFixedData, vec_strValue;
			if (g_modbus.DataDecode(commitem.m_strRecvData, vec_strFixedData, vec_strValue) == ZModbus::ERROR_OK)
			{
				if (vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("03") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("04") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("06"))
				{
					double dTgtData = _ttoi(vec_strValue[0])*commitem.m_p_dataitem->GetMultiplier() + commitem.m_p_dataitem->GetConstant();
					DWORD dwTime = DWORD(HighResolutionTimeCounter::NowOfMillisecond());
					commitem.m_dTgtData = dTgtData;
					commitem.m_dwTgtTime = dwTime;
					if (commitem.m_bIsRecording)
					{
						commitem.m_vec_dTgtData.push_back(dTgtData);
						commitem.m_vec_dwTgtTime.push_back(dwTime);
					}
					if (commitem.m_p_wnd->GetSafeHwnd())
						commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVSUCCEED);
				}
			}
		}
		else
		{
			if (commitem.m_p_wnd->GetSafeHwnd())
				commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVFAIL);
		}
	}
	else
	{
		if (!m_bEnableSendCalibrate && (HighResolutionTimeCounter::NowOfMillisecond() - m_dwTimeSendCalibrate) > g_zdp.GetTimeOutRecv())
		{
			CCommItem & commitem = *(*g_it_p_commitem);
			if (commitem.m_p_wnd->GetSafeHwnd())
				commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVFAIL);
			m_bEnableSendCalibrate = true;
		}
		if (nResult == ZDevicePort::ERROR_OK)
		{
			CString strDataFrame;
			m_strAutoReportRemainData += strRecvData;
			while(DivideFirstFrame(m_strAutoReportRemainData,strDataFrame))
			{
				std::vector<CString> vec_strFixedData, vec_strValue;
				if (g_modbus.DataDecode(strDataFrame, vec_strFixedData, vec_strValue) == ZModbus::ERROR_OK)
				{
					if (vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("03") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("04") || vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("06"))
					{
						CCommItem & commitem = *(*g_it_p_commitem);
						commitem.m_strRecvData = strDataFrame;
						if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
							m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&commitem.m_strRecvData), MSGUSER_RECVSUCCEED);
						double dTgtData = _ttoi(vec_strValue[0])*commitem.m_p_dataitem->GetMultiplier() + commitem.m_p_dataitem->GetConstant();
						DWORD dwTime = DWORD(HighResolutionTimeCounter::NowOfMillisecond());
						commitem.m_dTgtData = dTgtData;
						commitem.m_dwTgtTime = dwTime;
						if (commitem.m_bIsRecording)
						{
							commitem.m_vec_dTgtData.push_back(dTgtData);
							commitem.m_vec_dwTgtTime.push_back(dwTime);
						}
						if (commitem.m_p_wnd->GetSafeHwnd())
							commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVSUCCEED);
						m_bEnableSendCalibrate = true;
					}
					else if (vec_strFixedData[ZModbus::INDEX_FUNCCODE] == _T("41"))
					{
						m_commitemAutoReport.m_strRecvData = strDataFrame;
						if (g_bIsLogChecking&&m_p_commlog->GetSafeHwnd())
							m_p_commlog->PostMessage(WM_MSGRECVPRO, (WPARAM)(&m_commitemAutoReport.m_strRecvData), MSGUSER_RECVSUCCEED);
						int i = 0;
						for (CCommItem * p_commitem : g_vec_p_ciMonitor)
						{
							CCommItem & commitem = *p_commitem;
							double dTgtData = _ttoi(vec_strValue[i])*commitem.m_p_dataitem->GetMultiplier() + commitem.m_p_dataitem->GetConstant();
							DWORD dwTime = DWORD(HighResolutionTimeCounter::NowOfMillisecond());
							commitem.m_dTgtData = dTgtData;
							commitem.m_dwTgtTime = dwTime;
							if (commitem.m_bIsRecording)
							{
								commitem.m_vec_dTgtData.push_back(dTgtData);
								commitem.m_vec_dwTgtTime.push_back(dwTime);
							}
							if (commitem.m_p_wnd->GetSafeHwnd())
								commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVSUCCEED);
							++i;
						}
					}
				}
			}
		}
		else
		{
			for (CCommItem * p_commitem : g_vec_p_ciMonitor)
			{
				CCommItem & commitem = *p_commitem;
				if (commitem.m_p_wnd->GetSafeHwnd())
					commitem.m_p_wnd->PostMessage(WM_MSGRECVPRO, (WPARAM)commitem.m_nDataItemIndex, MSGUSER_RECVFAIL);
			}
		}
	}
	PostMessage(WM_MSGRECVPRO, (WPARAM)nResult, MSGUSER_RECVRESULT);
}


bool CEcarbView::DivideFirstFrame(CString & strData, CString & strDataFrame)
{
	int nLength = strData.GetLength();
	for (int i = 0; i < nLength; ++i)
	{
		CString str = strData.Left(i+1);
		if (g_modbus.CheckProtocol(str))
		{
			strDataFrame = str;
			strData.Delete(0, i+1);
			return true;
		}
	}
	if (nLength >= m_commitemAutoReport.m_strSendData.GetLength() * 2)
	{
		int nIndexFirst=strData.Find(_T("FF"));
		if (nIndexFirst != -1)
		{
			int nIndexSecond = strData.Find(_T("FF"), nIndexFirst + 1);
			if (nIndexSecond != -1)
			{
				strData.Delete(0, nIndexSecond);
			}
		}
	}
	return false;
}