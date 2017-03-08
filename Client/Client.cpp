
// Client.cpp : ����Ӧ�ó��������Ϊ��
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


// CClientApp ����

CClientApp::CClientApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClientApp ����

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

// CClientApp ��ʼ��

BOOL CClientApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("��ý���ѧ��ʾϵͳ"));

	// ��ȡ������Ϣ�������ݳ�ʼ��
	CString strConfigPath = GetConfigPath();
	CStringA strIPA = "";
	CString strIP = _T("");
	int nPort = 0;
	EClientType appClientType = (EClientType)GetPrivateProfileInt(_T("Client"), _T("Type"), 0, strConfigPath);
	GetPrivateProfileString(_T("Server"), _T("IP"), _T("127.0.0.1"), strIP.GetBuffer(), MAX_PATH, strConfigPath);
	strIPA = strIP;
	nPort = GetPrivateProfileInt(_T("Server"), _T("Port"), 0, strConfigPath);

	// 1����ʼ������
	// ��ʼ����Ϣ�����������
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	if (!CTCPNet::GetInstance().InitNet(nPort, &msgHelper, false, strIPA.GetBuffer()))
	{
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return FALSE;
	}

	{
		CDlgLogin dlgLogin;
		if (dlgLogin.DoModal() != IDOK)
		{
		}
		else
		{	// ������
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

