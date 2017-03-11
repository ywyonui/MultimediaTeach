#pragma once
/***************************************************************************************/
/**
文件名称:	CoreDefine.h
创建时间:	2016/10/21	19:43:38
作    者:	ywyonui
功能说明:	核心类型定义文件，用于定义所有用到的业务相关的类型
**/
/***************************************************************************************/
#include <windows.h>
#include <string>

#define MAX_STUDENT_CLIENT 10

#pragma region 枚举类型
/**
	类    型 :	客户端类型
	功能说明 :	用于接收教师端数据和学生端数据的区分处理
*/
enum EClientType
{
	eClientToServerError = -1,
	eClientToServerSuccess = 0,
	eTeacher = 1,	// 教师端
	eStudent = 2,	// 学生端
};

/**
	类    型 :	客户端状态
	功能说明 :	
*/
enum EClientStatus
{
	eClientDisConnect = 0,	// 未连接
	eClientConnect,			// 连接中，未登录
	eClientLogin			// 登陆成功
};


/**	
	类    型 :	消息类型的枚举
	功能说明 :	定义客户端与服务器之间的类型，类型通常两两对应，
				客户端到服务器（eXXX），服务器到客户端（eXXXResult）
*/
enum EMsgType
{
	eMsgConnect,	// 连接消息，初始化成功后，通知服务器，连接成功，并且传输对应的IP地址之类的数据
	eMsgConnectResult,	// 连接返回
	eMsgDisConnect,	// 断开连接
	eMsgReg,		// 注册
	eMsgRegResult,	// 注册返回消息
	eMsgLogin,			// 登陆
	eMsgLoginResult,	// 登陆返回消息

	eMsgAskClientList,	// 申请获取客户端列表信息
	eMsgAskClientListResult,	// 返回申请

	eMsgSettingStudentIP
};

#pragma endregion 枚举类型

#pragma region 网络模块通知界面的消息
enum EUIMessage
{
	EWND_MSG_CLIENT_RECV = WM_USER + 101,	// 登陆成功后的返回消息，WPARAM是登陆成功后返回的数据

	EWND_MSG_SERVER_RECV = WM_USER + 2001	// 服务器接收到数据，传给界面显示
};
#pragma endregion 网络模块通知界面的消息

#pragma region 数据交互用的结构体

/**
类    型 :	登陆信息
功能说明 :	用户名和密码
*/
struct ST_LoginUserInfo
{
	char	strUserName[30];	//	用户名
	char	strUserPwd[30];		//	密码

	ST_LoginUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strUserPwd, 0, 30);
	}

};

/**
类    型 :	注册用户的信息
功能说明 :	用户名、昵称、密码。。。
*/
struct ST_RegUserInfo
{
	char	strUserName[30];	//	用户名
	char	strPetName[50];		//	昵称
	char	strUserPwd[30];		//	密码

	ST_RegUserInfo()
	{
		memset(strUserName, 0, 30);
		memset(strPetName, 0, 50);
		memset(strUserPwd, 0, 30);
	}
};


/**
类    型 :	客户端信息
功能说明 :	当某个客户端连接上服务器之后应该主动发送连接消息，将基本的IP等信息交给服务器保存
*/
struct ST_ClientInfo
{
	DWORD		dwSocket;
	char		arrIP[16];	// IP地址
	EClientType		eCT;	// 客户端类型	
	EClientStatus	eCS;	// 客户端状态信息
};

/**
类    型 :	消息头部标记
功能说明 :	用于标识客户端与服务器之间传递数据的类型及其他说明信息
*/
struct ST_MsgHead
{
	EMsgType msgType;
	EClientType	clientType;	// 如果是客户端到服务器，表示客户端类型，从服务器到客户端则表示执行状态是否成功

	ST_MsgHead()
	{
		msgType = eMsgLogin;
		clientType = eClientToServerSuccess;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
类    型 :	连接消息
功能说明 :	当某个客户端连接上服务器之后应该主动发送连接消息，将基本的IP等信息交给服务器保存
*/
struct ST_MsgConnect
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	char		arrIP[16];	// IP地址

	ST_MsgConnect()
	{
		stMsgHead.msgType = eMsgConnect;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
类    型 :	登陆消息
功能说明 :
*/
struct ST_MsgLogin
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	ST_LoginUserInfo stLoginInfo;	// 登陆时录入的用户信息

	ST_MsgLogin()
	{
		stMsgHead.msgType = eMsgLogin;
	}
};

/**
类    型 :	登陆后的返回消息
功能说明 :	登陆之后返回数据，正常情况下返回一个用户信息即可，
*/
struct ST_MsgLoginResult
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	bool		bSuccess;	// 是否登陆成功

	ST_MsgLoginResult()
	{
		stMsgHead.msgType = eMsgLoginResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
类    型 :	注册消息
功能说明 :
*/
struct ST_MsgReg
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	ST_RegUserInfo stRegInfo;	// 注册录入的用户信息

	ST_MsgReg()
	{
		stMsgHead.msgType = eMsgReg;
	}
};

/**
类    型 :	登陆后的返回消息
功能说明 :	登陆之后返回数据，正常情况下返回一个用户信息即可，
*/
struct ST_MsgRegResult
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	bool		bSuccess;	// 是否登陆成功

	ST_MsgRegResult()
	{
		stMsgHead.msgType = eMsgRegResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
类    型 :	登陆后的返回消息
功能说明 :	登陆之后返回数据，正常情况下返回一个用户信息即可，
*/
struct ST_MsgAskClientListResult
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	ST_ClientInfo arrClient[MAX_STUDENT_CLIENT + 1];	// 默认最大MAX_STUDENT_CLIENT个客户端
	int			nSize;

	ST_MsgAskClientListResult()
	{
		stMsgHead.msgType = eMsgAskClientListResult;
		nSize = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
类    型 :	登陆后的返回消息
功能说明 :	登陆之后返回数据，正常情况下返回一个用户信息即可，
*/
struct ST_MsgSettingStudentIP
{
	ST_MsgHead	stMsgHead;	// 消息头，记录类型及其他相关信息
	char arrClient[MAX_STUDENT_CLIENT][16];	// 默认最大MAX_STUDENT_CLIENT个客户端
	int			nSize;

	ST_MsgSettingStudentIP()
	{
		stMsgHead.msgType = eMsgSettingStudentIP;
		nSize = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma endregion 数据交互用的结构体

#pragma region 通用函数

#pragma endregion 通用函数

