
// Ecarb.cpp: 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Ecarb.h"
#include "MainFrm.h"

#include "EcarbDoc.h"
#include "EcarbView.h"
#include "CEcarbActivation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEcarbApp

BEGIN_MESSAGE_MAP(CEcarbApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEcarbApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEcarbApp 构造

CEcarbApp::CEcarbApp() noexcept
	: m_gdiplusToken(0)
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Ecarb.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CEcarbApp 对象

CEcarbApp theApp;
std::list<CPage> g_list_page;
CPageDB g_pagedb;
CWndInfoDB g_wndinfodb;
CWndItemDB g_wnditemdb;
CDataItemDB g_dataitemdb;
ZDevicePort g_zdp;
ZModbus g_modbus;
bool g_bIsCommunicating = false;
std::vector<CCommItem *> g_vec_p_ciCalibrate;
std::vector<CCommItem *> g_vec_p_ciMonitor;
std::vector<CCommItem *>::iterator g_it_p_ciMonitor;
std::vector<CCommItem *>::iterator g_it_p_commitem;
bool g_bIsLogChecking = false;
CRecordDB g_recorddb;
CRecordItemDB g_rditemdb;
CRecordDataDB g_rddatadb;
int g_nRecordID = CRecordDB::GetNextInsertID();
int g_nRecordDataID = CRecordDataDB::GetNextInsertID();
CString g_strPassword;
int g_nTotalRecords = 0;
int g_nSavedRecords = 0;
std::mutex g_mutex;

// CEcarbApp 初始化

BOOL CEcarbApp::InitInstance()
{
	SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	if (!CActivation::Verify(CActivation::GetSerialNumber(),ZUtil::GetExeCatalogPath()+_T("\\Ecarb.af")))
	{
		CEcarbActivation activationDlg;
		if (activationDlg.DoModal() != IDOK)
			return FALSE;
	}
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, nullptr);
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEcarbDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CEcarbView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CEcarbApp::ExitInstance()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEcarbApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CEcarbApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEcarbApp 自定义加载/保存方法

void CEcarbApp::PreLoadState()
{
	BOOL bNameValid[5];
	CString strName[5];
	bNameValid[0] = strName[0].LoadString(IDS_EDIT_MENU);
	bNameValid[1] = strName[1].LoadString(IDS_EDIT_PAGE_MENU);
	bNameValid[2] = strName[2].LoadString(IDS_EDIT_WND_MENU);
	bNameValid[3] = strName[3].LoadString(IDS_OPERATE_BASICWND);
	bNameValid[4] = strName[4].LoadString(IDS_OPERATE_DATARECORD);
	for (int i = 0; i < 5; ++i)
		ASSERT(bNameValid[i]);
	GetContextMenuManager()->AddMenu(strName[0], IDR_POPUP_EDIT);
	GetContextMenuManager()->AddMenu(strName[1], IDR_POPUP_EDIT_PAGE);
	GetContextMenuManager()->AddMenu(strName[2], IDR_POPUP_EDIT_WND);
	GetContextMenuManager()->AddMenu(strName[3], IDR_POPUP_BASICWND);
	GetContextMenuManager()->AddMenu(strName[4], IDR_POPUP_DATARECORD);
}

void CEcarbApp::LoadCustomState()
{
}

void CEcarbApp::SaveCustomState()
{
}

// CEcarbApp 消息处理程序



