#include "stdafx.h"
#include "TCPNet.h"

#include <Windows.h>

#include <iostream>
using namespace std;

#include "cstdio"

#pragma region �ṹ��
/**	 	
	����˵�� :	���ڽ����������ɶ˿ںͻص�����	
*/
struct ST_ServerData
{
	HANDLE completionPort;
	CBaseNetMsgHelper* pMsgHelper;
};


/**
	����˵�� :	����ע����ɶ˿ں���ʱ�Ľṹ��
*/
typedef struct
{
	WSAOVERLAPPED overLap;
	WSABUF Buffer;
	char szMessage[MAX_MSG_SIZE];
	DWORD dwRecvedLen;
	DWORD dwFlags;
} IO_OPERATION_DATA, *LP_IO_OPERATION_DATA;

#pragma endregion �ṹ��


CTCPNet::CTCPNet()
	: m_ServerSocket(NULL),
	m_ClientSocket(NULL)
{

}


CTCPNet::~CTCPNet()
{
}

#pragma region �����ӿ�
/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CTCPNet���������
����˵��:	���ص�������
*************************************************************/
CTCPNet& CTCPNet::GetInstance(void)
{
	static CTCPNet m_hInstance;
	return m_hInstance;
}

/*************************************************************
��������:	InitNet
����˵��:	uPort:	�˿ں�,
			pMsgHelper:	��Ϣ�������
			bServer:	�ж��ǲ��Ƿ�����
			pAddr:	��ַ
�� �� ֵ:	bool:	��ʼ���ɹ�����ʧ��
����˵��:	��ʼ�����绷��
*************************************************************/
bool CTCPNet::InitNet(unsigned uPort, CBaseNetMsgHelper* pMsgHelper, bool bServer /*= true*/, char* pAddr /*= NULL*/)
	{

		if (bServer)
		{
			// ����Ƿ�����
			// ��ʼ������ -> ����SOCKET -> �󶨣��������ӵ�IP,�˿ڣ�-> ���� -> �������� -> ��Ϣ
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			// 1��������ɶ˿�
			HANDLE comletionPort = INVALID_HANDLE_VALUE;	// ��ɶ˿�
			comletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

			// ����CPU�����߳���
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);

			ST_ServerData stServer;
			stServer.completionPort = comletionPort;
			stServer.pMsgHelper = pMsgHelper;

			for (int i = 0; i < (int)systemInfo.dwNumberOfProcessors; ++i)
			{
				CreateThread(NULL, 0, ServerWorkerThread, (void*)&stServer, 0, 0);
			}

			// 2������SOCKET
			SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (-1 == sServer)
			{
				cout << "�������绷��ʧ��" << endl;
				return false;
			}

			m_ServerSocket = sServer;

			// 3����
			SOCKADDR_IN addr;
			memset(&addr, 0, sizeof(SOCKADDR_IN));
			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(uPort);

			int ret = bind(sServer, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
			if (-1 == ret)
			{
				cout << "���׽��ֺ͵�ַ�˿�ʧ��" << endl;
				return false;
			}

			// 4������
			listen(sServer, 5);

			// 5���������ӣ���Ϊ��ʱ������������ӣ�����Ҫ��ͣ�Ľ������ӣ����Դ����߳̽�������
			CreateThread(0, 0, AcceptConnectThread, (void*)comletionPort, 0, 0);

		}
		else
		{
			// ����ǿͻ���
			// ��ʼ������ -> ����SOCKET -> �󶨣��������ӵ�IP,�˿ڣ�-> ���� -> �������� -> ��Ϣ
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);

			// 2������SOCKET
			SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (-1 == sClient)
			{
				cout << "�������绷��ʧ��" << endl;
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
				cout << "����������ʧ��" << endl;
				return false;
			}

			m_ClientSocket = sClient;

			// ����һ���߳������ͷ�����ͨ��
			CreateThread(0, 0, ClientWorkerThread, (void*)pMsgHelper, 0, 0);
		}

		return true;
	}

