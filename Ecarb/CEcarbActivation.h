#pragma once


// CEcarbActivation 对话框

class CEcarbActivation : public CDialogEx
{
	DECLARE_DYNAMIC(CEcarbActivation)

public:
	CEcarbActivation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEcarbActivation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ECARBACTIVATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_strSerialNumber;
	CString m_strActivationFile;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedOk();
};
