#pragma once
/***************************************************************************************/
/**
	文件名称:	MsgHelperServer.h	
	创建时间:	2016/10/13	22:37:51
	作    者:	ywyonui
	功能说明:	服务器的消息管理工具，处理服务端接收到的来自客户端的消息
**/
/***************************************************************************************/

#include "BaseNetMsgHelper.h"

/**
	类    型 :	单例类
	功能说明 :	处理服务器接收到的消息
*/
class CMsgHelperMain : public CBaseNetMsgHelper
{
private:
	CMsgHelperMain();
	~CMsgHelperMain();

#pragma region 公共变量
public:

#pragma endregion 公共变量


#pragma region 私有变量
private:
	HWND	m_hWnd;
	DWORD	m_dwSocket;
#pragma endregion 私有变量


#pragma region 公共接口
public:
	/*************************************************************
	函数名称:	GetInstance
	参数说明:	void
	返 回 值:	CMsgHelperMain:	服务器消息处理对象
	功能说明:	返回单例对象
	*************************************************************/
	static CMsgHelperMain& GetInstance(void);

	/*************************************************************
	函数名称:	NetMsgCallBack
	参数说明:	dwID:	客户端Socket
	vParam:	数据
	nLen:	数据长度
	返 回 值:	void
	功能说明:	重载基类版本，实现具体的消息处理
	*************************************************************/
	virtual void NetMsgCallBack(DWORD dwID, void* vParam, int nLen);

	/*************************************************************
	函数名称:	NetMsgCallBack
	参数说明:	dwID:	客户端Socket,0表示服务器，其他表示服务器接收到客户端断开
	返 回 值:	void
	功能说明:	断开连接是个特殊事件，客户端和服务器处理方式不同，因此留出接口重写，默认版本什么都不做
	*************************************************************/
	virtual void DisconnectCallBack(DWORD dwID);

	/*************************************************************
	函数名称:	SetHwnd
	参数说明:	hWnd:	关联窗口
	返 回 值:	void
	功能说明:	设置一个窗口，用于发送消息，处理UI相关信息
	*************************************************************/
	void SetHwnd(HWND hWnd);

	/*************************************************************
	函数名称:	GetHwnd
	参数说明:	void
	返 回 值:	hWnd:	关联窗口
	功能说明:	返回关联窗口
	*************************************************************/
	HWND GetHwnd(void);

	/*************************************************************
	函数名称:	GetSocket()
	参数说明:	void
	返 回 值:	
	功能说明:	返回Socket，用于外部直接进行数据交互
	*************************************************************/
	DWORD GetSocket(void);
#pragma endregion 公共接口


#pragma region 私有接口
private:

#pragma endregion 私有接口

};

