#include "stdafx.h"
#include "ZExcelImport.h"


ZExcelImport::ZExcelImport()
	: m_bIsRun(false)
	, m_strSrcPath(_T(""))
	, m_nRowOffset(0)
	, m_nColumnOffset(0)
{
}


ZExcelImport::ZExcelImport(ZExcelImport && zxlsimport)
	: m_thread(std::move(zxlsimport.m_thread))
	, m_bIsRun(zxlsimport.m_bIsRun)
	, m_strSrcPath(zxlsimport.m_strSrcPath)
	, m_nRowOffset(zxlsimport.m_nRowOffset)
	, m_nColumnOffset(zxlsimport.m_nColumnOffset)
	, m_vec2_strDataImport(std::move(zxlsimport.m_vec2_strDataImport))
{
}


ZExcelImport & ZExcelImport::operator=(ZExcelImport && zxlsimport)
{
	if (this != &zxlsimport)
	{
		StopImport();
		m_thread = std::move(zxlsimport.m_thread);
		m_bIsRun = zxlsimport.m_bIsRun;
		m_strSrcPath = zxlsimport.m_strSrcPath;
		m_nRowOffset = zxlsimport.m_nRowOffset;
		m_nColumnOffset = zxlsimport.m_nColumnOffset;
		m_vec2_strDataImport = std::move(zxlsimport.m_vec2_strDataImport);
	}
	return *this;
}


ZExcelImport::~ZExcelImport()
{
	StopImport();
}


bool ZExcelImport::IsRun()
{
	return m_bIsRun;
}


bool ZExcelImport::StartImport()
{
	if (!m_bIsRun)
	{
		if (m_thread.joinable())
			m_thread.join();
		std::promise<void> prom;
		std::future<void> f = prom.get_future();
		m_thread = std::thread(&ZExcelImport::ImportThread, this, std::move(prom));
		f.wait();
		return true;
	}
	return false;
}


void ZExcelImport::StopImport()
{
	m_bIsRun = false;
	if (m_thread.joinable())
		m_thread.join();
}


void ZExcelImport::ImportThread(std::promise<void> prom)
{
	m_bIsRun = true;
	prom.set_value();
	CString strError;
	bool bResult = false;
	ZOperateExcelFile::InitOle();
	ZOperateExcelFile operateexcel;
	if (operateexcel.InitExcel(&strError))
	{
		if (operateexcel.OpenExcelFile(m_strSrcPath))
		{
			if (operateexcel.LoadSheet(0))
			{
				std::vector<std::vector<CString>> & vec2_Data = m_vec2_strDataImport;
				vec2_Data.clear();
				int nRow = (int)operateexcel.GetRowCount();
				int nColumn = (int)operateexcel.GetColumnCount();
				for (int i = m_nRowOffset; i < nRow; ++i)
				{
					std::vector<CString> vec_strRow;
					for (int j = m_nColumnOffset; j < nColumn; ++j)
						vec_strRow.push_back(operateexcel.GetCellString((long)i, (long)j));
					vec2_Data.push_back(vec_strRow);
					int nProgress = (i- m_nRowOffset+1) * 100 / (nRow - m_nRowOffset);
					if (m_bIsRun)
						OnImportProgressUpdate(nProgress);
					else
						break;
				}
				bResult = true;
			}
			operateexcel.CloseExcelFile();
		}
		operateexcel.ReleaseExcel();
	}
	if (m_bIsRun)
		OnImportResult(bResult);
	ZOperateExcelFile::UninitOle();
	m_bIsRun = false;
}
