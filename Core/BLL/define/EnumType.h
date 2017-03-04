#pragma once
/***************************************************************************************/
/**
	�ļ�����:	EnumType.h	
	����ʱ��:	2016/10/21	19:43:38
	��    ��:	ywyonui
	����˵��:	ö�����͵Ķ����ļ�
**/
/***************************************************************************************/

/**	
	��    �� :	��Ϣ���͵�ö��
	����˵�� :	����ͻ����������֮������ͣ�����ͨ��������Ӧ��
				�ͻ��˵���������eXXX�������������ͻ��ˣ�eXXXResult��
*/
enum EMsgType
{
	eReg,		// ע��
	eRegResult,	// ע�᷵����Ϣ
	eLogin,			// ��½
	eLoginResult,	// ��½������Ϣ
	eEnterGame,			// ������ټ�����Ϸ��ť
	eEnterGameResult,	// ������ټ�����Ϸ��ť������Ϣ
};

/**
	��    �� :	��λ״̬��ö��
	����˵�� :	���ڱ�ʶ�����е���λ״̬��Ϣ
*/
enum ESeatStatus
{
	eNull,		// ����λ
	eNotReady,	// ��������δ׼��
	eReady,		// �������²�׼��
	ePlaying	// ������Ϸ��
};

/**
��    �� :	����״̬��ö��
����˵�� :	���ڱ�ʶ����״̬��Ϣ
*/
enum EDeskStatus
{
	eFree,		// �п���λ
	eFill,		// �Ѿ�������
	eBusying	// ������Ϸ��
};

/**
	��    �� :	��������
	����˵�� :	����һЩ�������ͣ������жϺ�������ֵ
*/
enum EResultType
{
	eOK = 0,	// ��������
	eLoginError = -1,	// ��½ʧ��
};

#define DESK_MAX_PLAYER	3

