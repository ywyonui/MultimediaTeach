
// Teacher.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgMain.h"
#include "UI/DlgLogin.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/CoreDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeacherApp

BEGIN_MESSAGE_MAP(CTeacherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTeacherApp 构造

CTeacherApp::CTeacherApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTeacherApp 对象

CTeacherApp theApp;


CString CTeacherApp::GetModulePath()
{
	CString    sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}

// CTeacherApp 初始化

BOOL CTeacherApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("教师端"));

	// 1、初始化网络
	// 初始化消息处理类的数据
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	CTCPNet& tcpNet = CTCPNet::GetInstance();

	char arrServerIP[16] = { 0 };
	int nServerPort;
	CStringA strINIPath = CStringA(GetModulePath());
	strINIPath += "\\..\\config.ini";
	GetPrivateProfileStringA("Server", "IP", "", arrServerIP, 16, strINIPath);
	nServerPort = GetPrivateProfileIntA("Server", "Port", 0, strINIPath);

	if (!tcpNet.InitNet(nServerPort, &msgHelper, false, arrServerIP))
	{
		AfxMessageBox(_T("连接服务器失败"));
		return FALSE;
	}

	{
		CDlgLogin dlgLogin;
		if (dlgLogin.DoModal() != IDOK)
		{
			return FALSE;
		}

		{	// 主程序
			CDlgMain dlg;
			m_pMainWnd = &dlg;

			if (dlg.DoModal() == IDOK)
			{

			}
		}
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

