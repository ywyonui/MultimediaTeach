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

#ifndef NET_IMPORT
#define NET_IMPORT __declspec(dllimport)
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
	std::vector<SOCKET>	m_vecSocket;
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
	virtual void NetMsgCallBack(DWORD dwID, void* vParam, int nLen) = 0;

	/*************************************************************
	��������:	NetMsgCallBack
	����˵��:	dwID:	�ͻ���Socket,0��ʾ��������������ʾ���������յ��ͻ��˶Ͽ�
	�� �� ֵ:	void
	����˵��:	�Ͽ������Ǹ������¼����ͻ��˺ͷ���������ʽ��ͬ����������ӿ���д��Ĭ�ϰ汾ʲô������
	*************************************************************/
	virtual void DisconnectCallBack(DWORD dwID);

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:

#pragma endregion ˽�нӿ�

};

