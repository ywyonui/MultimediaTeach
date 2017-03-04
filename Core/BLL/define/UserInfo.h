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
	类    型 :	用户显示信息
	功能说明 :	用户名、昵称、金币等。。。
*/
struct ST_ShowUserInfo
{
	int		nUserID;			//	用户ID
	char	strUserName[30];	//	用户名
	char	strPetName[50];		//	昵称
	int		nMoney;				//	金币
	int		nIndexDesk;			//  桌子号
	int		nIndexSeat;			//  座位号
	long	nSocketID;			//	记录当前客户端的SocketID，登陆的时候保存

	ST_ShowUserInfo()
	{
		nUserID = 0;
		memset(strUserName, 0, 30);
		memset(strPetName, 0, 50);
		nMoney = 0;
		nIndexDesk = -1;
		nIndexSeat = 0;
		nSocketID = 0;
	}
};


