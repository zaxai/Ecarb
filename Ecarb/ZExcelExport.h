/*
** Version  1.0.0.6
** Date     2019.04.18
** Author   zax
** Copyright (C) Since 2009 Zaxai.Com All Rights Reserved
*/
#pragma once
#include "ZOperateExcelFile.h"
#include <vector>
#include <thread>
#include <future>

class ZExcelExport
{
private:
	std::thread m_thread;
	bool m_bIsRun;
	void ExportThread(std::promise<void> prom);
	ZExcelExport(const ZExcelExport & zxlsexport) = delete;
	ZExcelExport & operator = (const ZExcelExport & zxlsexport) = delete;
protected://派生类中判断IsRun()==false后允许设置protected成员变量
	CString m_strDestPath;
	CString m_strDemoPath;
	int m_nRowOffset;
	int m_nColumnOffset;
	std::vector<std::vector<CString>> m_vec2_strDataExport;
public:
	ZExcelExport();
	ZExcelExport(ZExcelExport && zxlsexport);
	ZExcelExport & operator=(ZExcelExport && zxlsexport);
	virtual ~ZExcelExport();
	bool IsRun();
	bool StartExport();
	void StopExport();
	virtual void OnExportProgressUpdate(int nProgress) = 0;
	virtual void OnExportResult(bool bResult) = 0;
};

