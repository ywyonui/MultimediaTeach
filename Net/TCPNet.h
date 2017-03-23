#pragma once
/*******************************************************/
/**
	�ļ�����:	TCPNet.h	
	����ʱ��:	2016/10/13	20:28:27
	��    ��:	ywyonui
	����˵��:	������һ�������ռ䣬��������㣬��Ҫ����һ������TCP�����������
**/
/*******************************************************/

#include "BaseNetMsgHelper.h"
#include <mutex>

#define MAX_NET_BUFFER_SIZE 2048

/**
	��    �� :	�����࣬���NET_API���ڵ�����DLL��
	����˵�� :	�������Ϳͻ��˹��õ����������
*/
class NET_API CTCPNet
{
private:
	CTCPNet();
	~CTCPNet();

#pragma region ��������
public:

#pragma endregion ��������


#pragma region ˽�б���
private:
	DWORD m_ServerSocket;
	DWORD m_ClientSocket;

	std::mutex m_mutex;
	CBaseNetMsgHelper*	m_pNetMsgHelper;

#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	/*************************************************************
	��������:	GetInstance
	����˵��:	void
	�� �� ֵ:	CTCPNet:	�������
	����˵��:	���ص�������
	*************************************************************/
	static CTCPNet& GetInstance(void);

	/*************************************************************
	��������:	InitNet
	����˵��:	uPort:	�˿ں�,
				pMsgHelper:	��Ϣ�������
				bServer:	�ж��ǲ��Ƿ�����
				pAddr:	��ַ
	�� �� ֵ:	bool:	��ʼ���ɹ�����ʧ��
	����˵��:	��ʼ�����绷��
	*************************************************************/
	bool InitNet(unsigned uPort, CBaseNetMsgHelper* pMsgHelper, bool bServer = true, char* pAddr = NULL);

	/*************************************************************
	��������:	SendToClient
	����˵��:	dwID:	�ͻ���Socket,
				vParam:	���͵�����
				nLen:	���ݳ���
	�� �� ֵ:	void
	����˵��:	�������������ݵ��ͻ���
	*************************************************************/
	void SendToClient(DWORD dwID, void* vParam, int nLen);


	/*************************************************************
	��������:	SendToServer
	����˵��:	vParam:	���͵�����
				nLen:	���ݳ���
	�� �� ֵ:	void
	����˵��:	�ͻ��˷������ݵ�������
	*************************************************************/
	void SendToServer(void* vParam, int nLen);

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:
	/*************************************************************
	��������:	RegisterWSARecv
	����˵��:	sClient:	�ͻ��˵�SOCKET
	�� �� ֵ:	void
	lpPerIOData:	�������ݵĶ���ָ��
	����˵��:	�ͻ��˹����ֳ������ڿͻ��˴���ͷ������Ľ���
	*************************************************************/
	void RegisterWSARecv(DWORD sClient, void* lpPerIOData);

	/*************************************************************
	��������:	AcceptConnectThread
	����˵��:	vParam:	�߳̽�������
	�� �� ֵ:	DWORD:	�߳�ִ�н��
	����˵��:	�����ͻ����������
	*************************************************************/
	static DWORD WINAPI AcceptConnectThread(void* vParam);

	/*************************************************************
	��������:	ServerWorkerThread
	����˵��:	vParam:	�߳̽�������
	�� �� ֵ:	DWORD:	�߳�ִ�н��	
	����˵��:	�����������̣߳����ڷ���������Ϳͻ��˵Ľ�����
				ÿ���ͻ��˶�Ӧһ���߳�
	*************************************************************/
	static DWORD WINAPI ServerWorkerThread(void* vParam);

	/*************************************************************
	��������:	ClientWorkerThread
	����˵��:	vParam:	�߳̽�������
	�� �� ֵ:	DWORD:	�߳�ִ�н��
	����˵��:	�ͻ��˹����ֳ������ڿͻ��˴���ͷ������Ľ���
	*************************************************************/
	static DWORD WINAPI ClientWorkerThread(void* vParam);

#pragma endregion ˽�нӿ�

};




