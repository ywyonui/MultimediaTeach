#pragma once
/*******************************************************/
/**
	文件名称:	TCPNet.h	
	创建时间:	2016/10/13	20:28:27
	作    者:	ywyonui
	功能说明:	定义了一个命名空间，包含网络层，主要定义一个基于TCP的网络管理类
**/
/*******************************************************/

#include "BaseNetMsgHelper.h"
#include <mutex>

#define MAX_NET_BUFFER_SIZE 2048

/**
	类    型 :	单例类，添加NET_API用于导出到DLL中
	功能说明 :	服务器和客户端公用的网络服务类
*/
class NET_API CTCPNet
{
private:
	CTCPNet();
	~CTCPNet();

#pragma region 公共变量
public:

#pragma endregion 公共变量


#pragma region 私有变量
private:
	DWORD m_ServerSocket;
	DWORD m_ClientSocket;

	std::mutex m_mutex;
	CBaseNetMsgHelper*	m_pNetMsgHelper;

#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	GetInstance
	参数说明:	void
	返 回 值:	CTCPNet:	网络对象
	功能说明:	返回单例对象
	*************************************************************/
	static CTCPNet& GetInstance(void);

	/*************************************************************
	函数名称:	InitNet
	参数说明:	uPort:	端口号,
				pMsgHelper:	消息处理对象
				bServer:	判断是不是服务器
				pAddr:	网址
	返 回 值:	bool:	初始化成功还是失败
	功能说明:	初始化网络环境
	*************************************************************/
	bool InitNet(unsigned uPort, CBaseNetMsgHelper* pMsgHelper, bool bServer = true, char* pAddr = NULL);

	/*************************************************************
	函数名称:	SendToClient
	参数说明:	dwID:	客户端Socket,
				vParam:	发送的数据
				nLen:	数据长度
	返 回 值:	void
	功能说明:	服务器发送数据到客户端
	*************************************************************/
	void SendToClient(DWORD dwID, void* vParam, int nLen);


	/*************************************************************
	函数名称:	SendToServer
	参数说明:	vParam:	发送的数据
				nLen:	数据长度
	返 回 值:	void
	功能说明:	客户端发送数据到服务器
	*************************************************************/
	void SendToServer(void* vParam, int nLen);

#pragma endregion 公共接口


#pragma region 私有接口
private:
	/*************************************************************
	函数名称:	RegisterWSARecv
	参数说明:	sClient:	客户端的SOCKET
	返 回 值:	void
	lpPerIOData:	接收数据的对象指针
	功能说明:	客户端工作现场，用于客户端处理和服务器的交互
	*************************************************************/
	void RegisterWSARecv(DWORD sClient, void* lpPerIOData);

	/*************************************************************
	函数名称:	AcceptConnectThread
	参数说明:	vParam:	线程接收数据
	返 回 值:	DWORD:	线程执行结果
	功能说明:	监听客户端连接情况
	*************************************************************/
	static DWORD WINAPI AcceptConnectThread(void* vParam);

	/*************************************************************
	函数名称:	ServerWorkerThread
	参数说明:	vParam:	线程接收数据
	返 回 值:	DWORD:	线程执行结果	
	功能说明:	服务器工作线程，用于服务器处理和客户端的交互，
				每个客户端对应一个线程
	*************************************************************/
	static DWORD WINAPI ServerWorkerThread(void* vParam);

	/*************************************************************
	函数名称:	ClientWorkerThread
	参数说明:	vParam:	线程接收数据
	返 回 值:	DWORD:	线程执行结果
	功能说明:	客户端工作现场，用于客户端处理和服务器的交互
	*************************************************************/
	static DWORD WINAPI ClientWorkerThread(void* vParam);

#pragma endregion 私有接口

};




