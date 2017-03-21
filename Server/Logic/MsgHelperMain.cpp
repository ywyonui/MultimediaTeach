#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"
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
	std::string m_strFileName;

public:
	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	std::vector<ST_ClientInfo>& GetClientList() { return m_vecClientInfo; }

private:
	void SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess);

public:
	// �û�����
	void UserConnect(DWORD dwID, void* vParam);
	// ע��
	void UserReg(DWORD dwID, void* vParam);
	// ��½
	void UserLogin(DWORD dwID, void* vParam);
	// �����ȡ�б�
	void AskClientList(DWORD dwID, void* vParam);
	// ��ʦ�˵�����
	void SetStudentClientIP(DWORD dwID, void* vParam);

	// �������Ϣת��
	void TransmitMsgHeadToClient(ST_MsgHead& msg, DWORD dwID = 0);

	// ת��������Ϣ
	void  TransmitFileToClient(DWORD dwID, void* vParam);

public:
	void DisconnectCallBack(DWORD dwID);


};

// ֪ͨ������������ʾ����
void CMsgHelperMain::Imp::SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess)
{
	char strID[10] = { 0 };
	sprintf_s(strID, "<%d>", dwID);
	std::string str = "SOCKET:";
	str += strID;
	str += strBase;
	str += bSuccess ? "�ɹ�" : "ʧ��";
	SendMessage(m_hWnd, EWND_MSG_SERVER_RECV, (WPARAM)(stHead), (LPARAM)(&str));
}

#pragma region ��Ϣ����
// ����
void CMsgHelperMain::Imp::UserConnect(DWORD dwID, void* vParam)
{
	ST_MsgConnect msg;
	memcpy(&msg, vParam, sizeof(ST_MsgConnect));

	ST_MsgHead msgRes;
	msgRes.msgType = eMsgConnectResult;
	msgRes.nSubType = msg.stMsgHead.nSubType;
	// ��ͬ�ͻ������ͣ����ӳɹ�֮������ͬ����
	if (msg.stMsgHead.nSubType == eTeacher)
	{
		ST_ClientInfo st;
		st.dwSocket = dwID;
		memcpy(st.arrIP, msg.arrIP, 16);
		st.eCS = eClientConnect;
		st.eCT = msg.stMsgHead.nSubType;
		m_vecClientInfo.push_back(st);

		msgRes.nSubType = 0;
	}
	else 
	{
		bool bIPinList = false;
		// ѧ���˵Ļ�����Ϣ���ڽ�ʦ�˵������н��в����
		for (auto& it : m_vecClientInfo)
		{
			// �ҵ�IP��ͬ�Ŀͻ��ˣ��ı�����״̬��Ϣ�����Ҽ�¼��Ӧ��Socket
			int nFlag = strcmp(it.arrIP, msg.arrIP);
			if (nFlag == 0 && !it.dwSocket && it.eCT == eStudent)
			{
				it.dwSocket = dwID;
				it.eCS = eClientConnect;
				it.eCT = msg.stMsgHead.nSubType;
				bIPinList = true;
				break;
			}
		}
		msgRes.nSubType = bIPinList ? 0 : -1;
	}
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgHead));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "����", true);
}
// ע��
void CMsgHelperMain::Imp::UserReg(DWORD dwID, void* vParam)
{
	ST_MsgReg msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	ST_MsgRegResult msgRes;
	msgRes.stMsgHead.msgType = eMsgRegResult;
	msgRes.stMsgHead.nSubType = msg.stMsgHead.nSubType;
	msgRes.bSuccess = CUserMgr::GetInstance().RegUser(msg.stRegInfo, msg.stMsgHead.nSubType);
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgRegResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "ע��", msgRes.bSuccess);
}
// ��½
void CMsgHelperMain::Imp::UserLogin(DWORD dwID, void* vParam)
{
	ST_MsgLogin msg;
	memcpy(&msg, vParam, sizeof(ST_MsgLogin));
	ST_MsgLoginResult msgRes;
	msgRes.stMsgHead.msgType = eMsgLoginResult;
	msgRes.stMsgHead.nSubType = msg.stMsgHead.nSubType;
	msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo, msg.stMsgHead.nSubType);

	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));

	for (auto& it : m_vecClientInfo)
	{
		// �ҵ�IP��ͬ�Ŀͻ��ˣ��ı�����״̬��Ϣ
		if (dwID == it.dwSocket)
		{
			it.eCS = eClientLogin;
			break;
		}
	}
	SendRecvMsgToUI(dwID, &msg.stMsgHead, "��½", msgRes.bSuccess);
}

