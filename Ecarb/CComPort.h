#pragma once


// CComPort 对话框

class CComPort : public CDialogEx
{
	DECLARE_DYNAMIC(CComPort)

public:
	CComPort(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CComPort();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboComPort;
	CComboBox m_comboBaudRate;
	CComboBox m_comboParity;
	CComboBox m_comboByteSize;
	CComboBox m_comboStopBits;
public:
	virtual BOOL OnInitDialog();
	void InitUI();
	afx_msg void OnBnClickedButtonComport();
};
