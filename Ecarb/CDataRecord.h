#pragma once


// CDataRecord 对话框

class CDataRecord : public CDialogEx,public ZListCtrl::ModifyCallBack
{
	DECLARE_DYNAMIC(CDataRecord)

public:
	CDataRecord(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDataRecord();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATARECORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CTreeCtrl m_treeRecord;
	ZListCtrl m_listRecord;
	CRecordStatic m_staticRecord;
	std::vector<CRecord> m_vec_record;
	std::vector<CRecordItem> m_vec_rditem;
	std::vector<Gdiplus::Color> m_vec_crItem;
	std::vector<Gdiplus::REAL> m_vec_fWidth;
	std::thread m_threadDelete;
	bool m_bIsDeleting;
public:
	virtual BOOL OnInitDialog();
	void Refresh();
	void InitData();
	void InitTree();
	void InitList();
	void InsertList(const std::vector<CRecordItem> & vec_rditem, const std::vector<Gdiplus::Color> & vec_crItem, const std::vector<Gdiplus::REAL> & vec_fWidth);
	void ExpandTree(CTreeCtrl & tree, HTREEITEM hTreeItem);
	afx_msg void OnTvnSelchangedTreeRecord(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew);
	afx_msg void OnNMDblclkListRecord(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnOperateRefreshrecord();
	afx_msg void OnOperateDeleterecord();
	afx_msg void OnUpdateOperateDeleterecord(CCmdUI *pCmdUI);
	void DeleteDBRecord(CRecord & record);
	void DeleteDBRecordItem(CRecordItem & rditem);
};
