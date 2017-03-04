#pragma once
/***************************************************************************************/
/**
	文件名称:	EnumType.h	
	创建时间:	2016/10/21	19:43:38
	作    者:	ywyonui
	功能说明:	枚举类型的定义文件
**/
/***************************************************************************************/

/**	
	类    型 :	消息类型的枚举
	功能说明 :	定义客户端与服务器之间的类型，类型通常两两对应，
				客户端到服务器（eXXX），服务器到客户端（eXXXResult）
*/
enum EMsgType
{
	eReg,		// 注册
	eRegResult,	// 注册返回消息
	eLogin,			// 登陆
	eLoginResult,	// 登陆返回消息
	eEnterGame,			// 点击快速加入游戏按钮
	eEnterGameResult,	// 点击快速加入游戏按钮返回消息
};

/**
	类    型 :	座位状态的枚举
	功能说明 :	用于标识桌子中的座位状态信息
*/
enum ESeatStatus
{
	eNull,		// 空座位
	eNotReady,	// 有人坐下未准备
	eReady,		// 有人坐下并准备
	ePlaying	// 正在游戏中
};

/**
类    型 :	桌子状态的枚举
功能说明 :	用于标识桌子状态信息
*/
enum EDeskStatus
{
	eFree,		// 有空座位
	eFill,		// 已经坐满了
	eBusying	// 正在游戏中
};

/**
	类    型 :	返回类型
	功能说明 :	定义一些返回类型，用于判断函数返回值
*/
enum EResultType
{
	eOK = 0,	// 返回正常
	eLoginError = -1,	// 登陆失败
};

#define DESK_MAX_PLAYER	3


