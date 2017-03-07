#include "stdafx.h"
#include "UserMgr.h"

#include "DataBase/DBMySQL.h"

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
bool CUserMgr::RegUser(const ST_RegUserInfo& stRegUserInfo)
{
	char pStrSQL[1024] = { 0 };

	// 1����ѯ�û����ǲ��Ǵ���
	sprintf_s(pStrSQL, "select UserName from user where UserName = \"%s\";",
			  stRegUserInfo.strUserName);


	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 1, strReturnMsg);

	// ����鵽�����ݣ����ʾ�û����ظ���
	if (bSuccess && vecResData.size() > 0)
	{
		return false;
	}
	
	sprintf_s(pStrSQL, "insert into user(UserName, UserPetName, UserPwd) values(\"%s\", \"%s\", md5(\"%s\") );", 
			  stRegUserInfo.strUserName, stRegUserInfo.strPetName, stRegUserInfo.strUserPwd);

	std::string strReturn = "";

	return CDBMySQL::GetInstance().InsertData(pStrSQL, strReturn);
}

/*************************************************************
��������:	Login
����˵��:	stLoginUserInfo :	��½���û���Ϣ
			stShowUserInfo��OUT��:	���ؽ����û�����ʾ��Ϣ
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	��½
*************************************************************/
bool CUserMgr::Login(const ST_LoginUserInfo& stLoginUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "select * from user where UserName = \"%s\" and UserPwd = md5(\"%s\");",
			  stLoginUserInfo.strUserName, stLoginUserInfo.strUserPwd);

	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 5, strReturnMsg);

	if (!bSuccess)
	{
		return false;
	}
	
	return true;
}


/*************************************************************
��������:	SearchUserList
����˵��:	list��OUT��: ������������ز�ѯ�����û��б�
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	��ѯ�û��б����ڼ�¼��ʾ
*************************************************************/
bool CUserMgr::SearchUserList(OUT int& list)
{
	return true;
}


#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�

