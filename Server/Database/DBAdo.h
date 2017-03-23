#pragma once
/***************************************************************************************/
/**
	�ļ�����:	DBAdo.h	
	����ʱ��:	2016/10/19	21:22:47
	��    ��:	ywyonui
	����˵��:	MySQL���ݿ����
**/
/***************************************************************************************/
#include "DBAdoFwd.h"
#include <string>

/**	
	��    �� :	
	����˵�� :	
*/
class CDBAdoController
{
private:
	CDBAdoController();
	~CDBAdoController();
	
#pragma region ��������
public:
	enum EDatabaseProvider
	{
		eAccess2000 = 1,
		eAccess2010,
		eODBC,
		eOracle,
		eSqlServer,
	};


#pragma endregion ��������


#pragma region ˽�б���
private:
	_ConnectionPtr	m_pConnection;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordset;
#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	/*************************************************************
	��������:	GetInstance
	����˵��:	void
	�� �� ֵ:	CDBAdoController:	���ݿ����
	����˵��:	���ص�������
	*************************************************************/
	static CDBAdoController& GetInstance();

	/*************************************************************
	��������:	Init
	����˵��:	void
	�� �� ֵ:	bool
	����˵��:	��ʼ������
	*************************************************************/
	bool Init();

	/*************************************************************
	��������:	Connect
	����˵��:	strConnect:����ʹ�õ��ַ���
	�� �� ֵ:	bool
	����˵��:	�������ݿ�
	*************************************************************/
	bool Connect(std::string strConnect);

	/*************************************************************
	��������:	Connect
	����˵��:	eDB:���ݿ����ͣ�
				strDataSource�����ݿ�·����
				strIP�����ݿ�IP��
				bHasPwd���Ƿ�ʹ���˺����룬
				strUserName�����ݿ��û�����
				strPwd�����ݿ��û�����
	�� �� ֵ:	bool
	����˵��:	��ʼ������
	*************************************************************/
	bool Connect(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd);

	/*************************************************************
	��������:	IsConnect
	����˵��:	void
	�� �� ֵ:	bool
	����˵��:	��ǰ�Ƿ��Ѿ�����
	*************************************************************/
	bool IsConnected() const;

	/*************************************************************
	��������:	ExecuteNonQuery
	����˵��:	strCommand: SQL���
	�� �� ֵ:	int: Ӱ������
	����˵��:	ִ�з�select��ѯ���
	*************************************************************/
	int ExecuteNonQuery(const std::string& strCommand);

	/*************************************************************
	��������:	ExecuteReader
	����˵��:	strCommand: SQL���
	�� �� ֵ:	CDBAdoRecordReader: ����ֵ����
	����˵��:	ִ��select��ѯ���
	*************************************************************/
	CDBAdoRecordReader ExecuteReader(const std::string& strCommand);
#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
private:
	/*************************************************************
	��������:	ConnectStringBuilder
	����˵��:	eDB:���ݿ����ͣ�
				strDataSource�����ݿ�·����
				strIP�����ݿ�IP��
				bHasPwd���Ƿ�ʹ���˺����룬
				strUserName�����ݿ��û�����
				strPwd�����ݿ��û�����
	�� �� ֵ:	string: ����ʹ�õ��ַ���
	����˵��:	��������ַ���
	*************************************************************/
	std::string ConnectStringBuilder(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd);

#pragma endregion ˽�нӿ�

};

class CDBAdoRecordReader
{
public:
	CDBAdoRecordReader();
	explicit CDBAdoRecordReader(_RecordsetPtr ptr);
	
	//ʹ�����ü���+1
	CDBAdoRecordReader(const CDBAdoRecordReader& lhs);

	//�������ü���
	~CDBAdoRecordReader();

#pragma region ˽�б���
private:
	_RecordsetPtr m_recordSetPtr;
#pragma endregion ˽�б���


#pragma region �����ӿ�
public:
	//�ƶ�����һ����¼
	bool MoveNext();

	//�ƶ�����һ����¼
	bool MoveFirst();

	//����
	int RecordCount();

	//����
	int FieldCount();

	_variant_t operator[](const size_t nIndex) const;
	
	_variant_t operator[](const std::string key) const;
#pragma endregion �����ӿ�


#pragma region ˽�нӿ�

#pragma endregion ˽�нӿ�


};