#pragma once


// CBasicWnd 对话框
#include "CItemOperate.h"
#define IDC_LIST_DATAITEM 2000
#define IDC_BUTTON_RECORD 2001
#define IDC_BUTTON_READ 2002
#define IDC_BUTTON_WRITE 2003
#define IDC_BUTTON_ZOOMIN 2004
#define IDC_BUTTON_ZOOMOUT 2005
#define IDC_STATIC_START 2006

class CBasicWnd : public CDialogEx, public ZListCtrl::ModifyCallBack
{
	DECLARE_DYNAMIC(CBasicWnd)

public:
	CBasicWnd(CWndInfo * p_wndinfo,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBasicWnd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASICWND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ZListCtrl * m_p_listItem;
	CButton * m_p_btnRecord;
	CButton * m_p_btnRead;
	CButton * m_p_btnWrite;
	CButton * m_p_btnZoomIn;
	CButton * m_p_btnZoomOut;
	std::list<CStatic *> m_list_p_static;
	CWndInfo * m_p_wndinfo;
	bool m_bIsInitFinish;
	bool m_bIsRecording;
	int m_nRecordID;
	DWORD m_dwTimeStart;
	bool m_bIsSavingRecords;
	std::thread m_threadSaveRecord;
	std::list<CDataItem> m_list_dataitem;
	std::list<CCommItem> m_list_commitem;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitData();
	void InitUI();
	void ClearUI();
	void InsertUI(int nIndex, const CDataItem & dataitem);
	void UpdateUI(int nIndex, const CDataItem & dataitem);
	void DeleteUI(int nIndex, const CDataItem & dataitem);
	void InitButtonRecord();
	void InitButtonCalibrate();
	void InitButtonZoom();
	void InitList();
	void InsertList(int nIndex, const CDataItem & dataitem);
	void UpdateList(int nIndex, const CDataItem & dataitem);
	void DeleteList(int nIndex, const CDataItem & dataitem);
	void InitStatic();
	void InsertStatic(int nIndex, const CDataItem & dataitem);
	void UpdateStatic(int nIndex);
	void DeleteStatic(int nIndex, const CDataItem & dataitem);
	void SetWndInfo(CWndInfo * p_wndinfo);
	CWndInfo * GetWndInfo();
	int GetPageID() const;
	std::list<CCommItem> & GetCommItem();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	int GetItemType(int nWndType);
	afx_msg void OnOperateItem();
	afx_msg void OnUpdateOperateItem(CCmdUI *pCmdUI);
	virtual void OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ListResize(bool bIsZoomIn);
	void StaticResize(CRect & rc, int nMaxColumn, double dRatioWidthToHeight);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonZoomin();
	afx_msg void OnBnClickedButtonZoomout();
	void SetRecordID();
	void StartRecord();
	void SaveRecord();
	void StartSave();
	void StopSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
