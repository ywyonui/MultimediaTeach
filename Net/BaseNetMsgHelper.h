#pragma once
/***************************************************************************************/
/**
	文件名称:	BaseNetMsgHelper.h	
	创建时间:	2016/10/15	12:59:56
	作    者:	ywyonui
	功能说明:	网络消息处理基类
**/
/***************************************************************************************/
#include <Windows.h>

#pragma region 宏声明

// 定义宏，用于DLL中的函数导出
#ifndef NET_API
#define NET_API __declspec(dllexport)
#endif

#ifndef NET_IMPORT
#define NET_IMPORT __declspec(dllimport)
#endif

// 消息最多尺寸
#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE 1024
#endif

#pragma endregion 宏声明

/**
	类    型 :	基类
	功能说明 :
*/
class NET_API CBaseNetMsgHelper
{
public:
	CBaseNetMsgHelper();
	~CBaseNetMsgHelper();

#pragma region 公共变量
public:

#pragma endregion 公共变量


#pragma region 私有变量
private:
	std::vector<SOCKET>	m_vecSocket;
#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	NetMsgCallBack
	参数说明:	dwID:	客户端Socket
				vParam:	数据
				nLen:	数据长度
	返 回 值:	void
	功能说明:	处理消息的回调函数，基类中可以做一些通用的处理，具体应该由派生类实现
	*************************************************************/
	virtual void NetMsgCallBack(DWORD dwID, void* vParam, int nLen) = 0;

	/*************************************************************
	函数名称:	NetMsgCallBack
	参数说明:	dwID:	客户端Socket,0表示服务器，其他表示服务器接收到客户端断开
	返 回 值:	void
	功能说明:	断开连接是个特殊事件，客户端和服务器处理方式不同，因此留出接口重写，默认版本什么都不做
	*************************************************************/
	virtual void DisconnectCallBack(DWORD dwID);

#pragma endregion 公共接口


#pragma region 私有接口
private:

#pragma endregion 私有接口

};