// �Ͽ�����
void CMsgHelperMain::Imp::DisconnectCallBack(DWORD dwID)
{
	// �Ͽ���ʱ�򣬽�ʦ����Ҫ������ѧ���˷�����Ϣ�����ҹر�����ѧ���ˣ�������շ������б�
	for (auto& it = m_vecClientInfo.begin(); it != m_vecClientInfo.end(); )
	{
		if (it->dwSocket == dwID) 
		{
			if (it->eCT == eTeacher)
			{
				auto it2 = it;
				m_vecClientInfo.erase(it2);
				ST_MsgHead stResult;
				stResult.msgType = eMsgDisConnect;
				int nSize = sizeof(ST_MsgHead);
				for (auto& it : m_vecClientInfo)
				{
					CTCPNet::GetInstance().SendToClient(it.dwSocket, &stResult, nSize);
				}
				m_vecClientInfo.clear();
			}
			else
			{
				it->eCS = eClientDisConnect;
				it->dwSocket = 0;
				AskClientList(0, NULL);
			}
			break;
		}
		it++;
	}
}

// �����ȡ�б�
void CMsgHelperMain::Imp::AskClientList(DWORD dwID, void* vParam)
{
	ST_MsgAskClientListResult msgRes;
	msgRes.nSize = m_vecClientInfo.size();
	int nLen = sizeof(ST_ClientInfo);
	for (int i = 0; i < msgRes.nSize; i++)
	{
		memcpy(msgRes.arrClient + i, &m_vecClientInfo.at(i), nLen);
	}
	for (auto& it : m_vecClientInfo)
	{
		if (it.dwSocket)
		{
			CTCPNet::GetInstance().SendToClient(it.dwSocket, &msgRes, sizeof(ST_MsgAskClientListResult));
		}
	}

	SendRecvMsgToUI(dwID, (ST_MsgHead*)vParam, "��ȡ�ͻ����б�", true);

}
// ��ʦ�˵�����
void CMsgHelperMain::Imp::SetStudentClientIP(DWORD dwID, void* vParam)
{
	ST_ClientInfo stClientInfo;
	memcpy(&stClientInfo, &(m_vecClientInfo.at(0)), sizeof(ST_ClientInfo));
	// ֪ͨ����ѧ���˶Ͽ�����
	DisconnectCallBack(dwID);

	ST_MsgSettingStudentIP msg;
	memcpy(&msg, vParam, sizeof(ST_MsgSettingStudentIP));

	m_vecClientInfo.clear();
	m_vecClientInfo.push_back(stClientInfo);

	for (int i = 0; i < msg.nSize; i++)
	{
		ST_ClientInfo st;
		st.dwSocket = 0;
		st.eCS = eClientDisConnect;
		st.eCT = eStudent;
		memcpy(st.arrIP, msg.arrClient[i], 16);
		m_vecClientInfo.push_back(st);
	}
}
// ����
void CMsgHelperMain::Imp::TransmitMsgHeadToClient(ST_MsgHead& msg, DWORD dwID)
{
	for (auto& it : m_vecClientInfo)
	{
		if (it.dwSocket != dwID && it.dwSocket)
		{
			CTCPNet::GetInstance().SendToClient(it.dwSocket, &msg, sizeof(ST_MsgHead));
		}
	}
}

void CMsgHelperMain::Imp::TransmitFileToClient(DWORD dwID, void* vParam)
{
	ST_MsgFileTransmit msg;
	memcpy(&msg, vParam, sizeof(ST_MsgFileTransmit));
	for (auto& it : m_vecClientInfo)
	{
		if (it.dwSocket != dwID && it.dwSocket)
		{
			CTCPNet::GetInstance().SendToClient(it.dwSocket, &msg, sizeof(ST_MsgFileTransmit));
		}
	}
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
	case eMsgConnect:
	{
		m_pImp->UserConnect(dwID, vParam);
	}
	break;
	case eMsgReg:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
	case eMsgLogin:	// ��½��Ϣ
	{
		m_pImp->UserLogin(dwID, vParam);
	}
	break;
	case eMsgAskClientList:	// �����ȡ�б�
	{
		m_pImp->AskClientList(dwID, &stHead);
	}
	break;
	case eMsgSettingStudentIP:
	{
		m_pImp->SetStudentClientIP(dwID, vParam);
	}
	break;
	case eMsgFileTransmit:
	case eMsgQuestion:
	{
		m_pImp->TransmitFileToClient(dwID, vParam);
	}
	break;
	default:	// Ĭ�ϵ�ת��������ϢԴ�����������ͻ���
	{
		m_pImp->TransmitMsgHeadToClient(stHead, dwID);
	}
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
	m_pImp->DisconnectCallBack(dwID);
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