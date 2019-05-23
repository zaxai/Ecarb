// CItemDatabase.cpp: 实现文件
//

#include "stdafx.h"
#include "Ecarb.h"
#include "CItemDatabase.h"
#include "afxdialogex.h"


// CItemDatabase 对话框

IMPLEMENT_DYNAMIC(CItemDatabase, CDialogEx)

CItemDatabase::CItemDatabase(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ITEMDATABASE, pParent)
	, m_listItem(this)
{

}

CItemDatabase::~CItemDatabase()
{
}

void CItemDatabase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ITEM, m_listItem);
}


BEGIN_MESSAGE_MAP(CItemDatabase, CDialogEx)
	ON_MESSAGE(WM_MSGRECVPRO, &CItemDatabase::OnMsgrecvpro)
	ON_BN_CLICKED(IDC_BUTTON_ADDCALIBRATE, &CItemDatabase::OnBnClickedButtonAddcalibrate)
	ON_BN_CLICKED(IDC_BUTTON_ADDMONITOR, &CItemDatabase::OnBnClickedButtonAddmonitor)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CItemDatabase::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTEXCEL, &CItemDatabase::OnBnClickedButtonImportexcel)
	ON_BN_CLICKED(IDC_BUTTON_EXPORTEXCEL, &CItemDatabase::OnBnClickedButtonExportexcel)
END_MESSAGE_MAP()


// CItemDatabase 消息处理程序


