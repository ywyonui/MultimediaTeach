#include "stdafx.h"
#include "UserMgr.h"

#include "DataBase/DBMySQL.h"

CUserMgr::CUserMgr()
{
}


CUserMgr::~CUserMgr()
{
}


#pragma region 公共接口

/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CUserMgr:	用户管理
功能说明:	返回单例对象
*************************************************************/
CUserMgr& CUserMgr::GetInstance()
{
	static CUserMgr m_hInstance;
	return m_hInstance;
}

/*************************************************************
函数名称:	RegUser
参数说明:	stRegUserInfo :	注册的用户信息
返 回 值:	bool :	是否成功
功能说明:	注册用户
*************************************************************/
bool CUserMgr::RegUser(const ST_RegUserInfo& stRegUserInfo)
{
	char pStrSQL[1024] = { 0 };

	// 1、查询用户名是不是存在
	sprintf_s(pStrSQL, "select UserName from user where UserName = \"%s\";",
			  stRegUserInfo.strUserName);


	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 1, strReturnMsg);

	// 如果查到了数据，则表示用户名重复了
	if (bSuccess && vecResData.size() > 0)
	{
		return false;
	}
	
	sprintf_s(pStrSQL, "insert into user(UserName, UserPetName, UserPwd) values(\"%s\", \"%s\", md5(\"%s\") );", 
			  stRegUserInfo.strUserName, stRegUserInfo.strPetName, stRegUserInfo.strUserPwd);

	std::string strReturn = "";

	return CDBMySQL::GetInstance().InsertData(pStrSQL, strReturn);
}

/*************************************************************
函数名称:	Login
参数说明:	stLoginUserInfo :	登陆的用户信息
			stShowUserInfo（OUT）:	返回接收用户的显示信息
返 回 值:	bool :	是否成功
功能说明:	登陆
*************************************************************/
bool CUserMgr::Login(const ST_LoginUserInfo& stLoginUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "select * from user where UserName = \"%s\" and UserPwd = md5(\"%s\");",
			  stLoginUserInfo.strUserName, stLoginUserInfo.strUserPwd);

	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 5, strReturnMsg);

	if (!bSuccess)
	{
		return false;
	}
	
	return true;
}


/*************************************************************
函数名称:	SearchUserList
参数说明:	list（OUT）: 输出参数，返回查询到的用户列表
返 回 值:	bool :	是否成功
功能说明:	查询用户列表，用于记录显示
*************************************************************/
bool CUserMgr::SearchUserList(OUT int& list)
{
	return true;
}


#pragma endregion 公共接口


#pragma region 私有接口


#pragma endregion 私有接口

