/*
** Version  1.0.0.2
** Date     2019.04.18
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com
*/
#pragma once
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CApplication.h"
//本类中的传参lSheetIndex,lRow,lColumn均是从0开始
class ZOperateExcelFile
{
private:
	CApplication m_excelapplication;//EXCEL的进程实例
	CWorkbooks m_excelbooks;//Excel Book集合，（多个文件时）
	CWorkbook m_excelworkbook;//当前使用的Book，当前处理的文件
	CWorksheets m_excelsheets;//EXCEL的sheets集合
	CWorksheet m_excelworksheet;//当前使用sheet
	CRange m_excelrange;//当前的操作区域
	CString m_strFileName;//打开的Excel文件名
	int m_nIsInitCount;
	BOOL m_nIsPreloadSucceed;//是否已经预加载了某个sheet的数据
	COleSafeArray m_olesafearray;//Create the SAFEARRAY from the VARIANT ret.
	BOOL PreloadSheet();//预先加载
private:
	static COleVariant m_covOptional;
public:
	ZOperateExcelFile();
	virtual ~ZOperateExcelFile();
	BOOL InitExcel(CString * p_strError = NULL);//初始化EXCEL
	void ReleaseExcel();//释放EXCEL
	void ShowInExcel(BOOL nIsShow);
	BOOL IsCellString(long lRow, long lColumn);//检查一个CELL是否是字符串
	BOOL IsCellInt(long lRow, long lColumn);//检查一个CELL是否是数值
	CString GetCellString(long lRow, long lColumn);//得到一个CELL的String
	int GetCellInt(long lRow, long lColumn);//得到整数
	double GetCellDouble(long lRow, long lColumn);//得到double的数据
	long GetRowCount();//取得行的总数
	long GetColumnCount();//取得列的总数
	BOOL LoadSheet(long lSheetIndex, BOOL nIsPreload = TRUE);//使用某个shet，shit，shit
	BOOL LoadSheet(const CString & strSheet, BOOL nIsPreload = TRUE);//通过名称使用某个sheet，
	CString GetSheetName(long lSheetIndex);//通过序号取得某个Sheet的名称
	long GetSheetCount();//得到Sheet的总数
	BOOL OpenExcelFile(const CString & strFileName);//打开文件
	void CloseExcelFile(BOOL nIsSave = FALSE);//关闭打开的Excel 文件，有时候打开EXCEL文件就要
	void SaveasXSLFile(const CString & strFilePath);//另存为一个EXCEL文件
	CString GetFileName();	//取得打开文件的名称
	CString GetLoadSheetName();//取得打开sheet的名称
	void SetCellInt(long lRow, long lColumn, int nData);//写入一个CELL一个int
	void SetCellString(long lRow, long lColumn, const CString & strData);//写入一个CELL一个string
public:
	static void InitOle();
	static void UninitOle();
	static char * GetColumnName(long lColumn);//取得列的名称，比如27->AA	
};
