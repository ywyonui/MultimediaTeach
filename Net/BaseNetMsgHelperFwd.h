#pragma once
/***************************************************************************************/
/**
文件名称:	BaseNetMsgHelper.h
创建时间:	2016/10/15	12:59:56
作    者:	ywyonui
功能说明:	网络消息处理基类
**/
/***************************************************************************************/
#pragma region 宏声明

// 定义宏，用于DLL中的函数导出
#ifndef NET_API
#define NET_API __declspec(dllexport)
#endif

// 消息最多尺寸
#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE 1024
#endif

#pragma endregion 宏声明

class NET_API CBaseNetMsgHelper;
