#include "stdafx.h"
#include "TCPNet.h"

#include <Windows.h>

#include <iostream>
using namespace std;

#include "cstdio"

#pragma region 结构体
/**	 	
	功能说明 :	用于交互网络的完成端口和回调函数	
*/
struct ST_ServerData
{
	HANDLE completionPort;
	CBaseNetMsgHelper* pMsgHelper;
};


/**
	功能说明 :	用于注册完成端口函数时的结构体
*/
typedef struct
{
	WSAOVERLAPPED overLap;
	WSABUF Buffer;
	char szMessage[MAX_MSG_SIZE];
	DWORD dwRecvedLen;
	DWORD dwFlags;
} IO_OPERATION_DATA, *LP_IO_OPERATION_DATA;

#pragma endregion 结构体


CTCPNet::CTCPNet()
	: m_ServerSocket(NULL),
	m_ClientSocket(NULL)
{

}


CTCPNet::~CTCPNet()
{
}

#pragma region 公共接口
/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CTCPNet：网络对象
功能说明:	返回单例对象
*************************************************************/
CTCPNet& CTCPNet::GetInstance(void)
{
	static CTCPNet m_hInstance;
	return m_hInstance;
}

/*************************************************************
函数名称:	InitNet
参数说明:	uPort:	端口号,
			pMsgHelper:	消息处理对象
			bServer:	判断是不是服务器
			pAddr:	网址
返 回 值:	bool:	初始化成功还是失败
功能说明:	初始化网络环境
*************************************************************/
bool CTCPNet::InitNet(unsigned uPort, CBaseNetMsgHelper* pMsgHelper, bool bServer /*= true*/, char* pAddr /*= NULL*/)
	{

		if (bServer)
		{
			// 如果是服务器
			// 初始化网络 -> 创建SOCKET -> 绑定（接收连接的IP,端口）-> 监听 -> 接收连接 -> 消息
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			// 1、创建完成端口
			HANDLE comletionPort = INVALID_HANDLE_VALUE;	// 完成端口
			comletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

			// 创建CPU数的线程数
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);

			ST_ServerData stServer;
			stServer.completionPort = comletionPort;
			stServer.pMsgHelper = pMsgHelper;

			for (int i = 0; i < (int)systemInfo.dwNumberOfProcessors; ++i)
			{
				CreateThread(NULL, 0, ServerWorkerThread, (void*)&stServer, 0, 0);
			}

			// 2、创建SOCKET
			SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (-1 == sServer)
			{
				cout << "创建网络环境失败" << endl;
				return false;
			}

			m_ServerSocket = sServer;

			// 3、绑定
			SOCKADDR_IN addr;
			memset(&addr, 0, sizeof(SOCKADDR_IN));
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(uPort);

			int ret = bind(sServer, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
			if (-1 == ret)
			{
				cout << "绑定套接字和地址端口失败" << endl;
				return false;
			}

			// 4、监听
			listen(sServer, 5);

			// 5、接收连接，因为随时可能有玩家连接，所以要不停的接收连接，所以创建线程接收连接
			CreateThread(0, 0, AcceptConnectThread, (void*)comletionPort, 0, 0);

		}
		else
		{
			// 如果是客户端
			// 初始化网络 -> 创建SOCKET -> 绑定（接收连接的IP,端口）-> 监听 -> 接收连接 -> 消息
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			// 2、创建SOCKET
			SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (-1 == sClient)
			{
				cout << "创建网络环境失败" << endl;
				return false;
			}

			SOCKADDR_IN addr;
			memset(&addr, 0, sizeof(SOCKADDR_IN));
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = inet_addr(pAddr);
			addr.sin_port = htons(uPort);

			int ret = connect(sClient, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));

			if (SOCKET_ERROR == ret)
			{
				cout << "服务器连接失败" << endl;
				return false;
			}

			m_ClientSocket = sClient;

			// 创建一个线程用来和服务器通信
			CreateThread(0, 0, ClientWorkerThread, (void*)pMsgHelper, 0, 0);
		}

		return true;
	}

/*************************************************************
函数名称:	SendToClient
参数说明:	dwID:	客户端Socket,
vParam:	发送的数据
nLen:	数据长度
返 回 值:	void
功能说明:	服务器发送数据到客户端
*************************************************************/
void CTCPNet::SendToClient(DWORD dwID, void* vParam, int nLen)
{
	if (!m_ServerSocket)
	{
		return;
	}
	send(dwID, (char*)vParam, nLen, 0);
}

/*************************************************************
函数名称:	SendToServer
参数说明:	vParam:	发送的数据
nLen:	数据长度
返 回 值:	void
功能说明:	客户端发送数据到服务器
*************************************************************/
void CTCPNet::SendToServer(void* vParam, int nLen)
{
	if (m_ServerSocket)
	{
		return;
	}
	send(m_ClientSocket, (char*)vParam, nLen, 0);
}

#pragma endregion 公共接口


