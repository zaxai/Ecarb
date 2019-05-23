#pragma once


// CPasswordInput 对话框

class CPasswordInput : public CDialogEx
{
	DECLARE_DYNAMIC(CPasswordInput)

public:
	CPasswordInput(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPasswordInput();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PASSWORDINPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	static bool CheckPassword();
};
