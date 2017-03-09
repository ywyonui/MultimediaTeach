
// Teacher.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgMain.h"
#include "UI/DlgLogin.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/CoreDefine.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库

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

void GetCurrentIP(char* pBuffer)
{
	std::string strAddress;
	int nCardNo = 1;
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{
			//可能网卡有多IP,因此通过循环去判断
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
				// 这一堆不知道什么IP类型
			}
			break;
			case 71: {	// 无线网卡
				memcpy(pBuffer, pIpAddrString->IpAddress.String, 16);
			}
			break;
			default:
				// 未知类型网卡就跳出
				break;
			}
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
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

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("教师端"));

	// 1、初始化网络
	// 初始化消息处理类的数据
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	CTCPNet& tcpNet = CTCPNet::GetInstance();

	if (!tcpNet.InitNet(1234, &msgHelper, false, "127.0.0.1"))
		// if (!InitNet(1234, NetMsgCallBack, false, "192.168.18.100"))
	{
		AfxMessageBox(_T("连接服务器失败"));
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
			// 删除上面创建的 shell 管理器。
			if (pShellManager != NULL)
			{
				delete pShellManager;
			}
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

