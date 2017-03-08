#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"
#include "BLL/define/EUIMsg.h"
#include <vector>

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
	std::vector<ST_ClientInfo> m_vecClientInfo;

public:
	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }

private:
	void SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess);

public:
	// 用户连接
	void UserConnect(DWORD dwID, void* vParam);
	// 注册
	void UserReg(DWORD dwID, void* vParam);
	// 登陆
	void UserLogin(DWORD dwID, void* vParam);
};

// 通知服务器界面显示数据
void CMsgHelperMain::Imp::SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess)
{
	char strID[10] = { 0 };
	sprintf_s(strID, "<%d>", dwID);
	m_strToWnd = "SOCKET:";
	m_strToWnd += strID;
	m_strToWnd += strBase;
	m_strToWnd += bSuccess ? "成功" : "失败";
	SendMessage(m_hWnd, EWND_MSG_SERVER_RECV, (WPARAM)(stHead), (LPARAM)(&m_strToWnd));
}

#pragma region 消息处理
// 连接
void CMsgHelperMain::Imp::UserConnect(DWORD dwID, void* vParam)
{
	ST_MsgConnect msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	// 不同客户端类型，连接成功之后，做不同处理
	if (msg.stMsgHead.clientType == eTeacher)
	{

	}
	else
	{

	}

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "连接", true);
}
// 注册
void CMsgHelperMain::Imp::UserReg(DWORD dwID, void* vParam)
{
	ST_MsgReg msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	ST_MsgRegResult msgRes;
	msgRes.stMsgHead.msgType = eRegResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().RegUser(msg.stRegInfo, msg.stMsgHead.clientType);
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgRegResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "注册", msgRes.bSuccess);
}
// 登陆
void CMsgHelperMain::Imp::UserLogin(DWORD dwID, void* vParam)
{
	ST_MsgLogin msg;
	memcpy(&msg, vParam, sizeof(ST_MsgLogin));
	ST_MsgLoginResult msgRes;
	msgRes.stMsgHead.msgType = eLoginResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo, msg.stMsgHead.clientType);

	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "登陆", msgRes.bSuccess);
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
	case eConnect:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
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