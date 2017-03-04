#include "stdafx.h"
#include "UserMgr.h"

#include "DBMySQL.h"

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
int CUserMgr::ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName, 
						const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet, std::string& strReturnMsg)
{
	return CDBMySQL::GetInstance().ConnMySQL(strIP, nPort, strDBName, strUserName, strUserPwd, strCharSet, strReturnMsg);
}

/*************************************************************
函数名称:	DisConnMySQL
参数说明:	void
返 回 值:	void
功能说明:	断开数据库连接
*************************************************************/
void CUserMgr::DisConnMySQL()
{
	CDBMySQL::GetInstance().DisConnMySQL();
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
	sprintf_s(pStrSQL, "select UserName from userinfo where UserName = \"%s\";",
			  stRegUserInfo.strUserName);


	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 1, strReturnMsg);

	// 如果查到了数据，则表示用户名重复了
	if (bSuccess && vecResData.size() > 0)
	{
		return false;
	}
	
	sprintf_s(pStrSQL, "insert into userinfo(UserName, UserPetName, UserPwd) values(\"%s\", \"%s\", md5(\"%s\") );", 
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
bool CUserMgr::Login(const ST_LoginUserInfo& stLoginUserInfo, ST_ShowUserInfo& stShowUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "select * from userinfo where UserName = \"%s\" and UserPwd = md5(\"%s\");",
			  stLoginUserInfo.strUserName, stLoginUserInfo.strUserPwd);

	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 5, strReturnMsg);

	if (!bSuccess)
	{
		return false;
	}
	else
	{
		if (vecResData.size() != 5)
		{
			return false;
		}
		// 将数据读取出来存放到ST_ShowUserInfo中
		stShowUserInfo.nUserID = atoi(vecResData.at(0).c_str());
		memcpy(stShowUserInfo.strUserName, vecResData.at(1).c_str(), vecResData.at(1).size());
		memcpy(stShowUserInfo.strPetName, vecResData.at(2).c_str(), vecResData.at(2).size());
		stShowUserInfo.nMoney = atoi(vecResData.at(4).c_str());
		return true;
	}

}

/*************************************************************
函数名称:	UpdateMoney
参数说明:	stShowUserInfo
返 回 值:	bool :	是否成功
功能说明:	更新金币数据
*************************************************************/
bool CUserMgr::UpdateMoney(const ST_ShowUserInfo& stShowUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "update userinfo set Money = %d where UserID = %d;",
			  stShowUserInfo.nMoney, stShowUserInfo.nUserID);

	std::string strReturn = "";

	return CDBMySQL::GetInstance().UpdateData(pStrSQL, strReturn);
}


#pragma endregion 公共接口


#pragma region 私有接口


#pragma endregion 私有接口

