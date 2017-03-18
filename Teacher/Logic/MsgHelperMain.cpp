#include "stdafx.h"
#include "MsgHelperMain.h"
#include "BLL/CoreDefine.h"

#include <iostream>
using namespace std;



CMsgHelperMain::CMsgHelperMain()
	: m_dwSocket(NULL)
	, m_hWnd(NULL)
{
}


CMsgHelperMain::~CMsgHelperMain()
{
}


#pragma region �����ӿ�
/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CMsgHelperMain����������Ϣ�������
����˵��:	���ص�������
*************************************************************/
CMsgHelperMain& CMsgHelperMain::GetInstance(void)
{
	static CMsgHelperMain m_hInstance;
	return m_hInstance;
}


/*************************************************************
��������:	NetMsgCallBack
����˵��:	dwID:	�ͻ���Socket
			vParam:	����
			nLen:	���ݳ���
�� �� ֵ:	void
����˵��:	������Ϣ�Ļص������������п�����һЩͨ�õĴ�������Ӧ����������ʵ��
*************************************************************/
void CMsgHelperMain::NetMsgCallBack(DWORD dwID, void* vParam, int nLen)
{
	m_dwSocket = dwID;
	ST_MsgHead stHead;
	memcpy(&stHead, vParam, sizeof(ST_MsgHead));
	switch (stHead.msgType)
	{
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
			SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, 0, eMsgLoginResult);
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
		SendMessage(m_hWnd, EWND_MSG_CLIENT_RECV, (WPARAM)&msg, eMsgAskClientListResult);
	}
	break;
	case eMsgBeginDisplay:	// ���ܵ���ʼ��ʾ
	{

	}
	break;
	case eMsgEndDisplay:	// ���ܵ���ʼ��ʾ
	{

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
/*************************************************************
��������:	GetSocket()
����˵��:	void
�� �� ֵ:
����˵��:	����Socket�������ⲿֱ�ӽ������ݽ���
*************************************************************/
DWORD CMsgHelperMain::GetSocket(void)
{
	return m_dwSocket;
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