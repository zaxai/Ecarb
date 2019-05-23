
// EcarbView.h: CEcarbView 类的接口
//

#pragma once
#include "CNameInput.h"
#include "CBasicWnd.h"
#include "CItemDatabase.h"
#include "CComPort.h"
#include "CItemDBCheck.h"
#include "CCommLog.h"
#include "CDataRecord.h"

class CEcarbView : public CFormView,public ZDevicePort::CommCallBack
{
protected: // 仅从序列化创建
	CEcarbView() noexcept;
	DECLARE_DYNCREATE(CEcarbView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_ECARB_FORM };
#endif

// 特性
public:
	CEcarbDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CEcarbView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_tabPage;
	std::list<CBasicWnd *> m_list_p_basicwnd;
	CItemDatabase * m_p_itemdatabase;
	CComPort * m_p_comport;
	CItemDBCheck * m_p_itemdbcheck;
	CCommLog * m_p_commlog;
	CDataRecord * m_p_datarecord;
	bool m_bIsModeMasterSlave;
	bool m_bEnableSendCalibrate;
	DWORD m_dwTimeSendCalibrate;
	CDataItem m_dataitemAutoReport;
	CCommItem m_commitemAutoReport;
	CAutoReportCheck m_archeck;
	CString m_strAutoReportRemainData;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	void InitTab();
	afx_msg void OnSetComport();
	afx_msg void OnSetItemdb();
	afx_msg void OnEditAddpage();
	afx_msg void OnEditDeletepage();
	afx_msg void OnEditAddcalibratewnd();
	afx_msg void OnEditAddmonitorwnd();
	afx_msg void OnEditAdddashboardwnd();
	afx_msg void OnEditAddwavewnd();
	afx_msg void OnEditAddstatusledwnd();
	afx_msg void OnCheckItemdb();
	afx_msg void OnCheckCommlog();
	afx_msg void OnCheckDatarecord();
	afx_msg void OnCommStart();
	afx_msg void OnCommStop();
	afx_msg void OnCommModemasterslave();
	afx_msg void OnCommModeautoreport();
	afx_msg void OnUpdateSetItemdb(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddpage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditDeletepage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddcalibratewnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddmonitorwnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAdddashboardwnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddwavewnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditAddstatusledwnd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommModemasterslave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommModeautoreport(CCmdUI *pCmdUI);
	afx_msg void OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult);
	void AddWnd(int nWndType, CPage & page);
	bool InitComm();
	bool StartAutoReport();
	void StopAutoReport();
	std::vector<CCommItem *>::iterator GetNextCalibrateCommItem();
	std::vector<CCommItem *>::iterator GetNextMonitorCommItem();
	void StartComm();
	void StopComm();
	virtual void OnSendResult(int nResult, CString strSendData);
	virtual void OnRecvResult(int nResult, CString strRecvData);
	bool DivideFirstFrame(CString & strRecvData, CString & strDataFrame);
};

#ifndef _DEBUG  // EcarbView.cpp 中的调试版本
inline CEcarbDoc* CEcarbView::GetDocument() const
   { return reinterpret_cast<CEcarbDoc*>(m_pDocument); }
#endif