#pragma region 私有接口
/*************************************************************
函数名称:	RegisterWSARecv
参数说明:	sClient:	客户端的SOCKET
返 回 值:	void
lpPerIOData:	接收数据的对象指针
功能说明:	客户端工作现场，用于客户端处理和服务器的交互
*************************************************************/
void CTCPNet::RegisterWSARecv(DWORD sClient, void* pIOData)
{
	LP_IO_OPERATION_DATA lpPerIOData = (LP_IO_OPERATION_DATA)pIOData;
	memset(lpPerIOData, 0, sizeof(IO_OPERATION_DATA));
	lpPerIOData->Buffer.len = MAX_MSG_SIZE;
	lpPerIOData->Buffer.buf = lpPerIOData->szMessage;

	WSARecv(sClient,
		&lpPerIOData->Buffer,
		1,
		&lpPerIOData->dwRecvedLen,
		&lpPerIOData->dwFlags,
		&lpPerIOData->overLap,
		NULL);
}

/*************************************************************
函数名称:	AcceptConnectThread
参数说明:	vParam:	线程接收数据
返 回 值:	DWORD:	线程执行结果
功能说明:	监听客户端连接情况
*************************************************************/
DWORD WINAPI CTCPNet::AcceptConnectThread(void* vParam)
{
	HANDLE completionPort = (HANDLE)vParam;

	SOCKADDR_IN clientAddr;
	int nLen = sizeof(SOCKADDR_IN);

	while (true)
	{
		// 线程中死循环，接收来自客户端的连接
		SOCKET sClient = accept(GetInstance().m_ServerSocket, (SOCKADDR*)&clientAddr, &nLen);

		// 把新连接过来的SOCKET加入到完成端口中
		CreateIoCompletionPort((HANDLE)sClient, completionPort, (DWORD)sClient, 0);

		// 激活一个WSARecv(代替recv)
		LP_IO_OPERATION_DATA lpPerIOData = new IO_OPERATION_DATA;

		GetInstance().RegisterWSARecv(sClient, lpPerIOData);

	}
	return 0;
}

/*************************************************************
函数名称:	ServerWorkerThread
参数说明:	vParam:	线程接收数据
返 回 值:	DWORD:	线程执行结果
功能说明:	服务器工作线程，用于服务器处理和客户端的交互，
每个客户端对应一个线程
*************************************************************/
DWORD WINAPI CTCPNet::ServerWorkerThread(void* vParam)
{
	ST_ServerData* pStServer = (ST_ServerData*)vParam;

	HANDLE completionPort = pStServer->completionPort;
	CBaseNetMsgHelper* pMsgHelper = pStServer->pMsgHelper;

	LP_IO_OPERATION_DATA lpPerIOData = NULL;

	DWORD dwTransferredLen = 0;
	
	SOCKET sClient;

	while (true)
	{
		if (NULL != lpPerIOData)
		{
			memset(lpPerIOData, 0, sizeof(IO_OPERATION_DATA));
		}

		// 如果没有用完成端口，这个线程就是不停的recv来自客户端的消息
		// 使用了完成端口，直接在消息队列中取消息
		int ret = GetQueuedCompletionStatus(
					completionPort,				// 完成端口
					&dwTransferredLen,			// 消息大小
					(PULONG_PTR)&sClient,		// 套接字
					(LPOVERLAPPED*)&lpPerIOData,	// 消息数据
					INFINITE					// 等待时间
					);

		switch (dwTransferredLen)
		{
			case -1: 
			{

			}
			break;

			case 0: 
			{
				cout << "客户端断开连接" << endl;
				// 在AcceptConnectThread里面new出来的，因此每个客户端断开时delete掉
				delete lpPerIOData;
				lpPerIOData = NULL;
			}
			break;

			default:
			{
				if (pMsgHelper)
				{
					pMsgHelper->NetMsgCallBack(sClient, lpPerIOData->szMessage, dwTransferredLen);
					// 激活一个WSARecv(代替recv)
					GetInstance().RegisterWSARecv(sClient, lpPerIOData);
				}

			}
			break;
		}
	}
}

/*************************************************************
函数名称:	ClientWorkerThread
参数说明:	vParam:	线程接收数据
返 回 值:	DWORD:	线程执行结果
功能说明:	客户端工作现场，用于客户端处理和服务器的交互
*************************************************************/
DWORD WINAPI CTCPNet::ClientWorkerThread(void* vParam)
{
//	ST_ServerData* pStServer = (ST_ServerData*)vParam;

	CBaseNetMsgHelper* pMsgHelper = (CBaseNetMsgHelper*)vParam;

	char szBuff[MAX_MSG_SIZE] = { 0 };

	while (true)
	{
		int nLen = 0;
		int nNum = recv(GetInstance().m_ClientSocket, szBuff, MAX_MSG_SIZE, 0);
		if (SOCKET_ERROR == nNum)
		{
			cout << "和服务器断开连接" << endl;
			break;
		}
		pMsgHelper->NetMsgCallBack(0, szBuff, nLen);
	}

	return 0;
}

#pragma endregion 私有接口


