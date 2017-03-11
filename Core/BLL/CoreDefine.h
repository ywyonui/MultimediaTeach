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

#define MAX_STUDENT_CLIENT 10

#pragma region ö������
/**
	��    �� :	�ͻ�������
	����˵�� :	���ڽ��ս�ʦ�����ݺ�ѧ�������ݵ����ִ���
*/
enum EClientType
{
	eClientToServerError = -1,
	eClientToServerSuccess = 0,
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

	eMsgAskClientList,	// �����ȡ�ͻ����б���Ϣ
	eMsgAskClientListResult,	// ��������

	eMsgSettingStudentIP
};

#pragma endregion ö������

#pragma region ����ģ��֪ͨ�������Ϣ
enum EUIMessage
{
	EWND_MSG_CLIENT_RECV = WM_USER + 101,	// ��½�ɹ���ķ�����Ϣ��WPARAM�ǵ�½�ɹ��󷵻ص�����

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
	EClientType	clientType;	// ����ǿͻ��˵�����������ʾ�ͻ������ͣ��ӷ��������ͻ������ʾִ��״̬�Ƿ�ɹ�

	ST_MsgHead()
	{
		msgType = eMsgLogin;
		clientType = eClientToServerSuccess;
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

/**
��    �� :	��½��ķ�����Ϣ
����˵�� :	��½֮�󷵻����ݣ���������·���һ���û���Ϣ���ɣ�
*/
struct ST_MsgAskClientListResult
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	ST_ClientInfo arrClient[MAX_STUDENT_CLIENT + 1];	// Ĭ�����MAX_STUDENT_CLIENT���ͻ���
	int			nSize;

	ST_MsgAskClientListResult()
	{
		stMsgHead.msgType = eMsgAskClientListResult;
		nSize = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
��    �� :	��½��ķ�����Ϣ
����˵�� :	��½֮�󷵻����ݣ���������·���һ���û���Ϣ���ɣ�
*/
struct ST_MsgSettingStudentIP
{
	ST_MsgHead	stMsgHead;	// ��Ϣͷ����¼���ͼ����������Ϣ
	char arrClient[MAX_STUDENT_CLIENT][16];	// Ĭ�����MAX_STUDENT_CLIENT���ͻ���
	int			nSize;

	ST_MsgSettingStudentIP()
	{
		stMsgHead.msgType = eMsgSettingStudentIP;
		nSize = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma endregion ���ݽ����õĽṹ��

#pragma region ͨ�ú���

#pragma endregion ͨ�ú���

