
// Teacher.cpp : ����Ӧ�ó��������Ϊ��
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


// CTeacherApp ����

CTeacherApp::CTeacherApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTeacherApp ����

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

// CTeacherApp ��ʼ��

BOOL CTeacherApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("��ʦ��"));

	// 1����ʼ������
	// ��ʼ����Ϣ�����������
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
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return FALSE;
	}

	{
		CDlgLogin dlgLogin;
		if (dlgLogin.DoModal() != IDOK)
		{
			return FALSE;
		}

		{	// ������
			CDlgMain dlg;
			m_pMainWnd = &dlg;

			if (dlg.DoModal() == IDOK)
			{

			}
		}
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

