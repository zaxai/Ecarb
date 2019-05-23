#include "stdafx.h"
#include "ZExcelExport.h"


ZExcelExport::ZExcelExport()
	: m_bIsRun(false)
	, m_strDestPath(_T(""))
	, m_strDemoPath(_T(""))
	, m_nRowOffset(0)
	, m_nColumnOffset(0)
{
}


ZExcelExport::ZExcelExport(ZExcelExport && zxlsexport)
	: m_thread(std::move(zxlsexport.m_thread))
	, m_bIsRun(zxlsexport.m_bIsRun)
	, m_strDestPath(zxlsexport.m_strDestPath)
	, m_strDemoPath(zxlsexport.m_strDemoPath)
	, m_nRowOffset(zxlsexport.m_nRowOffset)
	, m_nColumnOffset(zxlsexport.m_nColumnOffset)
	, m_vec2_strDataExport(std::move(zxlsexport.m_vec2_strDataExport))
{
}


ZExcelExport & ZExcelExport::operator=(ZExcelExport && zxlsexport)
{
	if (this != &zxlsexport)
	{
		StopExport();
		m_thread = std::move(zxlsexport.m_thread);
		m_bIsRun = zxlsexport.m_bIsRun;
		m_strDestPath = zxlsexport.m_strDestPath;
		m_strDemoPath = zxlsexport.m_strDemoPath;
		m_nRowOffset = zxlsexport.m_nRowOffset;
		m_nColumnOffset = zxlsexport.m_nColumnOffset;
		m_vec2_strDataExport = std::move(zxlsexport.m_vec2_strDataExport);
	}
	return *this;
}


ZExcelExport::~ZExcelExport()
{
	StopExport();
}


bool ZExcelExport::IsRun()
{
	return m_bIsRun;
}


bool ZExcelExport::StartExport()
{
	if (!m_bIsRun)
	{
		if (m_thread.joinable())
			m_thread.join();
		std::promise<void> prom;
		std::future<void> f = prom.get_future();
		m_thread = std::thread(&ZExcelExport::ExportThread, this, std::move(prom));
		f.wait();
		return true;
	}
	return false;
}


void ZExcelExport::StopExport()
{
	m_bIsRun = false;
	if (m_thread.joinable())
		m_thread.join();
}


void ZExcelExport::ExportThread(std::promise<void> prom)
{
	m_bIsRun = true;
	prom.set_value();
	CString strError;
	bool bResult = false;
	ZOperateExcelFile::InitOle();
	ZOperateExcelFile operateexcel;
	if (operateexcel.InitExcel(&strError))
	{
		if (operateexcel.OpenExcelFile(m_strDemoPath))
		{
			if (operateexcel.LoadSheet(0))
			{
				const std::vector<std::vector<CString>> & vec2_Data = m_vec2_strDataExport;
				int nRow = vec2_Data.size();
				if (nRow)
				{
					int nColumn = vec2_Data[0].size();
					for (int i = 0; i < nRow; ++i)
					{
						for (int j = 0; j < nColumn; ++j)
							operateexcel.SetCellString(i + m_nRowOffset, j + m_nColumnOffset, vec2_Data[i][j]);
						int nProgress = (i+1) * 100 / nRow;
						if (m_bIsRun)
							OnExportProgressUpdate(nProgress);
						else
							break;
					}
				}
				operateexcel.SaveasXSLFile(m_strDestPath);
				bResult = true;
			}
			operateexcel.CloseExcelFile();
		}
		operateexcel.ReleaseExcel();
	}
	if (m_bIsRun)
		OnExportResult(bResult);
	ZOperateExcelFile::UninitOle();
	m_bIsRun = false;
}