/*************************************************************
��������:	SendToClient
����˵��:	dwID:	�ͻ���Socket,
vParam:	���͵�����
nLen:	���ݳ���
�� �� ֵ:	void
����˵��:	�������������ݵ��ͻ���
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
��������:	SendToServer
����˵��:	vParam:	���͵�����
nLen:	���ݳ���
�� �� ֵ:	void
����˵��:	�ͻ��˷������ݵ�������
*************************************************************/
void CTCPNet::SendToServer(void* vParam, int nLen)
{
	if (m_ServerSocket)
	{
		return;
	}
	send(m_ClientSocket, (char*)vParam, nLen, 0);
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
/*************************************************************
��������:	RegisterWSARecv
����˵��:	sClient:	�ͻ��˵�SOCKET
�� �� ֵ:	void
lpPerIOData:	�������ݵĶ���ָ��
����˵��:	�ͻ��˹����ֳ������ڿͻ��˴���ͷ������Ľ���
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
��������:	AcceptConnectThread
����˵��:	vParam:	�߳̽�������
�� �� ֵ:	DWORD:	�߳�ִ�н��
����˵��:	�����ͻ����������
*************************************************************/
DWORD WINAPI CTCPNet::AcceptConnectThread(void* vParam)
{
	HANDLE completionPort = (HANDLE)vParam;

	SOCKADDR_IN clientAddr;
	int nLen = sizeof(SOCKADDR_IN);

	while (true)
	{
		// �߳�����ѭ�����������Կͻ��˵�����
		SOCKET sClient = accept(GetInstance().m_ServerSocket, (SOCKADDR*)&clientAddr, &nLen);

		// �������ӹ�����SOCKET���뵽��ɶ˿���
		CreateIoCompletionPort((HANDLE)sClient, completionPort, (DWORD)sClient, 0);

		// ����һ��WSARecv(����recv)
		LP_IO_OPERATION_DATA lpPerIOData = new IO_OPERATION_DATA;

		GetInstance().RegisterWSARecv(sClient, lpPerIOData);

	}
	return 0;
}

/*************************************************************
��������:	ServerWorkerThread
����˵��:	vParam:	�߳̽�������
�� �� ֵ:	DWORD:	�߳�ִ�н��
����˵��:	�����������̣߳����ڷ���������Ϳͻ��˵Ľ�����
ÿ���ͻ��˶�Ӧһ���߳�
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

		// ���û������ɶ˿ڣ�����߳̾��ǲ�ͣ��recv���Կͻ��˵���Ϣ
		// ʹ������ɶ˿ڣ�ֱ������Ϣ������ȡ��Ϣ
		int ret = GetQueuedCompletionStatus(
					completionPort,				// ��ɶ˿�
					&dwTransferredLen,			// ��Ϣ��С
					(PULONG_PTR)&sClient,		// �׽���
					(LPOVERLAPPED*)&lpPerIOData,	// ��Ϣ����
					INFINITE					// �ȴ�ʱ��
					);

		switch (dwTransferredLen)
		{
			case -1: 
			{

			}
			break;

			case 0: 
			{
				cout << "�ͻ��˶Ͽ�����" << endl;
				// ��AcceptConnectThread����new�����ģ����ÿ���ͻ��˶Ͽ�ʱdelete��
				delete lpPerIOData;
				lpPerIOData = NULL;
			}
			break;

			default:
			{
				if (pMsgHelper)
				{
					pMsgHelper->NetMsgCallBack(sClient, lpPerIOData->szMessage, dwTransferredLen);
					// ����һ��WSARecv(����recv)
					GetInstance().RegisterWSARecv(sClient, lpPerIOData);
				}

			}
			break;
		}
	}
}

/*************************************************************
��������:	ClientWorkerThread
����˵��:	vParam:	�߳̽�������
�� �� ֵ:	DWORD:	�߳�ִ�н��
����˵��:	�ͻ��˹����ֳ������ڿͻ��˴���ͷ������Ľ���
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
			cout << "�ͷ������Ͽ�����" << endl;
			break;
		}
		pMsgHelper->NetMsgCallBack(0, szBuff, nLen);
	}

	return 0;
}

#pragma endregion ˽�нӿ�


