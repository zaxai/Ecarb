#pragma once


// CItemDBCheck 对话框

class CItemDBCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CItemDBCheck)

public:
	CItemDBCheck(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CItemDBCheck();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ITEMDBCHECK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listItem;
	CComboBox m_comboFirst;
	CComboBox m_comboSecond;
	CString m_strOrderFirst;
	CString m_strOrderSecond;
public:
	virtual BOOL OnInitDialog();
	void InitList();
	void InitCombo();
	void InsertList(const CDataItem & dataitem);
	afx_msg void OnBnClickedRadioAscendfirst();
	afx_msg void OnBnClickedRadioDescendfirst();
	afx_msg void OnBnClickedRadioAscendsecond();
	afx_msg void OnBnClickedRadioDescendsecond();
	afx_msg void OnBnClickedButtonSort();
};
