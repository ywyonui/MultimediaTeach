#pragma once
/***************************************************************************************/
/**
	�ļ�����:	BaseNetMsgHelper.h	
	����ʱ��:	2016/10/15	12:59:56
	��    ��:	ywyonui
	����˵��:	������Ϣ�������
**/
/***************************************************************************************/

#include <Windows.h>

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


/**
	��    �� :	����
	����˵�� :
*/
class NET_API CBaseNetMsgHelper
{
public:
	CBaseNetMsgHelper();
	~CBaseNetMsgHelper();

#pragma region ��������
public:

#pragma endregion ��������


#pragma region ˽�б���
private:

#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	/*************************************************************
	��������:	NetMsgCallBack
	����˵��:	dwID:	�ͻ���Socket
	vParam:	����
	nLen:	���ݳ���
	�� �� ֵ:	void
	����˵��:	������Ϣ�Ļص������������п�����һЩͨ�õĴ�������Ӧ����������ʵ��
	*************************************************************/
	virtual void NetMsgCallBack(DWORD id, void* vParam, int nLen) = 0;
#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:

#pragma endregion ˽�нӿ�

};

