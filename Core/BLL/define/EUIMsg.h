#pragma once
/***************************************************************************************/
/**
	文件名称:	ClientDefine.h	
	创建时间:	2016/11/07	22:30:12
	作    者:	ywyonui
	功能说明:	客户端的一些公共个定义
**/
/***************************************************************************************/
#include <windows.h>

#pragma region 自定义消息
enum EClientMessage
{
	EWND_MSG_LOGIN_SUCCESS = WM_USER + 101,	// 登陆成功后的返回消息，WPARAM是登陆成功后返回的数据

	EWND_MSG_SERVER_RECV = WM_USER + 2001	// 服务器接收到数据，传给界面显示
};
#pragma endregion 自定义消息