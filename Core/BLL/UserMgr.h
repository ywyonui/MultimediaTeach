#pragma once
/***************************************************************************************/
/**
	�ļ�����:	UserMgr.h	
	����ʱ��:	2016/10/22	22:08:08
	��    ��:	ywyonui
	����˵��:	
**/
/***************************************************************************************/
#include "BaseBLL.h"
#include "CoreDefine.h"
#include <vector>

/**	
	��    �� :	
	����˵�� :	
*/
class BLL_API CUserMgr
{
public:
	CUserMgr();
	~CUserMgr();
	
#pragma region ��������
public:

#pragma endregion ��������


#pragma region ˽�б���
private:

#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	/*************************************************************
	��������:	GetInstance
	����˵��:	void
	�� �� ֵ:	CUserMgr:	�û�����
	����˵��:	���ص�������
	*************************************************************/
	static CUserMgr& GetInstance();

	/*************************************************************
	��������:	RegUser
	����˵��:	stRegUserInfo :	ע����û���Ϣ
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	ע���û�
	*************************************************************/
	bool RegUser(const ST_RegUserInfo& stRegUserInfo, const EClientType& eType);

	/*************************************************************
	��������:	Login
	����˵��:	stLoginUserInfo :	��½���û���Ϣ
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	��½��Ŀǰ��½�ɹ�֮����Ҫ��ȡ�������ݣ����������ڳ�ʼ����ʱ�������������ݲ�ѯ
	*************************************************************/
	bool Login(const ST_LoginUserInfo& stLoginUserInfo, const EClientType& eType);

	/*************************************************************
	��������:	AskForClientList
	����˵��:	vecClient��OUT��: ������������ز�ѯ�����û��б� 
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	��ѯ�û��б����ڼ�¼��ʾ
	*************************************************************/
	bool AskForClientList(std::vector<ST_ClientInfo>& vecClient);

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:

#pragma endregion ˽�нӿ�

};

