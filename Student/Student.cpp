
// Student.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Student.h"
#include "DlgMain.h"
#include "UI/DlgLogin.h"

#include "TCPNet.h"
#include "Logic/MsgHelperMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudentApp

BEGIN_MESSAGE_MAP(CStudentApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CStudentApp 构造

CStudentApp::CStudentApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CStudentApp 对象

CStudentApp theApp;


// CStudentApp 初始化

BOOL CStudentApp::InitInstance()
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

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	SetRegistryKey(_T("斗地主客户端"));

	// 1、初始化网络
	// 初始化消息处理类的数据
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	if (!CTCPNet::GetInstance().InitNet(1234, &msgHelper, false, "127.0.0.1"))
		// if (!InitNet(1234, NetMsgCallBack, false, "192.168.18.100"))
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


	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

