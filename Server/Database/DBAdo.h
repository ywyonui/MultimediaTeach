#pragma once
/***************************************************************************************/
/**
	文件名称:	DBAdo.h	
	创建时间:	2016/10/19	21:22:47
	作    者:	ywyonui
	功能说明:	MySQL数据库访问
**/
/***************************************************************************************/
#include "DBAdoFwd.h"
#include <string>

/**	
	类    型 :	
	功能说明 :	
*/
class CDBAdoController
{
private:
	CDBAdoController();
	~CDBAdoController();
	
#pragma region 公共变量
public:
	enum EDatabaseProvider
	{
		eAccess2000 = 1,
		eAccess2010,
		eODBC,
		eOracle,
		eSqlServer,
	};


#pragma endregion 公共变量


#pragma region 私有变量
private:
	_ConnectionPtr	m_pConnection;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordset;
#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	GetInstance
	参数说明:	void
	返 回 值:	CDBAdoController:	数据库访问
	功能说明:	返回单例对象
	*************************************************************/
	static CDBAdoController& GetInstance();

	/*************************************************************
	函数名称:	Init
	参数说明:	void
	返 回 值:	bool
	功能说明:	初始化对象
	*************************************************************/
	bool Init();

	/*************************************************************
	函数名称:	Connect
	参数说明:	strConnect:连接使用的字符串
	返 回 值:	bool
	功能说明:	连接数据库
	*************************************************************/
	bool Connect(std::string strConnect);

	/*************************************************************
	函数名称:	Connect
	参数说明:	eDB:数据库类型，
				strDataSource：数据库路径，
				strIP：数据库IP，
				bHasPwd：是否使用账号密码，
				strUserName：数据库用户名，
				strPwd：数据库用户密码
	返 回 值:	bool
	功能说明:	初始化对象
	*************************************************************/
	bool Connect(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd);

	/*************************************************************
	函数名称:	IsConnect
	参数说明:	void
	返 回 值:	bool
	功能说明:	当前是否已经连接
	*************************************************************/
	bool IsConnected() const;

	/*************************************************************
	函数名称:	ExecuteNonQuery
	参数说明:	strCommand: SQL语句
	返 回 值:	int: 影响行数
	功能说明:	执行非select查询语句
	*************************************************************/
	int ExecuteNonQuery(const std::string& strCommand);

	/*************************************************************
	函数名称:	ExecuteReader
	参数说明:	strCommand: SQL语句
	返 回 值:	CDBAdoRecordReader: 返回值容器
	功能说明:	执行select查询语句
	*************************************************************/
	CDBAdoRecordReader ExecuteReader(const std::string& strCommand);
#pragma endregion 公共接口


#pragma region 私有接口
private:
	/*************************************************************
	函数名称:	ConnectStringBuilder
	参数说明:	eDB:数据库类型，
				strDataSource：数据库路径，
				strIP：数据库IP，
				bHasPwd：是否使用账号密码，
				strUserName：数据库用户名，
				strPwd：数据库用户密码
	返 回 值:	string: 连接使用的字符串
	功能说明:	组合连接字符串
	*************************************************************/
	std::string ConnectStringBuilder(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd);

#pragma endregion 私有接口

};

class CDBAdoRecordReader
{
public:
	CDBAdoRecordReader();
	explicit CDBAdoRecordReader(_RecordsetPtr ptr);
	
	//使其引用计数+1
	CDBAdoRecordReader(const CDBAdoRecordReader& lhs);

	//减少引用计数
	~CDBAdoRecordReader();

#pragma region 私有变量
private:
	_RecordsetPtr m_recordSetPtr;
#pragma endregion 私有变量


#pragma region 公共接口
public:
	//移动至下一条记录
	bool MoveNext();

	//移动到第一条记录
	bool MoveFirst();

	//行数
	int RecordCount();

	//列数
	int FieldCount();

	_variant_t operator[](const size_t nIndex) const;
	
	_variant_t operator[](const std::string key) const;
#pragma endregion 公共接口


#pragma region 私有接口

#pragma endregion 私有接口


};