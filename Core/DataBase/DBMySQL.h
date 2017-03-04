#pragma once
/***************************************************************************************/
/**
	�ļ�����:	DBMySQL.h	
	����ʱ��:	2016/10/19	21:22:47
	��    ��:	ywyonui
	����˵��:	MySQL���ݿ����
**/
/***************************************************************************************/

// ����꣬����DLL�еĺ�������
#ifndef NET_API
#define NET_API __declspec(dllexport)
#endif

#include <mysql.h>
#include <vector>

/**	
	��    �� :	
	����˵�� :	
*/
class NET_API CDBMySQL
{
private:
	CDBMySQL();
	~CDBMySQL();
	
#pragma region ��������
public:

#pragma endregion ��������


#pragma region ˽�б���
private:
	MYSQL	m_MySQL;	// MySQL���ݿ����
#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	/*************************************************************
	��������:	GetInstance
	����˵��:	void
	�� �� ֵ:	CDBMySQL:	���ݿ����
	����˵��:	���ص�������
	*************************************************************/
	static CDBMySQL& GetInstance();

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
	��������:	InsertData
	����˵��:	strSQL:					SQ���
				strReturnMsg��OUT��:	����������Ϣ
	�� �� ֵ:	bool:	���ؽ��״̬
	����˵��:	��������
	*************************************************************/
	bool InsertData(const std::string& strSQL, std::string& strReturnMsg);

	/*************************************************************
	��������:	DelData
	����˵��:	strSQL:					SQ���
				strReturnMsg��OUT��:	����������Ϣ
	�� �� ֵ:	bool:	���ؽ��״̬
	����˵��:	ɾ������
	*************************************************************/
	bool DelData(const std::string& strSQL, std::string& strReturnMsg);

	/*************************************************************
	��������:	UpdateData
	����˵��:	strSQL:					SQ���
				strReturnMsg��OUT��:	����������Ϣ
	�� �� ֵ:	bool:	���ؽ��״̬
	����˵��:	��������
	*************************************************************/
	bool UpdateData(const std::string& strSQL, std::string& strReturnMsg);
	
	/*************************************************************
	��������:	SelectData
	����˵��:	strSQL:					SQ���
				nCol:					��ѯ���ֶ�����
				vecResData��OUT��:		��ѯ�������������ݶ����ַ�������ʽ����
				strReturnMsg��OUT��:	����������Ϣ
	�� �� ֵ:	bool:	�Ƿ��ѯ��������
	����˵��:	��ѯ���ݿ�
	*************************************************************/
	bool SelectData(const std::string& strSQL, std::vector<std::string>& vecResData, const int nCol, std::string& strReturnMsg);
#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:

#pragma endregion ˽�нӿ�

};
