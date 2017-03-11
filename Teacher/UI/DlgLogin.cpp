// DlgLogin.cpp : 实现文件
//
#include "stdafx.h"
#include "Teacher.h"
#include "afxdialogex.h"
#include "DlgLogin.h"
#include "DlgReg.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库


#include "Logic/MsgHelperMain.h"

#include "BLL/CoreDefine.h"

#include <string>

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogin::IDD, pParent)
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

#pragma region 其他

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

	PIP_ADAPTER_INFO pIpInfo = pIpAdapterInfo;
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
			case 71: 
			{	// 无线网卡
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
	if (pIpInfo)
	{
		delete pIpInfo;
	}
}
#pragma endregion 其他



BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlgLogin::OnLogin)
	ON_BN_CLICKED(IDC_BTN_REG, &CDlgLogin::OnRegUser)
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgLogin::OnServerMsgResult)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序
void CDlgLogin::OnLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strName = _T("");
	CString strPwd = _T("");

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
	if (strName.IsEmpty()) 
	{
		AfxMessageBox(_T("账号不能为空"));
		return;
	}
	else if (strName.GetLength() > 30)
	{
		AfxMessageBox(_T("账号长度不能超过30个字符"));
		return;
	}
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);
	if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	else if (strPwd.GetLength() > 30)
	{
		AfxMessageBox(_T("密码长度不能超过30个字符"));
		return;
	}
	
	// 准备将数据传给服务器进行判断
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	// 1、将当前窗口句柄交给消息处理器，用于接收消息
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2、将数据转换成string的格式
	CStringA strNameA(strName);
	CStringA strPwdA(strPwd);

	ST_MsgLogin msg;
	msg.stMsgHead.clientType = eTeacher;

	memcpy(msg.stLoginInfo.strUserName, strNameA.GetBuffer(), strNameA.GetLength());
	memcpy(msg.stLoginInfo.strUserPwd, strPwdA.GetBuffer(), strPwdA.GetLength());

	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgLogin));
}


void CDlgLogin::OnRegUser()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgReg dlg;
	if (IDOK == dlg.DoModal())
	{

	}

}


LRESULT CDlgLogin::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
{
	switch ((EMsgType)lParam)
	{
	case eMsgLoginResult:
	{
		PostMessage(WM_COMMAND, IDOK, 0);
	}
		break;

	default:
		break;
	}

	return 0;
}

BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // 屏蔽回车
			OnLogin();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ST_MsgConnect stConnect;
	GetCurrentIP(stConnect.arrIP);
	stConnect.stMsgHead.clientType = eTeacher;
	CTCPNet::GetInstance().SendToServer(&stConnect, sizeof(ST_MsgConnect));

	CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());

	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(_T("Teacher"));
	GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T("123456"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
