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
#include "define/MsgInfo.h"

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
	��������:	ConnMySQL
	����˵��:	strIP:			����
				nPort:			�˿�
				strDBName:		���ݿ�����
				strUserName:	�û���
				strUserPwd:		�û�����
				strCharSet:		�ַ���
				strReturnMsg��OUT��:	����������Ϣ
	�� �� ֵ:	int:	��������״̬
	����˵��:	�������ݿ�
	*************************************************************/
	int ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName,
		const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet,
		std::string& strReturnMsg);


	/*************************************************************
	��������:	DisConnMySQL
	����˵��:	void
	�� �� ֵ:	void
	����˵��:	�Ͽ����ݿ�����
	*************************************************************/
	void DisConnMySQL();

	/*************************************************************
	��������:	RegUser
	����˵��:	stRegUserInfo :	ע����û���Ϣ
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	ע���û�
	*************************************************************/
	bool RegUser(const ST_RegUserInfo& stRegUserInfo);

	/*************************************************************
	��������:	Login
	����˵��:	stLoginUserInfo :	��½���û���Ϣ
				stShowUserInfo��OUT��:	���ؽ����û�����ʾ��Ϣ
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	��½
	*************************************************************/
	bool Login(const ST_LoginUserInfo& stLoginUserInfo, ST_ShowUserInfo& stShowUserInfo);

	/*************************************************************
	��������:	UpdateMoney
	����˵��:	stShowUserInfo 
	�� �� ֵ:	bool :	�Ƿ�ɹ�
	����˵��:	���½������
	*************************************************************/
	bool UpdateMoney(const ST_ShowUserInfo& stShowUserInfo);


#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:

#pragma endregion ˽�нӿ�

};

