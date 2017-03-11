#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/CoreDefine.h"

#include <windows.h>

#include <iostream>
using namespace std;

CMsgHelperMain::CMsgHelperMain()
	: m_hWnd(NULL)
{
}


CMsgHelperMain::~CMsgHelperMain()
{
	m_hWnd = NULL;
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
	case eMsgConnectResult:
	{
		ST_MsgHead msg;
		memcpy(&msg, vParam, sizeof(ST_MsgHead));
		bool bSuccess = msg.clientType == 0;

		SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, bSuccess, stHead.msgType);
	}
	break;
	case eMsgRegResult:	// ��½������Ϣ
	{
		ST_MsgRegResult msg;
		memcpy(&msg, vParam, sizeof(ST_MsgRegResult));
		if (msg.bSuccess)
		{
			SendMessage(m_hWnd, WM_COMMAND, IDOK, 0);
		}
		else
		{
			AfxMessageBox(_T("ע��ʧ�ܣ��˺��Ѿ�����"));
		}
	}
	break;
	case eMsgLoginResult:	// ��½������Ϣ
	{
		ST_MsgLoginResult msg;
		memcpy(&msg, vParam, sizeof(ST_MsgLoginResult));
		if (msg.bSuccess)
		{
			SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, 0, stHead.msgType);
		}
		else
		{
			AfxMessageBox(_T("��½ʧ�ܣ��˺Ż�����������ʹ���"));
		}
	}
	break;
	case eMsgAskClientListResult:	// ��ȡ�б�����Ϣ
	{
		ST_MsgAskClientListResult msg;
		memcpy(&msg, vParam, sizeof(ST_MsgAskClientListResult));
		SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, (WPARAM)&msg, stHead.msgType);
	}
	break;
	case eMsgLockScreen:	// ����
	case eMsgUnLockScreen:	// ����
	{
		SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, 0, stHead.msgType);
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