#pragma once
/***************************************************************************************/
/**
	�ļ�����:	MsgInfo.h	
	����ʱ��:	2016/10/21	19:59:06
	��    ��:	ywyonui
	����˵��:	
**/
/***************************************************************************************/
#include "EnumType.h"
#include "UserInfo.h"

/**
	��    �� :	��Ϣͷ�����
	����˵�� :	���ڱ�ʶ�ͻ����������֮�䴫�����ݵ����ͼ�����˵����Ϣ
*/
struct ST_MsgHead
{
	EMsgType msgType;
};

/**
	��    �� :	��½��Ϣ
	����˵�� :
*/
struct ST_MsgLogin
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	ST_LoginUserInfo stLoginInfo;	// ��½ʱ¼����û���Ϣ


	ST_MsgLogin()
	{
		stMsgHead.msgType = eLogin;
	}
};


/**
	��    �� :	��½��ķ�����Ϣ
	����˵�� :	��½֮�󷵻����ݣ���������·���һ���û���Ϣ���ɣ�
*/
struct ST_MsgLoginResult
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	bool		bSuccess;	// �Ƿ��½�ɹ�
	ST_ShowUserInfo	stShowUserInfo;	// ������ʾ���û���Ϣ

	ST_MsgLoginResult()
	{
		stMsgHead.msgType = eLoginResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
��    �� :	ע����Ϣ
����˵�� :
*/
struct ST_MsgReg
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	ST_RegUserInfo stRegInfo;	// ע��¼����û���Ϣ

	ST_MsgReg()
	{
		stMsgHead.msgType = eReg;
	}

};

/**
��    �� :	��½��ķ�����Ϣ
����˵�� :	��½֮�󷵻����ݣ���������·���һ���û���Ϣ���ɣ�
*/
struct ST_MsgRegResult
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	bool		bSuccess;	// �Ƿ��½�ɹ�

	ST_MsgRegResult()
	{
		stMsgHead.msgType = eRegResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
��    �� :	������Ϸ
����˵�� :
*/
struct ST_MsgEnterGame
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	ST_ShowUserInfo stShowUserInfo;	// ��ǰ�û�����Ϣ

	ST_MsgEnterGame()
	{
		stMsgHead.msgType = eEnterGame;
	}
};


