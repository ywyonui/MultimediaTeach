
// Client.cpp : ����Ӧ�ó��������Ϊ��
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


// CClientApp ����

CClientApp::CClientApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClientApp ����

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

// CClientApp ��ʼ��

BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("��ý���ѧ��ʾϵͳ"));

	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����    
	CString strConfigPath = GetModulePath();
	strConfigPath += _T("\\..\\config.ini");

	WritePrivateProfileString(_T("Client"), _T("Type"), _T("1"), strConfigPath);
	WritePrivateProfileString(_T("Server"), _T("IP"), _T("1"), strConfigPath);
	WritePrivateProfileString(_T("Server"), _T("Port"), _T("1"), strConfigPath);


	return FALSE;
}

