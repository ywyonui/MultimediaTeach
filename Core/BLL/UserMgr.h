#pragma once
/***************************************************************************************/
/**
	文件名称:	UserMgr.h	
	创建时间:	2016/10/22	22:08:08
	作    者:	ywyonui
	功能说明:	
**/
/***************************************************************************************/
#include "BaseBLL.h"
#include "CoreDefine.h"
#include <vector>

/**	
	类    型 :	
	功能说明 :	
*/
class BLL_API CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();
	
#pragma region 公共变量
public:

#pragma endregion 公共变量


#pragma region 私有变量
private:

#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	GetInstance
	参数说明:	void
	返 回 值:	CUserMgr:	用户管理
	功能说明:	返回单例对象
	*************************************************************/
	static CUserMgr& GetInstance();

	/*************************************************************
	函数名称:	RegUser
	参数说明:	stRegUserInfo :	注册的用户信息
	返 回 值:	bool :	是否成功
	功能说明:	注册用户
	*************************************************************/
	bool RegUser(const ST_RegUserInfo& stRegUserInfo, const EClientType& eType);

	/*************************************************************
	函数名称:	Login
	参数说明:	stLoginUserInfo :	登陆的用户信息
	返 回 值:	bool :	是否成功
	功能说明:	登陆，目前登陆成功之后不需要获取返回数据，而是主窗口初始化的时候主动发起数据查询
	*************************************************************/
	bool Login(const ST_LoginUserInfo& stLoginUserInfo, const EClientType& eType);

	/*************************************************************
	函数名称:	AskForClientList
	参数说明:	vecClient（OUT）: 输出参数，返回查询到的用户列表 
	返 回 值:	bool :	是否成功
	功能说明:	查询用户列表，用于记录显示
	*************************************************************/
	bool AskForClientList(std::vector<ST_ClientInfo>& vecClient);

#pragma endregion 公共接口


#pragma region 私有接口
private:

#pragma endregion 私有接口

};

