#pragma once


// CCommLog 对话框

class CCommLog : public CDialogEx
{
	DECLARE_DYNAMIC(CCommLog)

public:
	CCommLog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCommLog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_listLog;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	void InitList();
	afx_msg void OnBnClickedButtonLog();
	afx_msg void OnBnClickedButtonClear();
	void InsertList(const CString strFlag, const CString & strData);
	CString DataFormat(const CString & str);
};
