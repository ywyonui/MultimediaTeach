/***************************************************************************************/
/**
	�ļ�����:	DBMySQL.cpp	
	����ʱ��:	2016/10/19	21:23:23
	��    ��:	ywyonui
	����˵��:	
**/
/***************************************************************************************/
#include "stdafx.h"

#include <string>

#include "DBMySQL.h"


CDBMySQL::CDBMySQL()
{
}


CDBMySQL::~CDBMySQL()
{
}

#pragma region �����ӿ�
/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CDBMySQL:	��������Ϣ�������
����˵��:	���ص�������
*************************************************************/
CDBMySQL& CDBMySQL::GetInstance()
{
	static CDBMySQL m_hInstance;
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
			strReturnMsg:	����������Ϣ
�� �� ֵ:	int:	��������״̬
����˵��:	�������ݿ�
*************************************************************/
int CDBMySQL::ConnMySQL(const std::string& strIP, const int& nPort, const std::string& strDBName,
	const std::string& strUserName, const std::string& strUserPwd, const std::string& strCharSet,
	std::string& strReturnMsg)
{
	if (mysql_init(&m_MySQL) == NULL)
	{
		strReturnMsg = "inital mysql handle error";
		return 1;
	}

	if (mysql_real_connect(&m_MySQL, strIP.c_str(), strUserName.c_str(), strUserPwd.c_str(), strDBName.c_str(), nPort, NULL, 0) == NULL)
	{
		strReturnMsg = "Failed to connect to database: Error";
		return 1;
	}

	if (mysql_set_character_set(&m_MySQL, strCharSet.c_str()) == NULL)
	{
		strReturnMsg = "mysql_set_character_set Error";
		return 1;
	}

	return 0;
}

/*************************************************************
��������:	DisConnMySQL
����˵��:	void
�� �� ֵ:	void
����˵��:	�Ͽ����ݿ�����
*************************************************************/
void CDBMySQL::DisConnMySQL()
{
	mysql_close(&m_MySQL);
}

/*************************************************************
��������:	InsertData
����˵��:	strSQL:					SQ���
			strReturnMsg��OUT��:	����������Ϣ
�� �� ֵ:	bool:	���ؽ��״̬
����˵��:	��������
*************************************************************/
bool CDBMySQL::InsertData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Insert Data Error";
		return false;
	}

	return true;
}

/*************************************************************
��������:	DelData
����˵��:	strSQL:					SQ���
			strReturnMsg��OUT��:	����������Ϣ
�� �� ֵ:	bool:	���ؽ��״̬
����˵��:	ɾ������
*************************************************************/
bool CDBMySQL::DelData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Del Data Error";
		return false;
	}

	return true;
}

/*************************************************************
��������:	UpdateData
����˵��:	strSQL:					SQ���
			strReturnMsg��OUT��:	����������Ϣ
�� �� ֵ:	bool:	���ؽ��״̬
����˵��:	��������
*************************************************************/
bool CDBMySQL::UpdateData(const std::string& strSQL, std::string& strReturnMsg)
{
	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "Update Data Error";
		return false;
	}

	return true;
}

/*************************************************************
��������:	SelectData
����˵��:	strSQL:					SQ���
			vecResData��OUT��:		��ѯ�������������ݶ����ַ�������ʽ����
			nCol:					��ѯ���ֶ�����
			strReturnMsg��OUT��:	����������Ϣ
�� �� ֵ:	bool:	�Ƿ��ѯ��������
����˵��:	��ѯ���ݿ�
*************************************************************/
bool CDBMySQL::SelectData(const std::string& strSQL, std::vector<std::string>& vecResData, const int nCol, std::string& strReturnMsg)
{
	MYSQL_ROW mRow;
	MYSQL_RES *mRes;

	if (mysql_query(&m_MySQL, strSQL.c_str()) != 0)
	{
		strReturnMsg = "select ps_info Error";
		return false;
	}
	mRes = mysql_store_result(&m_MySQL);

	if (mRes == NULL)
	{
		strReturnMsg = "select username Error";
		return false;
	}
	while (mRow = mysql_fetch_row(mRes))
	{
		for (int i = 0; i < nCol; i++)
		{
			if (mRow[i]) 
			{
				vecResData.push_back(mRow[i]);
			}
		}
	}

	mysql_free_result(mRes);

	return true;
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�


#pragma endregion ˽�нӿ�
