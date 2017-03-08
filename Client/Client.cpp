
// Client.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Client.h"
#include "DlgMain.h"
#include "UI/DlgLogin.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/define/EnumType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp 构造

CClientApp::CClientApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CClientApp 对象

CClientApp theApp;

CString GetModulePath()
{
	CString    sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}

CString GetConfigPath()
{
	return GetModulePath() + _T("\\..\\config.ini");
}

// CClientApp 初始化

BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("多媒体教学演示系统"));

	// 获取配置信息用于数据初始化
	CString strConfigPath = GetConfigPath();
	CStringA strIPA = "";
	CString strIP = _T("");
	int nPort = 0;
	EClientType appClientType = (EClientType)GetPrivateProfileInt(_T("Client"), _T("Type"), 0, strConfigPath);
	GetPrivateProfileString(_T("Server"), _T("IP"), _T("127.0.0.1"), strIP.GetBuffer(), MAX_PATH, strConfigPath);
	strIPA = strIP;
	nPort = GetPrivateProfileInt(_T("Server"), _T("Port"), 0, strConfigPath);

	// 1、初始化网络
	// 初始化消息处理类的数据
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	if (!CTCPNet::GetInstance().InitNet(nPort, &msgHelper, false, strIPA.GetBuffer()))
	{
		AfxMessageBox(_T("连接服务器失败"));
		return FALSE;
	}

	{
		CDlgLogin dlgLogin;
		if (dlgLogin.DoModal() != IDOK)
		{
		}
		else
		{	// 主程序
			CDlgMain dlg;
			dlg.SetClientType(appClientType);
			m_pMainWnd = (CWnd*)&dlg;

			if (dlg.DoModal() == IDOK)
			{

			}
		}
	}

	return FALSE;
}

