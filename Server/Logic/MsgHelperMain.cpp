#include "stdafx.h"
#include "MsgHelperMain.h"

#include "BLL/UserMgr.h"
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
	std::vector<ST_ClientInfo>& GetClientList() { return m_vecClientInfo; }

private:
	void SendRecvMsgToUI(DWORD dwID, ST_MsgHead* stHead, char* strBase, bool bSuccess);

public:
	// 用户连接
	void UserConnect(DWORD dwID, void* vParam);
	// 注册
	void UserReg(DWORD dwID, void* vParam);
	// 登陆
	void UserLogin(DWORD dwID, void* vParam);

public:
	void DisconnectCallBack(DWORD dwID);


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
	memcpy(&msg, vParam, sizeof(ST_MsgConnect));

	ST_MsgConnectResult msgRes;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	// 不同客户端类型，连接成功之后，做不同处理
	if (msg.stMsgHead.clientType == eTeacher)
	{
		ST_ClientInfo st;
		st.dwSocket = dwID;
		memcpy(st.arrIP, msg.arrIP, 16);
		st.eCS = eClientConnect;
		st.eCT = msg.stMsgHead.clientType;
		m_vecClientInfo.push_back(st);

		msgRes.bSuccess = true;
	}
	else 
	{
		bool bIPinList = false;
		// 学生端的基本信息是在教师端的配置中进行插入的
		for (auto& it : m_vecClientInfo)
		{
			// 找到IP相同的客户端，改变他的状态信息，并且记录对应的Socket
			if (!memcmp(it.arrIP, msg.arrIP, 16))
			{
				it.dwSocket = dwID;
				it.eCS = eClientConnect;
				it.eCT = msg.stMsgHead.clientType;
				bIPinList = true;
				break;
			}
		}
		msgRes.bSuccess = bIPinList;
	}
	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgConnectResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "连接", true);
}
// 注册
void CMsgHelperMain::Imp::UserReg(DWORD dwID, void* vParam)
{
	ST_MsgReg msg;
	memcpy(&msg, vParam, sizeof(ST_MsgReg));
	ST_MsgRegResult msgRes;
	msgRes.stMsgHead.msgType = eMsgRegResult;
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
	msgRes.stMsgHead.msgType = eMsgLoginResult;
	msgRes.stMsgHead.clientType = msg.stMsgHead.clientType;
	msgRes.bSuccess = CUserMgr::GetInstance().Login(msg.stLoginInfo, msg.stMsgHead.clientType);

	CTCPNet::GetInstance().SendToClient(dwID, &msgRes, sizeof(ST_MsgLoginResult));

	SendRecvMsgToUI(dwID, &msg.stMsgHead, "登陆", msgRes.bSuccess);
}

// 断开连接
void CMsgHelperMain::Imp::DisconnectCallBack(DWORD dwID)
{
	// 断开的时候，教师端需要向所有学生端发送消息，并且关闭所有学生端，并且清空服务器列表

	for (auto& it = m_vecClientInfo.begin(); it != m_vecClientInfo.end(); )
	{
		if (it->dwSocket == dwID) 
		{
			if (it->eCT == eTeacher)
			{
				auto it2 = it;
				m_vecClientInfo.erase(it2);
				ST_MsgHead stResult;
				stResult.msgType = eMsgDisConnect;
				int nSize = sizeof(ST_MsgHead);
				for (auto& it : m_vecClientInfo)
				{
					CTCPNet::GetInstance().SendToClient(it.dwSocket, &stResult, nSize);
				}
				m_vecClientInfo.clear();
			}
			else
			{
				it->eCS = eClientDisConnect;
			}
			break;
		}
		it++;
	}
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
	case eMsgConnect:
	{
		m_pImp->UserConnect(dwID, vParam);
	}
	break;
	case eMsgReg:
	{
		m_pImp->UserReg(dwID, vParam);
	}
	break;
	case eMsgLogin:	// 登陆消息
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
	m_pImp->DisconnectCallBack(dwID);
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