BOOL CItemDatabase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CItemDatabase::InitList()
{
	int nMax = 14;
	CRect rc;
	m_listItem.GetClientRect(rc);
	m_listItem.SetExtendedStyle(m_listItem.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_listItem.InsertColumn(0, _T("Item ID"), LVCFMT_CENTER, rc.Width() / nMax, 0);
	m_listItem.InsertColumn(1, _T("Item type"), LVCFMT_CENTER, rc.Width() / nMax, 1);
	m_listItem.InsertColumn(2, _T("Name"), LVCFMT_CENTER, rc.Width() / nMax, 2);
	m_listItem.InsertColumn(3, _T("Unit"), LVCFMT_CENTER, rc.Width() / nMax, 3);
	m_listItem.InsertColumn(4, _T("Start address(Hex)"), LVCFMT_CENTER, rc.Width()*2 / nMax, 4);
	m_listItem.InsertColumn(5, _T("Register count"), LVCFMT_CENTER, rc.Width() / nMax, 5);
	m_listItem.InsertColumn(6, _T("Decimal digits"), LVCFMT_CENTER, rc.Width() / nMax, 6);
	m_listItem.InsertColumn(7, _T("Multiplier"), LVCFMT_CENTER, rc.Width() / nMax, 7);
	m_listItem.InsertColumn(8, _T("Constant"), LVCFMT_CENTER, rc.Width() / nMax, 8);
	m_listItem.InsertColumn(9, _T("Initial data"), LVCFMT_CENTER, rc.Width() / nMax, 9);
	m_listItem.InsertColumn(10, _T("Max data"), LVCFMT_CENTER, rc.Width() / nMax, 10);
	m_listItem.InsertColumn(11, _T("Min data"), LVCFMT_CENTER, rc.Width() / nMax, 11);
	m_listItem.EnableEdit(TRUE);
	m_listItem.SetDefaultEnableFlag(TRUE);
	g_dataitemdb.Select(_T("SELECT * FROM DataItem ORDER BY ID"), m_vec_dataitem);
	for (const CDataItem & dataitem : m_vec_dataitem)
		InsertList(dataitem);
}


afx_msg LRESULT CItemDatabase::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_IMPORTPROGRESS:
	{
		static unsigned int nLastProgress = 0;
		if (nLastProgress != wParam)
		{
			CString strPercent;
			strPercent.Format(_T("Import%d%%..."), wParam);
			SetDlgItemText(IDC_BUTTON_IMPORTEXCEL, strPercent);
			nLastProgress = wParam;
		}
	}
	break;
	case MSGUSER_IMPORTRESULT:
	{
		SetDlgItemText(IDC_BUTTON_IMPORTEXCEL, _T("Import Excel"));
		GetDlgItem(IDC_BUTTON_IMPORTEXCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(TRUE);
		if (wParam)
		{
			if (m_vec2_strDataImport.size())
				m_vec2_strDataImport.erase(m_vec2_strDataImport.begin());
			int nRow = m_vec2_strDataImport.size();
			if (nRow)
			{
				int nColumn = m_vec2_strDataImport[0].size();
				if (nColumn == 12)
				{
					for (int i = 0; i < nRow; ++i)
					{
						int nItemType;
						CDataItem dataitem;
						if (m_vec2_strDataImport[i][0].IsEmpty())
							dataitem.SetID(CDataItemDB::GetNextInsertID());
						else
							dataitem.SetID(_ttoi(m_vec2_strDataImport[i][0]));
						nItemType = _ttoi(m_vec2_strDataImport[i][1]);
						switch (nItemType)
						{
						case CDataItem::TYPE_CALIBRATE:break;
						case CDataItem::TYPE_MONITOR:break;
						default:nItemType = CDataItem::TYPE_CALIBRATE; break;
						}
						dataitem.SetItemType(nItemType);
						dataitem.SetName(m_vec2_strDataImport[i][2]);
						dataitem.SetUnit(m_vec2_strDataImport[i][3]);
						dataitem.SetRegStartAddr(m_vec2_strDataImport[i][4]);
						dataitem.SetRegNum(_ttoi(m_vec2_strDataImport[i][5]));
						dataitem.SetDecimalDigits(_ttoi(m_vec2_strDataImport[i][6]));
						dataitem.SetMultiplier(_ttof(m_vec2_strDataImport[i][7]));
						dataitem.SetConstant(_ttof(m_vec2_strDataImport[i][8]));
						dataitem.SetTgtData(_ttof(m_vec2_strDataImport[i][9]));
						dataitem.SetMaxData(_ttof(m_vec2_strDataImport[i][10]));
						dataitem.SetMinData(_ttof(m_vec2_strDataImport[i][11]));
						if (g_dataitemdb.Insert(dataitem))//插入dataitem数据库
						{
							m_vec_dataitem.push_back(dataitem);//插入dataitem内存
							InsertList(dataitem);//插入dataitem界面
						}
						else
						{
							if (g_dataitemdb.Update(dataitem))//更新dataitem数据库
							{
								int nSize = m_vec_dataitem.size();
								for (int i = 0; i < nSize; ++i)
								{
									if (m_vec_dataitem[i] == dataitem)
									{
										m_vec_dataitem[i] = dataitem;//更新dataitem内存
										UpdateList(i,dataitem);//更新dataitem界面
										NotifyUpdateDataItem(dataitem.GetID());
										break;
									}
								}
							}
						}
					}
				}
			}
			AfxMessageBox(_T("Import succeeded"));
		}
		else
			AfxMessageBox(_T("Import failed"));
	}
	break;
	case MSGUSER_EXPORTPROGRESS:
	{
		static unsigned int nLastProgress = 0;
		if (nLastProgress != wParam)
		{
			CString strPercent;
			strPercent.Format(_T("Export%d%%..."), wParam);
			SetDlgItemText(IDC_BUTTON_EXPORTEXCEL, strPercent);
			nLastProgress = wParam;
		}
	}
	break;
	case MSGUSER_EXPORTRESULT:
	{
		SetDlgItemText(IDC_BUTTON_EXPORTEXCEL, _T("Export Excel"));
		GetDlgItem(IDC_BUTTON_IMPORTEXCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(TRUE);
		if (wParam)
			AfxMessageBox(_T("Export succeeded"));
		else
			AfxMessageBox(_T("Export failed"));
	}
	break;
	}
	return 0;
}


void CItemDatabase::OnBnClickedButtonAddcalibrate()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataItem dataitem(CDataItemDB::GetNextInsertID(), CDataItem::TYPE_CALIBRATE, _T(""), _T(""), _T(""), 1, 0, 1.0, 0.0, 0.0, 0.0, 0.0);
	if (g_dataitemdb.Insert(dataitem))//插入dataitem数据库
	{
		m_vec_dataitem.push_back(dataitem);//插入dataitem内存
		InsertList(dataitem);//插入dataitem界面
	}
}


