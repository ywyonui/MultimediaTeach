#pragma once

#include <windows.h>

// ��Ϣ���ߴ�
#define MAX_MSG_SIZE 1024

// ��ʼ������ӿ�
#define NET_API __declspec(dllexport)

// ������Ϣ�Ļص�������dwID��ʾsocket��vParam��ʾ������Ϣ��nLen��ʾ��Ϣ����
typedef void(*NetCallBack)(DWORD dwID, void* vParam, int nLen);

// ��ʼ�����磬�������Ϳͻ��˶�����ͬһ��
NET_API bool InitNet(unsigned uPort, NetCallBack callBack, bool bServer = true, char* pAddr = NULL);

// ������Ϣ���ͻ���
NET_API void SendToClient(DWORD dwID, void* vParam, int nLen);

// ������Ϣ��������
NET_API void SendToServer(void* vParam, int nLen);
