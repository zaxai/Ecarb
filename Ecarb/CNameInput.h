#pragma once


// CNameInput 对话框

class CNameInput : public CDialogEx
{
	DECLARE_DYNAMIC(CNameInput)

public:
	CNameInput(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNameInput();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAMEINPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strTitle;
	CString m_strName;
public:
	virtual BOOL OnInitDialog();
	void SetParam(const CString & strTitle, const CString & strName);
	void GetParam(CString & strTitle, CString & strName);
	afx_msg void OnBnClickedOk();
};
