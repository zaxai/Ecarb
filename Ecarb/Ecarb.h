
// Ecarb.h: Ecarb 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ZClassLib.h" 

// CEcarbApp:
// 有关此类的实现，请参阅 Ecarb.cpp
//

class CEcarbApp : public CWinAppEx
{
public:
	CEcarbApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
};

extern CEcarbApp theApp;
extern std::list<CPage> g_list_page;
extern CPageDB g_pagedb;
extern CWndInfoDB g_wndinfodb;
extern CWndItemDB g_wnditemdb;
extern CDataItemDB g_dataitemdb;
extern ZDevicePort g_zdp;
extern ZModbus g_modbus;
extern bool g_bIsCommunicating;
extern std::vector<CCommItem *> g_vec_p_ciCalibrate;
extern std::vector<CCommItem *> g_vec_p_ciMonitor;
extern std::vector<CCommItem *>::iterator g_it_p_ciMonitor;
extern std::vector<CCommItem *>::iterator g_it_p_commitem;
extern bool g_bIsLogChecking;
extern CRecordDB g_recorddb;
extern CRecordItemDB g_rditemdb;
extern CRecordDataDB g_rddatadb;
extern int g_nRecordID;
extern int g_nRecordDataID;
extern CString g_strPassword;
extern int g_nTotalRecords;
extern int g_nSavedRecords;
extern std::mutex g_mutex;

#define WM_MSGRECVPRO WM_USER+1

enum MSGUSER
{
	MSGUSER_CLOSEAPP = 0,
	MSGUSER_COMMSTART,
	MSGUSER_COMMSTOP,
	MSGUSER_SENDRESULT,
	MSGUSER_RECVRESULT,
	MSGUSER_SENDSUCCEED,
	MSGUSER_RECVSUCCEED,
	MSGUSER_SENDFAIL,
	MSGUSER_RECVFAIL,
	MSGUSER_CLOSEBASICWND,
	MSGUSER_UPDATEDATAITEM,
	MSGUSER_DELETEDATAITEM,
	MSGUSER_IMPORTPROGRESS,
	MSGUSER_IMPORTRESULT,
	MSGUSER_EXPORTPROGRESS,
	MSGUSER_EXPORTRESULT,
	MSGUSER_UPDATEPROGRESS,
	MSGUSER_ABANDONSAVERECORD
};