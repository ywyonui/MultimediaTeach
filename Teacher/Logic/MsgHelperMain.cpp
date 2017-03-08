#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/define/MsgInfo.h"
#include "BLL/define/UserInfo.h"
#include "BLL/define/EUIMsg.h"

#include <iostream>
using namespace std;



CMsgHelperMain::CMsgHelperMain()
{
}


CMsgHelperMain::~CMsgHelperMain()
{
}


#pragma region 公共接口
/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CMsgHelperMain：服务器消息处理对象
功能说明:	返回单例对象
*************************************************************/
CMsgHelperMain& CMsgHelperMain::GetInstance(void)
{
	static CMsgHelperMain m_hInstance;
	return m_hInstance;
}


/*************************************************************
函数名称:	NetMsgCallBack
参数说明:	dwID:	客户端Socket
			vParam:	数据
			nLen:	数据长度
返 回 值:	void
功能说明:	处理消息的回调函数，基类中可以做一些通用的处理，具体应该由派生类实现
*************************************************************/
void CMsgHelperMain::NetMsgCallBack(DWORD dwID, void* vParam, int nLen)
{
	ST_MsgHead stHead;
	memcpy(&stHead, vParam, sizeof(ST_MsgHead));
	switch (stHead.msgType)
	{
	case eRegResult:	// 登陆返回消息
	{
		ST_MsgRegResult msg;
		memcpy(&msg, vParam, sizeof(ST_MsgRegResult));
		if (msg.bSuccess)
		{
			SendMessage(m_hWnd, WM_COMMAND, IDOK, 0);
		}
		else
		{
			AfxMessageBox(_T("注册失败，账号已经存在"));
		}
	}
	break;
	case eLoginResult:	// 登陆返回消息
	{
		ST_MsgLoginResult msg;
		memcpy(&msg, vParam, sizeof(ST_MsgLoginResult));
		if (msg.bSuccess)
		{
			//	SendMessage(m_hWnd, EWND_MSG_LOGIN, (DWORD)&msg.stShowUserInfo, 0);
			SendMessage(m_hWnd, EWND_MSG_LOGIN_SUCCESS, 0, 0);
		}
		else
		{
			AfxMessageBox(_T("登陆失败，账号或密码或者类型错误"));
		}
	}
	break;
	}
}

/*************************************************************
函数名称:	NetMsgCallBack
参数说明:	dwID:	客户端Socket,0表示服务器，其他表示服务器接收到客户端断开
返 回 值:	void
功能说明:	断开连接是个特殊事件，客户端和服务器处理方式不同，因此留出接口重写，默认版本什么都不做
*************************************************************/
void CMsgHelperMain::DisconnectCallBack(DWORD dwID)
{

}

/*************************************************************
函数名称:	SetHwnd
参数说明:	hWnd:	关联窗口
返 回 值:	void
功能说明:	设置一个窗口，用于发送消息，处理UI相关信息
*************************************************************/
void CMsgHelperMain::SetHwnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

/*************************************************************
函数名称:	GetHwnd
参数说明:	void
返 回 值:	hWnd:	关联窗口
功能说明:	返回关联窗口
*************************************************************/
HWND CMsgHelperMain::GetHwnd(void)
{
	return m_hWnd;
}
#pragma endregion 公共接口


#pragma region 私有接口


#pragma endregion 私有接口