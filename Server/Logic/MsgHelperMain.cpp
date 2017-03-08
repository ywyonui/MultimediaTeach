#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"
#include "BLL/define/EUIMsg.h"
#include <vector>

#include <iostream>
using namespace std;

#pragma region Imp
class CMsgHelperMain::Imp
{
public:
	Imp() {}
	~Imp(){}

private:
	HWND m_hWnd;
	std::string m_strToWnd;
	std::vector<ST_ClientInfo> m_vecClientInfo;

public:
	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }

private:
	void SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess);

public:
	// �û�����
	void UserConnect(DWORD dwID, void* vParam);
	// ע��
	void UserReg(DWORD dwID, void* vParam);
	// ��½
	void UserLogin(DWORD dwID, void* vParam);
};

// ֪ͨ������������ʾ����
void CMsgHelperMain::Imp::SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess)
{
	char strID[10] = { 0 };
	sprintf_s(strID, "<%d>", dwID);
	m_strToWnd = "SOCKET:";
	m_strToWnd += strID;
	m_strToWnd += strBase;
	m_strToWnd += bSuccess ? "�ɹ�" : "ʧ��";
	SendMessage(m_hWnd, EWND_MSG_SERVER_RECV, (WPARAM)(stHead), (LPARAM)(&m_strToWnd));
}

#pragma region ��Ϣ����
// ����
void CMsgHelperMain::Imp::UserConnect(DWORD dwID, void* vParam)
{
	ST_MsgConnect msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	// ��ͬ�ͻ������ͣ����ӳɹ�֮������ͬ����
	if (msg.stMsgHead.clientType == eTeacher)
	{

	}
	else
	{

	}

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "����", true);
}
// ע��
void CMsgHelperMain::Imp::UserReg(DWORD dwID, void* vParam)
{
	ST_MsgReg msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	ST_MsgRegResult msgRes;
	msgRes.stMsgHead.msgType = eRegResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().RegUser(msg.stRegInfo, msg.stMsgHead.clientType);
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgRegResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "ע��", msgRes.bSuccess);
}
// ��½
void CMsgHelperMain::Imp::UserLogin(DWORD dwID, void* vParam)
{
	ST_MsgLogin msg;
	memcpy(&msg, vParam, sizeof(ST_MsgLogin));
	ST_MsgLoginResult msgRes;
	msgRes.stMsgHead.msgType = eLoginResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo, msg.stMsgHead.clientType);

	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "��½", msgRes.bSuccess);
}

#pragma endregion ��Ϣ����


#pragma endregion Imp



CMsgHelperMain::CMsgHelperMain()
	: m_hWnd(NULL)
{
	m_pImp = new Imp;
}


CMsgHelperMain::~CMsgHelperMain()
{
	m_hWnd = NULL;
	if (m_pImp)
	{
		delete m_pImp;
		m_pImp = NULL;
	}
}


#pragma region �����ӿ�
/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CMsgHelperServer����������Ϣ�������
����˵��:	���ص�������
*************************************************************/
CMsgHelperMain& CMsgHelperMain::GetInstance(void)
{
	static CMsgHelperMain m_hInstance;
	return m_hInstance;
}


/*************************************************************
��������:	NetMsgCallBack
����˵��:	dwID:	������Socket
			vParam:	����
			nLen:	���ݳ���
�� �� ֵ:	void
����˵��:	������Ϣ�Ļص������������п�����һЩͨ�õĴ�������Ӧ����������ʵ��
*************************************************************/
void CMsgHelperMain::NetMsgCallBack(DWORD dwID, void* vParam, int nLen)
{
	ST_MsgHead stHead;
	memcpy(&stHead, vParam, sizeof(ST_MsgHead));

	switch (stHead.msgType)
	{
	case eConnect:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
	case eReg:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
	case eLogin:	// ��½��Ϣ
	{
		m_pImp->UserLogin(dwID, vParam);
	}
	break;

	default:
		break;
	}
}

/*************************************************************
��������:	NetMsgCallBack
����˵��:	dwID:	�ͻ���Socket,0��ʾ��������������ʾ���������յ��ͻ��˶Ͽ�
�� �� ֵ:	void
����˵��:	�Ͽ������Ǹ������¼����ͻ��˺ͷ���������ʽ��ͬ����������ӿ���д��Ĭ�ϰ汾ʲô������
*************************************************************/
void CMsgHelperMain::DisconnectCallBack(DWORD dwID)
{

}

/*************************************************************
��������:	SetHwnd
����˵��:	hWnd:	��������
�� �� ֵ:	void
����˵��:	����һ�����ڣ����ڷ�����Ϣ������UI�����Ϣ
*************************************************************/
void CMsgHelperMain::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pImp->SetHwnd(hWnd);
}

/*************************************************************
��������:	GetHwnd
����˵��:	void
�� �� ֵ:	hWnd:	��������
����˵��:	���ع�������
*************************************************************/
HWND CMsgHelperMain::GetHwnd(void)
{
	return m_hWnd;
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