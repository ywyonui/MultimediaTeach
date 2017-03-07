#pragma once
/***************************************************************************************/
/**
	文件名称:	MsgInfo.h	
	创建时间:	2016/10/21	19:59:06
	作    者:	ywyonui
	功能说明:	
**/
/***************************************************************************************/
#include "EnumType.h"
#include "UserInfo.h"

/**
	类    型 :	消息头部标记
	功能说明 :	用于标识客户端与服务器之间传递数据的类型及其他说明信息
*/
struct ST_MsgHead
{
	EMsgType msgType;
	EClientType	clientType;
};

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
		stMsgHead.msgType = eLogin;
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
		stMsgHead.msgType = eLoginResult;
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
		stMsgHead.msgType = eReg;
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
		stMsgHead.msgType = eRegResult;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////