void CItemDatabase::OnBnClickedButtonAddmonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	CDataItem dataitem(CDataItemDB::GetNextInsertID(), CDataItem::TYPE_MONITOR, _T(""), _T(""), _T(""), 1, 0, 1.0, 0.0, 0.0, 0.0, 0.0);
	if (g_dataitemdb.Insert(dataitem))//插入dataitem数据库
	{
		m_vec_dataitem.push_back(dataitem);//插入dataitem内存
		InsertList(dataitem);//插入dataitem界面
	}
}


void CItemDatabase::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSize = m_listItem.GetItemCount();
	for (int i = nSize-1; i >=0;--i)
	{
		if (m_listItem.GetCheck(i))
		{
			if (g_dataitemdb.Delete(m_vec_dataitem[i]))//删除dataitem数据库
			{
				NotifyDeleteDataItem(m_vec_dataitem[i].GetID());
				m_listItem.DeleteItem(i);//删除dataitem界面
				m_vec_dataitem.erase(m_vec_dataitem.begin() + i);//删除dataitem内存
			}
		}
	}
}


void CItemDatabase::OnBnClickedButtonImportexcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter;
	strFilter = _T("Excel files(*.xlsx)|*.xlsx|Excel files(*.xls)|*.xls|All files(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("xlsx"), NULL, 0, strFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		m_strSrcPath = fileDlg.GetPathName();
		if (StartImport())
		{
			GetDlgItem(IDC_BUTTON_IMPORTEXCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(FALSE);
		}
		else
			AfxMessageBox(_T("Import failed"));
	}
}


