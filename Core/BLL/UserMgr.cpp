#include "stdafx.h"
#include "UserMgr.h"

#include "DBMySQL.h"

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
int CUserMgr::ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName, 
						const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet, std::string& strReturnMsg)
{
	return CDBMySQL::GetInstance().ConnMySQL(strIP, nPort, strDBName, strUserName, strUserPwd, strCharSet, strReturnMsg);
}

/*************************************************************
��������:	DisConnMySQL
����˵��:	void
�� �� ֵ:	void
����˵��:	�Ͽ����ݿ�����
*************************************************************/
void CUserMgr::DisConnMySQL()
{
	CDBMySQL::GetInstance().DisConnMySQL();
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
	sprintf_s(pStrSQL, "select UserName from userinfo where UserName = \"%s\";",
			  stRegUserInfo.strUserName);


	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 1, strReturnMsg);

	// ����鵽�����ݣ����ʾ�û����ظ���
	if (bSuccess && vecResData.size() > 0)
	{
		return false;
	}
	
	sprintf_s(pStrSQL, "insert into userinfo(UserName, UserPetName, UserPwd) values(\"%s\", \"%s\", md5(\"%s\") );", 
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
bool CUserMgr::Login(const ST_LoginUserInfo& stLoginUserInfo, ST_ShowUserInfo& stShowUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "select * from userinfo where UserName = \"%s\" and UserPwd = md5(\"%s\");",
			  stLoginUserInfo.strUserName, stLoginUserInfo.strUserPwd);

	std::string strReturnMsg = "";

	std::vector<std::string> vecResData;

	bool bSuccess = CDBMySQL::GetInstance().SelectData(pStrSQL, vecResData, 5, strReturnMsg);

	if (!bSuccess)
	{
		return false;
	}
	else
	{
		if (vecResData.size() != 5)
		{
			return false;
		}
		// �����ݶ�ȡ������ŵ�ST_ShowUserInfo��
		stShowUserInfo.nUserID = atoi(vecResData.at(0).c_str());
		memcpy(stShowUserInfo.strUserName, vecResData.at(1).c_str(), vecResData.at(1).size());
		memcpy(stShowUserInfo.strPetName, vecResData.at(2).c_str(), vecResData.at(2).size());
		stShowUserInfo.nMoney = atoi(vecResData.at(4).c_str());
		return true;
	}

}

/*************************************************************
��������:	UpdateMoney
����˵��:	stShowUserInfo
�� �� ֵ:	bool :	�Ƿ�ɹ�
����˵��:	���½������
*************************************************************/
bool CUserMgr::UpdateMoney(const ST_ShowUserInfo& stShowUserInfo)
{
	char pStrSQL[1024] = { 0 };
	sprintf_s(pStrSQL, "update userinfo set Money = %d where UserID = %d;",
			  stShowUserInfo.nMoney, stShowUserInfo.nUserID);

	std::string strReturn = "";

	return CDBMySQL::GetInstance().UpdateData(pStrSQL, strReturn);
}


#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�

