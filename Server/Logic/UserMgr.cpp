#include "stdafx.h"
#include "UserMgr.h"

#include "DBAdo.h"

CUserMgr::CUserMgr()
{
}


CUserMgr::~CUserMgr()
{
}


#pragma region �����ӿ�

/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CUserMgr:	�û�����
����˵��:	���ص�������
*************************************************************/
CUserMgr& CUserMgr::GetInstance()
{
	static CUserMgr m_hInstance;
	return m_hInstance;
}

/*************************************************************
��������:	RegUser
����˵��:	stRegUserInfo :	ע����û���Ϣ
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	ע���û�
*************************************************************/
bool CUserMgr::RegUser(const ST_RegUserInfo& stRegUserInfo, const int& eType)
{
	char pStrSQL[1024] = { 0 };

	// 1����ѯ�û����ǲ��Ǵ���
	sprintf_s(pStrSQL, "select UserName from user where UserName = \"%s\";",
			  stRegUserInfo.strUserName);


	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	CDBAdoRecordReader record = CDBAdoController::GetInstance().ExecuteReader(pStrSQL);

	// ����鵽�����ݣ����ʾ�û����ظ���
	if (record.RecordCount() > 0)
	{
		return false;
	}
	
	sprintf_s(pStrSQL, "insert into user(UserName, UserPetName, UserPwd, UserType) values(\"%s\", \"%s\", \"%s\", %d );", 
			  stRegUserInfo.strUserName, stRegUserInfo.strPetName, stRegUserInfo.strUserPwd, eType);

	return CDBAdoController::GetInstance().ExecuteNonQuery(pStrSQL) == 1;
}

/*************************************************************
��������:	Login
����˵��:	stLoginUserInfo :	��½���û���Ϣ
			stShowUserInfo��OUT��:	���ؽ����û�����ʾ��Ϣ
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	��½
*************************************************************/
bool CUserMgr::Login(const ST_LoginUserInfo& stLoginUserInfo, const int& eType)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "select * from user where UserName = \"%s\" and UserPwd = \"%s\" and UserType = %d;",
			  stLoginUserInfo.strUserName, stLoginUserInfo.strUserPwd, eType);

	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	return CDBAdoController::GetInstance().ExecuteNonQuery(pStrSQL) == 1;
}


/*************************************************************
��������:	SearchUserList
����˵��:	list��OUT��: ������������ز�ѯ�����û��б�
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	��ѯ�û��б����ڼ�¼��ʾ
*************************************************************/
bool CUserMgr::AskForClientList(std::vector<ST_ClientInfo>& vecClient)
{

	return true;
}


#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�

