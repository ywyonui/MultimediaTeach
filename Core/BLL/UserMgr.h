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
#include "define/MsgInfo.h"

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
	函数名称:	ConnMySQL
	参数说明:	strIP:			密码
				nPort:			端口
				strDBName:		数据库名称
				strUserName:	用户名
				strUserPwd:		用户密码
				strCharSet:		字符集
				strReturnMsg（OUT）:	返回连接信息
	返 回 值:	int:	返回连接状态
	功能说明:	连接数据库
	*************************************************************/
	int ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName,
		const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet,
		std::string& strReturnMsg);


	/*************************************************************
	函数名称:	DisConnMySQL
	参数说明:	void
	返 回 值:	void
	功能说明:	断开数据库连接
	*************************************************************/
	void DisConnMySQL();

	/*************************************************************
	函数名称:	RegUser
	参数说明:	stRegUserInfo :	注册的用户信息
	返 回 值:	bool :	是否成功
	功能说明:	注册用户
	*************************************************************/
	bool RegUser(const ST_RegUserInfo& stRegUserInfo);

	/*************************************************************
	函数名称:	Login
	参数说明:	stLoginUserInfo :	登陆的用户信息
				stShowUserInfo（OUT）:	返回接收用户的显示信息
	返 回 值:	bool :	是否成功
	功能说明:	登陆
	*************************************************************/
	bool Login(const ST_LoginUserInfo& stLoginUserInfo, ST_ShowUserInfo& stShowUserInfo);

	/*************************************************************
	函数名称:	UpdateMoney
	参数说明:	stShowUserInfo 
	返 回 值:	bool :	是否成功
	功能说明:	更新金币数据
	*************************************************************/
	bool UpdateMoney(const ST_ShowUserInfo& stShowUserInfo);


#pragma endregion 公共接口


#pragma region 私有接口
private:

#pragma endregion 私有接口

};

