#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"

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
	case eReg:
	{
		ST_MsgReg msg;
		memcpy(&msg, vParam, sizeof(ST_MsgReg));
		ST_MsgRegResult msgRes;
		msgRes.bSuccess = CUserMgr::GetInstance().RegUser(msg.stRegInfo, stHead.clientType);
		CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgRegResult));
	}
	break;
	case eLogin:	// ��½��Ϣ
	{
		ST_MsgLogin msg;
		memcpy(&msg, vParam, sizeof(ST_MsgLogin));
		ST_MsgLoginResult msgRes;
		msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo);
		CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));
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