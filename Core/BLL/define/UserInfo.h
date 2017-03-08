#pragma once
/***************************************************************************************/
/**
	文件名称:	UserInfo.h	
	创建时间:	2016/10/21	19:47:07
	作    者:	ywyonui
	功能说明:	用户数据定义，目前这个程序唯一需要和数据库交互的
**/
/***************************************************************************************/

#include <string>

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
	char		arrIP[4];	// IP地址
	EClientType		eCT;	// 客户端类型	
	EClientStatus	eCS;	// 客户端状态信息
};