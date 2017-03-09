#pragma once
/***************************************************************************************/
/**
�ļ�����:	CoreDefine.h
����ʱ��:	2016/10/21	19:43:38
��    ��:	ywyonui
����˵��:	�������Ͷ����ļ������ڶ��������õ���ҵ����ص�����
**/
/***************************************************************************************/
#include <windows.h>
#include <string>

#pragma region ö������
/**
	��    �� :	�ͻ�������
	����˵�� :	���ڽ��ս�ʦ�����ݺ�ѧ�������ݵ����ִ���
*/
enum EClientType
{
	eTeacher = 1,	// ��ʦ��
	eStudent = 2,	// ѧ����
};

/**
	��    �� :	�ͻ���״̬
	����˵�� :	
*/
enum EClientStatus
{
	eClientDisConnect = 0,	// δ����
	eClientConnect,			// �����У�δ��¼
	eClientLogin			// ��½�ɹ�
};


/**	
	��    �� :	��Ϣ���͵�ö��
	����˵�� :	����ͻ����������֮������ͣ�����ͨ��������Ӧ��
				�ͻ��˵���������eXXX�������������ͻ��ˣ�eXXXResult��
*/
enum EMsgType
{
	eMsgConnect,	// ������Ϣ����ʼ���ɹ���֪ͨ�����������ӳɹ������Ҵ����Ӧ��IP��ַ֮�������
	eMsgConnectResult,	// ���ӷ���
	eMsgDisConnect,	// �Ͽ�����
	eMsgReg,		// ע��
	eMsgRegResult,	// ע�᷵����Ϣ
	eMsgLogin,			// ��½
	eMsgLoginResult,	// ��½������Ϣ
};


/**
	��    �� :	��������
	����˵�� :	����һЩ�������ͣ������жϺ�������ֵ
*/
enum EResultType
{
	eOK = 0,			// ��������
	eLoginError = -1,	// ��½ʧ��
};



#pragma endregion ö������

#pragma region ����ģ��֪ͨ�������Ϣ
enum EUIMessage
{
	EWND_MSG_LOGIN_SUCCESS = WM_USER + 101,	// ��½�ɹ���ķ�����Ϣ��WPARAM�ǵ�½�ɹ��󷵻ص�����

	EWND_MSG_SERVER_RECV = WM_USER + 2001	// ���������յ����ݣ�����������ʾ
};
#pragma endregion ����ģ��֪ͨ�������Ϣ

#pragma region ���ݽ����õĽṹ��

/**
��    �� :	��½��Ϣ
����˵�� :	�û���������
*/
struct ST_LoginUserInfo
{
	char	strUserName[30];	//	�û���
	char	strUserPwd[30];		//	����

	ST_LoginUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strUserPwd, 0, 30);
	}

};

/**
��    �� :	ע���û�����Ϣ
����˵�� :	�û������ǳơ����롣����
*/
struct ST_RegUserInfo
{
	char	strUserName[30];	//	�û���
	char	strPetName[50];		//	�ǳ�
	char	strUserPwd[30];		//	����

	ST_RegUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strPetName, 0, 50);
		memset(strUserPwd, 0, 30);
	}
};


/**
��    �� :	�ͻ�����Ϣ
����˵�� :	��ĳ���ͻ��������Ϸ�����֮��Ӧ����������������Ϣ����������IP����Ϣ��������������
*/
struct ST_ClientInfo
{
	DWORD		dwSocket;
	char		arrIP[16];	// IP��ַ
	EClientType		eCT;	// �ͻ�������	
	EClientStatus	eCS;	// �ͻ���״̬��Ϣ
};

/**
��    �� :	��Ϣͷ�����
����˵�� :	���ڱ�ʶ�ͻ����������֮�䴫�����ݵ����ͼ�����˵����Ϣ
*/
struct ST_MsgHead
{
	EMsgType msgType;
	EClientType	clientType;

	ST_MsgHead()
	{
		msgType = eMsgLogin;
		clientType = eStudent;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
��    �� :	������Ϣ
����˵�� :	��ĳ���ͻ��������Ϸ�����֮��Ӧ����������������Ϣ����������IP����Ϣ��������������
*/
struct ST_MsgConnect
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	char		arrIP[16];	// IP��ַ

	ST_MsgConnect()
	{
		stMsgHead.msgType = eMsgConnect;
	}
};

/**
��    �� :	���ӷ���
����˵�� :	
*/
struct ST_MsgConnectResult
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	bool bSuccess;

	ST_MsgConnectResult()
	{
		stMsgHead.msgType = eMsgConnectResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		stMsgHead.msgType = eMsgLogin;
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

	ST_MsgLoginResult()
	{
		stMsgHead.msgType = eMsgLoginResult;
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
		stMsgHead.msgType = eMsgReg;
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
		stMsgHead.msgType = eMsgRegResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////




#pragma endregion ���ݽ����õĽṹ��

#pragma region ͨ�ú���

#pragma endregion ͨ�ú���

