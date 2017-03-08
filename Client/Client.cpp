
// Client.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Client.h"
#include "DlgMain.h"

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


CString CClientApp::GetModulePath()
{
	CString    sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sPath.ReleaseBuffer();
	int    nPos;
	nPos = sPath.ReverseFind('\\');
	sPath = sPath.Left(nPos);
	return    sPath;
}

// CClientApp 初始化

BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("多媒体教学演示系统"));

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用    
	CString strConfigPath = GetModulePath();
	strConfigPath += _T("\\..\\config.ini");

	WritePrivateProfileString(_T("Client"), _T("Type"), _T("1"), strConfigPath);
	WritePrivateProfileString(_T("Server"), _T("IP"), _T("1"), strConfigPath);
	WritePrivateProfileString(_T("Server"), _T("Port"), _T("1"), strConfigPath);


	return FALSE;
}

