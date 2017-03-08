#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"
#include "BLL/define/EUIMsg.h"

#include <iostream>
using namespace std;

#pragma region Imp
class CMsgHelperMain::Imp
{
public:
	Imp() {}
	~Imp(){}

private:
	HWND m_hWnd;
	std::string m_strToWnd;

public:
	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }

public:
	void UserReg(DWORD dwID, void* vParam);

	void UserLogin(DWORD dwID, void* vParam);
};

#pragma region 消息处理

void CMsgHelperMain::Imp::UserReg(DWORD dwID, void* vParam)
{
	ST_MsgReg msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	ST_MsgRegResult msgRes;
	msgRes.stMsgHead.msgType = eRegResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().RegUser(msg.stRegInfo, msg.stMsgHead.clientType);
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgRegResult));

	char strID[10] = { 0 };
	sprintf_s(strID, "<%d>", dwID);
	m_strToWnd = "SOCKET:";
	m_strToWnd += strID; 
	m_strToWnd += "客户端注册";
	SendMessage(m_hWnd, EWND_MSG_SERVER_RECV, (WPARAM)(&msg.stMsgHead), (LPARAM)(&m_strToWnd));
}

void CMsgHelperMain::Imp::UserLogin(DWORD dwID, void* vParam)
{
	ST_MsgLogin msg;
	memcpy(&msg, vParam, sizeof(ST_MsgLogin));
	ST_MsgLoginResult msgRes;
	msgRes.stMsgHead.msgType = eLoginResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo, msg.stMsgHead.clientType);

	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));

	char strID[10] = { 0 };
	sprintf_s(strID, "<%d>", dwID);
	m_strToWnd = "SOCKET:";
	m_strToWnd += strID;
	m_strToWnd += "客户端登陆";
	SendMessage(m_hWnd, EWND_MSG_SERVER_RECV, (WPARAM)(&msg.stMsgHead), (LPARAM)(&m_strToWnd));
}

#pragma endregion 消息处理


#pragma endregion Imp



CMsgHelperMain::CMsgHelperMain()
	: m_hWnd(NULL)
{
	m_pImp = new Imp;
}


CMsgHelperMain::~CMsgHelperMain()
{
	m_hWnd = NULL;
	if (m_pImp)
	{
		delete m_pImp;
		m_pImp = NULL;
	}
}


#pragma region 公共接口
/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CMsgHelperServer：服务器消息处理对象
功能说明:	返回单例对象
*************************************************************/
CMsgHelperMain& CMsgHelperMain::GetInstance(void)
{
	static CMsgHelperMain m_hInstance;
	return m_hInstance;
}


/*************************************************************
函数名称:	NetMsgCallBack
参数说明:	dwID:	服务器Socket
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
	case eReg:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
	case eLogin:	// 登陆消息
	{
		m_pImp->UserLogin(dwID, vParam);
	}
	break;

	default:
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
	m_pImp->SetHwnd(hWnd);
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