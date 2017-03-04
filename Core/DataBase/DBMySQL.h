#pragma once
/***************************************************************************************/
/**
	文件名称:	DBMySQL.h	
	创建时间:	2016/10/19	21:22:47
	作    者:	ywyonui
	功能说明:	MySQL数据库访问
**/
/***************************************************************************************/

// 定义宏，用于DLL中的函数导出
#ifndef NET_API
#define NET_API __declspec(dllexport)
#endif

#include <mysql.h>
#include <vector>

/**	
	类    型 :	
	功能说明 :	
*/
class NET_API CDBMySQL
{
private:
	CDBMySQL();
	~CDBMySQL();
	
#pragma region 公共变量
public:

#pragma endregion 公共变量


#pragma region 私有变量
private:
	MYSQL	m_MySQL;	// MySQL数据库对象
#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	GetInstance
	参数说明:	void
	返 回 值:	CDBMySQL:	数据库访问
	功能说明:	返回单例对象
	*************************************************************/
	static CDBMySQL& GetInstance();

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
	函数名称:	InsertData
	参数说明:	strSQL:					SQ语句
				strReturnMsg（OUT）:	返回连接信息
	返 回 值:	bool:	返回结果状态
	功能说明:	增加数据
	*************************************************************/
	bool InsertData(const std::string& strSQL, std::string& strReturnMsg);

	/*************************************************************
	函数名称:	DelData
	参数说明:	strSQL:					SQ语句
				strReturnMsg（OUT）:	返回连接信息
	返 回 值:	bool:	返回结果状态
	功能说明:	删除数据
	*************************************************************/
	bool DelData(const std::string& strSQL, std::string& strReturnMsg);

	/*************************************************************
	函数名称:	UpdateData
	参数说明:	strSQL:					SQ语句
				strReturnMsg（OUT）:	返回连接信息
	返 回 值:	bool:	返回结果状态
	功能说明:	更新数据
	*************************************************************/
	bool UpdateData(const std::string& strSQL, std::string& strReturnMsg);
	
	/*************************************************************
	函数名称:	SelectData
	参数说明:	strSQL:					SQ语句
				nCol:					查询的字段数量
				vecResData（OUT）:		查询出来的所有数据都以字符串的形式返回
				strReturnMsg（OUT）:	返回连接信息
	返 回 值:	bool:	是否查询到数据了
	功能说明:	查询数据库
	*************************************************************/
	bool SelectData(const std::string& strSQL, std::vector<std::string>& vecResData, const int nCol, std::string& strReturnMsg);
#pragma endregion 公共接口


#pragma region 私有接口
private:

#pragma endregion 私有接口

};
