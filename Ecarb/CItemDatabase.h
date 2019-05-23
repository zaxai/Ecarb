#pragma once


// CItemDatabase 对话框

class CItemDatabase : public CDialogEx,public ZListCtrl::ModifyCallBack,public ZExcelImport,public ZExcelExport
{
	DECLARE_DYNAMIC(CItemDatabase)

public:
	CItemDatabase(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CItemDatabase();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEMDATABASE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ZListCtrl m_listItem;
	std::vector<CDataItem> m_vec_dataitem;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitList();
	afx_msg void OnBnClickedButtonAddcalibrate();
	afx_msg void OnBnClickedButtonAddmonitor();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonImportexcel();
	afx_msg void OnBnClickedButtonExportexcel();
	void InsertList(const CDataItem & dataitem);
	void UpdateList(int nIndex, const CDataItem & dataitem);
	virtual void OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew);
	virtual void OnImportProgressUpdate(int nProgress);
	virtual void OnImportResult(bool bResult);
	virtual void OnExportProgressUpdate(int nProgress);
	virtual void OnExportResult(bool bResult);
	void NotifyUpdateDataItem(int nDataItemID);
	void NotifyDeleteDataItem(int nDataItemID);
};
