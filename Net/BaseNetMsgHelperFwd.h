#pragma once
/***************************************************************************************/
/**
�ļ�����:	BaseNetMsgHelper.h
����ʱ��:	2016/10/15	12:59:56
��    ��:	ywyonui
����˵��:	������Ϣ�������
**/
/***************************************************************************************/
#pragma region ������

// ����꣬����DLL�еĺ�������
#ifndef NET_API
#define NET_API __declspec(dllexport)
#endif

// ��Ϣ���ߴ�
#ifndef MAX_MSG_SIZE
#define MAX_MSG_SIZE 1024
#endif

#pragma endregion ������

class NET_API CBaseNetMsgHelper;
