/***************************************************************************************/
/**
	文件名称:	DBMySQL.cpp	
	创建时间:	2016/10/19	21:23:23
	作    者:	ywyonui
	功能说明:	
**/
/***************************************************************************************/
#include "stdafx.h"

#include <string>

#include "DBMySQL.h"


CDBMySQL::CDBMySQL()
{
}


CDBMySQL::~CDBMySQL()
{
}

#pragma region 公共接口
/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CDBMySQL:	服务器消息处理对象
功能说明:	返回单例对象
*************************************************************/
CDBMySQL& CDBMySQL::GetInstance()
{
	static CDBMySQL m_hInstance;
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
			strReturnMsg:	返回连接信息
返 回 值:	int:	返回连接状态
功能说明:	连接数据库
*************************************************************/
int CDBMySQL::ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName,
	const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet,
	std::string& strReturnMsg)
{
	if (mysql_init(&m_MySQL) == NULL)
	{
		strReturnMsg = "inital mysql handle error";
		return 1;
	}

	if (mysql_real_connect(&m_MySQL, strIP.c_str(), strUserName.c_str(), strUserPwd.c_str(), strDBName.c_str(), nPort, NULL, 0) == NULL)
	{
		strReturnMsg = "Failed to connect to database: Error";
		return 1;
	}

	if (mysql_set_character_set(&m_MySQL, strCharSet.c_str()) == NULL)
	{
		strReturnMsg = "mysql_set_character_set Error";
		return 1;
	}

	return 0;
}

/*************************************************************
函数名称:	DisConnMySQL
参数说明:	void
返 回 值:	void
功能说明:	断开数据库连接
*************************************************************/
void CDBMySQL::DisConnMySQL()
{
	mysql_close(&m_MySQL);
}

/*************************************************************
函数名称:	InsertData
参数说明:	strSQL:					SQ语句
			strReturnMsg（OUT）:	返回连接信息
返 回 值:	bool:	返回结果状态
功能说明:	增加数据
*************************************************************/
bool CDBMySQL::InsertData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Insert Data Error";
		return false;
	}

	return true;
}

/*************************************************************
函数名称:	DelData
参数说明:	strSQL:					SQ语句
			strReturnMsg（OUT）:	返回连接信息
返 回 值:	bool:	返回结果状态
功能说明:	删除数据
*************************************************************/
bool CDBMySQL::DelData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Del Data Error";
		return false;
	}

	return true;
}

/*************************************************************
函数名称:	UpdateData
参数说明:	strSQL:					SQ语句
			strReturnMsg（OUT）:	返回连接信息
返 回 值:	bool:	返回结果状态
功能说明:	更新数据
*************************************************************/
bool CDBMySQL::UpdateData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Update Data Error";
		return false;
	}

	return true;
}

/*************************************************************
函数名称:	SelectData
参数说明:	strSQL:					SQ语句
			vecResData（OUT）:		查询出来的所有数据都以字符串的形式返回
			nCol:					查询的字段数量
			strReturnMsg（OUT）:	返回连接信息
返 回 值:	bool:	是否查询到数据了
功能说明:	查询数据库
*************************************************************/
bool CDBMySQL::SelectData(const std::string& strSQL, std::vector<std::string>& vecResData, const int nCol, std::string& strReturnMsg)
{
	MYSQL_ROW mRow;
	MYSQL_RES *mRes;

	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "select ps_info Error";
		return false;
	}
	mRes = mysql_store_result(&m_MySQL);

	if (mRes == NULL)
	{
		strReturnMsg = "select username Error";
		return false;
	}
	while (mRow = mysql_fetch_row(mRes))
	{
		for (int i = 0; i < nCol; i++)
		{
			if (mRow[i]) 
			{
				vecResData.push_back(mRow[i]);
			}
		}
	}

	mysql_free_result(mRes);

	return true;
}

#pragma endregion 公共接口


#pragma region 私有接口


#pragma endregion 私有接口
