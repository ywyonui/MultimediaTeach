#include "stdafx.h"
#include "BaseNetMsgHelper.h"

CBaseNetMsgHelper::CBaseNetMsgHelper()
{

}

CBaseNetMsgHelper::~CBaseNetMsgHelper()
{

}

#pragma region 公共接口
/*************************************************************
函数名称:	NetMsgCallBack
参数说明:	dwID:	客户端Socket,0表示服务器，其他表示服务器接收到客户端断开
返 回 值:	void
功能说明:	断开连接是个特殊事件，客户端和服务器处理方式不同，因此留出接口重写，默认什么都不做
*************************************************************/
void CBaseNetMsgHelper::DisconnectCallBack(DWORD dwID)
{

}

#pragma endregion 公共接口


#pragma region 私有接口


#pragma endregion 私有接口