void CItemDatabase::OnBnClickedButtonExportexcel()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter;
	strFilter = _T("Excel files(*.xlsx)|*.xlsx|Excel files(*.xls)|*.xls|All files(*.*)|*.*||");
	CFileDialog fileDlg(FALSE, _T("xlsx"), _T("Item data"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);
	if (IDOK == fileDlg.DoModal())
	{
		m_strDestPath = fileDlg.GetPathName();
		m_strDemoPath = ZUtil::GetExeCatalogPath() + _T("\\Demo.xlsx");
		m_vec2_strDataExport.clear();
		for (const CDataItem & dataitem : m_vec_dataitem)
		{
			CString str;
			std::vector<CString> vec_strRow;
			str.Format(_T("%d"), dataitem.GetID());
			vec_strRow.push_back(str);
			switch (dataitem.GetItemType())
			{
			case CDataItem::TYPE_CALIBRATE:str = _T("0-Calibrate"); break;
			case CDataItem::TYPE_MONITOR:str = _T("1-Monitor"); break;
			}
			vec_strRow.push_back(str);
			vec_strRow.push_back(dataitem.GetName());
			vec_strRow.push_back(dataitem.GetUnit());
			vec_strRow.push_back(dataitem.GetRegStartAddr());
			str.Format(_T("%d"), dataitem.GetRegNum());
			vec_strRow.push_back(str);
			str.Format(_T("%d"), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			str = ZUtil::GetDoubleString(dataitem.GetMultiplier(), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			str = ZUtil::GetDoubleString(dataitem.GetConstant(), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			str = ZUtil::GetDoubleString(dataitem.GetTgtData(), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			str = ZUtil::GetDoubleString(dataitem.GetMaxData(), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			str = ZUtil::GetDoubleString(dataitem.GetMinData(), dataitem.GetDecimalDigits());
			vec_strRow.push_back(str);
			m_vec2_strDataExport.push_back(vec_strRow);
		}
		ZExcelExport::m_nRowOffset = 1;
		if (StartExport())
		{
			GetDlgItem(IDC_BUTTON_IMPORTEXCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_EXPORTEXCEL)->EnableWindow(FALSE);
		}
		else
			AfxMessageBox(_T("Export failed"));
	}
}


void CItemDatabase::InsertList(const CDataItem & dataitem)
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
	m_listItem.SetItemEnableFlag(nIndex,0, FALSE);
	m_listItem.SetItemEnableFlag(nIndex,1, FALSE);
	m_listItem.SetItemEnableFlag(nIndex,5, FALSE);
}


void CItemDatabase::UpdateList(int nIndex, const CDataItem & dataitem)
{
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
	for (int i = 0; i < 12; ++i)
		m_listItem.SetItemText(nIndex, i, strText[i]);
}


void CItemDatabase::OnModifyItem(const ZListCtrl::ItemInfo & ii, const CString & strOld, const CString & strNew)
{
	CDataItem dataitem(m_vec_dataitem[ii.m_nItem]);
	switch (ii.m_nSubItem)
	{
	//case 0:m_vec_item[ii.m_nItem].SetID(_ttoi(strNew)); break;
	case 1:m_vec_dataitem[ii.m_nItem].SetItemType(_ttoi(strNew)); break;
	case 2:m_vec_dataitem[ii.m_nItem].SetName(strNew); break;
	case 3:m_vec_dataitem[ii.m_nItem].SetUnit(strNew); break;
	case 4:m_vec_dataitem[ii.m_nItem].SetRegStartAddr(strNew); break;
	case 5:m_vec_dataitem[ii.m_nItem].SetRegNum(_ttoi(strNew)); break;
	case 6:m_vec_dataitem[ii.m_nItem].SetDecimalDigits(_ttoi(strNew)); break;
	case 7:m_vec_dataitem[ii.m_nItem].SetMultiplier(_ttof(strNew)); break;
	case 8:m_vec_dataitem[ii.m_nItem].SetConstant(_ttof(strNew)); break;
	case 9:m_vec_dataitem[ii.m_nItem].SetTgtData(_ttof(strNew)); break;
	case 10:m_vec_dataitem[ii.m_nItem].SetMaxData(_ttof(strNew)); break;
	case 11:m_vec_dataitem[ii.m_nItem].SetMinData(_ttof(strNew)); break;
	}
	if (g_dataitemdb.Update(m_vec_dataitem[ii.m_nItem]))//更新dataitem数据库
		NotifyUpdateDataItem(m_vec_dataitem[ii.m_nItem].GetID());
	else
	{
		m_vec_dataitem[ii.m_nItem] = dataitem;//退回更新dataitem内存
		m_listItem.SetItemText(ii.m_nItem, ii.m_nSubItem, strOld);//退回更新dataitem界面
	}
}


void CItemDatabase::OnImportProgressUpdate(int nProgress)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_IMPORTPROGRESS);
}


void CItemDatabase::OnImportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_IMPORTRESULT);
}


void CItemDatabase::OnExportProgressUpdate(int nProgress)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)nProgress, MSGUSER_EXPORTPROGRESS);
}


void CItemDatabase::OnExportResult(bool bResult)
{
	PostMessage(WM_MSGRECVPRO, (WPARAM)bResult, MSGUSER_EXPORTRESULT);
}


void CItemDatabase::NotifyUpdateDataItem(int nDataItemID)
{
	std::list<CWndItem> list_wnditem;
	CString strSql;
	strSql.Format(_T("SELECT * FROM WndItem WHERE DataItemID=%d"), nDataItemID);
	g_wnditemdb.Select(strSql, list_wnditem);
	if (list_wnditem.size() && GetParent())
		((CFrameWndEx *)GetParent())->GetActiveView()->PostMessage(WM_MSGRECVPRO, (WPARAM)nDataItemID, MSGUSER_UPDATEDATAITEM);
}


void CItemDatabase::NotifyDeleteDataItem(int nDataItemID)
{
	std::list<CWndItem> list_wnditem;
	CString strSql;
	strSql.Format(_T("SELECT * FROM WndItem WHERE DataItemID=%d"), nDataItemID);
	g_wnditemdb.Select(strSql, list_wnditem);
	if (list_wnditem.size() && GetParent())
		((CFrameWndEx *)GetParent())->GetActiveView()->PostMessage(WM_MSGRECVPRO, (WPARAM)nDataItemID, MSGUSER_DELETEDATAITEM);
}

