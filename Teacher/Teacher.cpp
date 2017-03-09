
// Teacher.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgMain.h"
#include "UI/DlgLogin.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/CoreDefine.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��

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

void GetCurrentIP(char* pBuffer)
{
	std::string strAddress;
	int nCardNo = 1;
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{
			//���������ж�IP,���ͨ��ѭ��ȥ�ж�
			IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			switch (pIpAdapterInfo->Type)
			{
			case MIB_IF_TYPE_OTHER:
			case MIB_IF_TYPE_ETHERNET:
			case MIB_IF_TYPE_TOKENRING:
			case MIB_IF_TYPE_FDDI:
			case MIB_IF_TYPE_PPP:
			case MIB_IF_TYPE_LOOPBACK:
			case MIB_IF_TYPE_SLIP:
			{
				// ��һ�Ѳ�֪��ʲôIP����
			}
			break;
			case 71: {	// ��������
				memcpy(pBuffer, pIpAddrString->IpAddress.String, 16);
			}
			break;
			default:
				// δ֪��������������
				break;
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
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

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("��ʦ��"));

	// 1����ʼ������
	// ��ʼ����Ϣ�����������
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	CTCPNet& tcpNet = CTCPNet::GetInstance();

	if (!tcpNet.InitNet(1234, &msgHelper, false, "127.0.0.1"))
		// if (!InitNet(1234, NetMsgCallBack, false, "192.168.18.100"))
	{
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return FALSE;
	}

	ST_MsgConnect stConnect;
	GetCurrentIP(stConnect.arrIP);
	stConnect.stMsgHead.clientType = eTeacher;
	CTCPNet::GetInstance().SendToServer(&stConnect, sizeof(ST_MsgConnect));

	{
		CDlgLogin dlgLogin;
		if (dlgLogin.DoModal() != IDOK)
		{
			// ɾ�����洴���� shell ��������
			if (pShellManager != NULL)
			{
				delete pShellManager;
			}
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

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

