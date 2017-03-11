// DlgLogin.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Teacher.h"
#include "afxdialogex.h"
#include "DlgLogin.h"
#include "DlgReg.h"

#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��


#include "Logic/MsgHelperMain.h"

#include "BLL/CoreDefine.h"

#include <string>

// CDlgLogin �Ի���

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

#pragma region ����

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

	PIP_ADAPTER_INFO pIpInfo = pIpAdapterInfo;
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
			case 71: 
			{	// ��������
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
	if (pIpInfo)
	{
		delete pIpInfo;
	}
}
#pragma endregion ����



BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlgLogin::OnLogin)
	ON_BN_CLICKED(IDC_BTN_REG, &CDlgLogin::OnRegUser)
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgLogin::OnServerMsgResult)
END_MESSAGE_MAP()


// CDlgLogin ��Ϣ�������
void CDlgLogin::OnLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strName = _T("");
	CString strPwd = _T("");

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
	if (strName.IsEmpty()) 
	{
		AfxMessageBox(_T("�˺Ų���Ϊ��"));
		return;
	}
	else if (strName.GetLength() > 30)
	{
		AfxMessageBox(_T("�˺ų��Ȳ��ܳ���30���ַ�"));
		return;
	}
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);
	if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("���벻��Ϊ��"));
		return;
	}
	else if (strPwd.GetLength() > 30)
	{
		AfxMessageBox(_T("���볤�Ȳ��ܳ���30���ַ�"));
		return;
	}
	
	// ׼�������ݴ��������������ж�
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	// 1������ǰ���ھ��������Ϣ�����������ڽ�����Ϣ
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2��������ת����string�ĸ�ʽ
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // ���λس�
			OnLogin();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ST_MsgConnect stConnect;
	GetCurrentIP(stConnect.arrIP);
	stConnect.stMsgHead.clientType = eTeacher;
	CTCPNet::GetInstance().SendToServer(&stConnect, sizeof(ST_MsgConnect));

	CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());

	GetDlgItem(IDC_EDIT_NAME)->SetWindowText(_T("Teacher"));
	GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T("123456"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
