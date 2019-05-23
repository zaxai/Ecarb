// CItemDBCheck.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CItemDBCheck.h"
#include "afxdialogex.h"


// CItemDBCheck 对话框

IMPLEMENT_DYNAMIC(CItemDBCheck, CDialogEx)

CItemDBCheck::CItemDBCheck(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITEMDBCHECK, pParent)
{

}

CItemDBCheck::~CItemDBCheck()
{
}

void CItemDBCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
	DDX_Control(pDX, IDC_COMBO_FIRST, m_comboFirst);
	DDX_Control(pDX, IDC_COMBO_SECOND, m_comboSecond);
}


BEGIN_MESSAGE_MAP(CItemDBCheck, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_ASCENDFIRST, &CItemDBCheck::OnBnClickedRadioAscendfirst)
	ON_BN_CLICKED(IDC_RADIO_DESCENDFIRST, &CItemDBCheck::OnBnClickedRadioDescendfirst)
	ON_BN_CLICKED(IDC_RADIO_ASCENDSECOND, &CItemDBCheck::OnBnClickedRadioAscendsecond)
	ON_BN_CLICKED(IDC_RADIO_DESCENDSECOND, &CItemDBCheck::OnBnClickedRadioDescendsecond)
	ON_BN_CLICKED(IDC_BUTTON_SORT, &CItemDBCheck::OnBnClickedButtonSort)
END_MESSAGE_MAP()


// CItemDBCheck 消息处理程序


BOOL CItemDBCheck::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	InitCombo();
	m_strOrderFirst = _T("ASC");
	m_strOrderSecond = _T("ASC");
	CheckDlgButton(IDC_RADIO_ASCENDFIRST, BST_CHECKED);
	CheckDlgButton(IDC_RADIO_ASCENDSECOND, BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CItemDBCheck::InitList()
{
	int nMax = 14;
	CRect rc;
	m_listItem.GetClientRect(rc);
	m_listItem.SetExtendedStyle(m_listItem.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listItem.InsertColumn(0, _T("Item ID"), LVCFMT_CENTER, rc.Width() / nMax, 0);
	m_listItem.InsertColumn(1, _T("Item type"), LVCFMT_CENTER, rc.Width() / nMax, 1);
	m_listItem.InsertColumn(2, _T("Name"), LVCFMT_CENTER, rc.Width() / nMax, 2);
	m_listItem.InsertColumn(3, _T("Unit"), LVCFMT_CENTER, rc.Width() / nMax, 3);
	m_listItem.InsertColumn(4, _T("Start address(Hex)"), LVCFMT_CENTER, rc.Width() * 2 / nMax, 4);
	m_listItem.InsertColumn(5, _T("Register count"), LVCFMT_CENTER, rc.Width() / nMax, 5);
	m_listItem.InsertColumn(6, _T("Decimal digits"), LVCFMT_CENTER, rc.Width() / nMax, 6);
	m_listItem.InsertColumn(7, _T("Multiplier"), LVCFMT_CENTER, rc.Width() / nMax, 7);
	m_listItem.InsertColumn(8, _T("Constant"), LVCFMT_CENTER, rc.Width() / nMax, 8);
	m_listItem.InsertColumn(9, _T("Initial data"), LVCFMT_CENTER, rc.Width() / nMax, 9);
	m_listItem.InsertColumn(10, _T("Max data"), LVCFMT_CENTER, rc.Width() / nMax, 10);
	m_listItem.InsertColumn(11, _T("Min data"), LVCFMT_CENTER, rc.Width() / nMax, 11);
	std::vector<CDataItem> vec_dataitem;
	g_dataitemdb.Select(_T("SELECT * FROM DataItem ORDER BY ID"), vec_dataitem);
	for (const CDataItem & dataitem : vec_dataitem)
		InsertList(dataitem);
}


void CItemDBCheck::InitCombo()
{
	CString sz_strItem[] = { _T("Item ID"), _T("Item type"), _T("Name"),_T("Unit"), _T("Start address"), _T("Register count"),_T("Decimal digits"),_T("Multiplier"),_T("Constant"), _T("Initial data"), _T("Max data"), _T("Min data") };
	for (const CString & strItem : sz_strItem)
	{
		m_comboFirst.AddString(strItem);
		m_comboSecond.AddString(strItem);
	}
	m_comboFirst.SetCurSel(0);
	m_comboSecond.SetCurSel(0);
}


void CItemDBCheck::InsertList(const CDataItem & dataitem)
{
	int nIndex = m_listItem.GetItemCount();
	CString strText[12];
	strText[0].Format(_T("%d"), dataitem.GetID());
	switch (dataitem.GetItemType())
	{
	case CDataItem::TYPE_CALIBRATE:strText[1] = _T("0-Calibrate"); break;
	case CDataItem::TYPE_MONITOR:strText[1] = _T("1-Monitor"); break;
	}
	strText[2] = dataitem.GetName();
	strText[3] = dataitem.GetUnit();
	strText[4] = dataitem.GetRegStartAddr();
	strText[5].Format(_T("%d"), dataitem.GetRegNum());
	strText[6].Format(_T("%d"), dataitem.GetDecimalDigits());
	strText[7] = ZUtil::GetDoubleString(dataitem.GetMultiplier(), dataitem.GetDecimalDigits());
	strText[8] = ZUtil::GetDoubleString(dataitem.GetConstant(), dataitem.GetDecimalDigits());
	strText[9] = ZUtil::GetDoubleString(dataitem.GetTgtData(), dataitem.GetDecimalDigits());
	strText[10] = ZUtil::GetDoubleString(dataitem.GetMaxData(), dataitem.GetDecimalDigits());
	strText[11] = ZUtil::GetDoubleString(dataitem.GetMinData(), dataitem.GetDecimalDigits());
	m_listItem.InsertItem(nIndex, strText[0]);
	for (int i = 1; i < 12; ++i)
		m_listItem.SetItemText(nIndex, i, strText[i]);
}


void CItemDBCheck::OnBnClickedRadioAscendfirst()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strOrderFirst = _T("ASC");
}


void CItemDBCheck::OnBnClickedRadioDescendfirst()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strOrderFirst = _T("DESC");
}


void CItemDBCheck::OnBnClickedRadioAscendsecond()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strOrderSecond = _T("ASC");
}


void CItemDBCheck::OnBnClickedRadioDescendsecond()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strOrderSecond = _T("DESC");
}


void CItemDBCheck::OnBnClickedButtonSort()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sz_strColumnName[] = { _T("ID"), _T("ItemType"), _T("Name"),_T("Unit"), _T("RegStartAddr"), _T("RegNum"),_T("DecimalDigits"),_T("Multiplier"),_T("Constant"), _T("TgtData"), _T("MaxData"), _T("MinData") };
	int nSelFirst = m_comboFirst.GetCurSel();
	int nSelSecond = m_comboSecond.GetCurSel();
	if (nSelFirst != CB_ERR && nSelSecond != CB_ERR)
	{
		CString strSql, strColumnFirst, strColumnSecond;
		strColumnFirst = sz_strColumnName[nSelFirst];
		strColumnSecond = sz_strColumnName[nSelSecond];
		if (strColumnFirst == strColumnSecond)
		{
			strSql.Format(_T("SELECT * FROM DataItem ORDER BY %s %s"), strColumnFirst, m_strOrderFirst);
		}
		else
		{
			strSql.Format(_T("SELECT * FROM DataItem ORDER BY %s %s,%s %s"), strColumnFirst, m_strOrderFirst, strColumnSecond, m_strOrderSecond);
		}
		std::vector<CDataItem> vec_dataitem;
		g_dataitemdb.Select(strSql, vec_dataitem);
		m_listItem.DeleteAllItems();
		for (const CDataItem & dataitem : vec_dataitem)
			InsertList(dataitem);
	}
}