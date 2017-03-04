#pragma once

#include <windows.h>

// 消息最多尺寸
#define MAX_MSG_SIZE 1024

// 初始化网络接口
#define NET_API __declspec(dllexport)

// 接受消息的回调函数，dwID表示socket，vParam表示具体消息，nLen表示消息长度
typedef void(*NetCallBack)(DWORD dwID, void* vParam, int nLen);

// 初始化网络，服务器和客户端都是用同一个
NET_API bool InitNet(unsigned uPort, NetCallBack callBack, bool bServer = true, char* pAddr = NULL);

// 发送消息给客户端
NET_API void SendToClient(DWORD dwID, void* vParam, int nLen);

// 发送消息给服务器
NET_API void SendToServer(void* vParam, int nLen);
