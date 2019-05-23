#pragma once


// CItemOperate 对话框

class CItemOperate : public CDialogEx
{
	DECLARE_DYNAMIC(CItemOperate)

public:
	CItemOperate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CItemOperate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEMOPERATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listWndItem;
	CListCtrl m_listDatabaseItem;
	std::list<CDataItem> m_list_itemWnd;
	std::list<CDataItem> m_list_itemDB;
public:
	virtual BOOL OnInitDialog();
	void InitList();
	void SetParam(const std::list<CDataItem> & list_itemWnd,const std::list<CDataItem> & list_itemDB);
	void GetParam(std::list<CDataItem> & list_itemWnd, std::list<CDataItem> & list_itemDB);
	void InsertList(CListCtrl & list, const CDataItem & dataitem);
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonDelete();
};
