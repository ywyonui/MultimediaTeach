/***************************************************************************************/
/**
	�ļ�����:	DBADO.cpp	
	����ʱ��:	2016/10/19	21:23:23
	��    ��:	ywyonui
	����˵��:	
**/
/***************************************************************************************/
#include "stdafx.h"
#include "afxwin.h"
#include <string>
#include "DBAdo.h"

#pragma region CDBAdoController

CDBAdoController::CDBAdoController()
{
	AfxOleInit();
}


CDBAdoController::~CDBAdoController()
{
	if (m_pConnection->State)
	{
		m_pConnection->Close();
	}
	m_pCommand.Release();
	m_pRecordset.Release();
	m_pConnection.Release();
}

#pragma region �����ӿ�
/*************************************************************
��������:	GetInstance
����˵��:	void
�� �� ֵ:	CDBAdoController:	��������Ϣ�������
����˵��:	���ص�������
*************************************************************/
CDBAdoController& CDBAdoController::GetInstance()
{
	static CDBAdoController m_hInstance;
	return m_hInstance;
}

/*************************************************************
��������:	Init
����˵��:	void
�� �� ֵ:	bool
����˵��:	��ʼ������
*************************************************************/
bool CDBAdoController::Init()
{
	HRESULT hr = m_pConnection.CreateInstance("ADODB.Connection");
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	hr = m_pCommand.CreateInstance("ADODB.Command");
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	hr = m_pRecordset.CreateInstance("ADODB.Recordset");
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	return true;
}

/*************************************************************
��������:	Connect
����˵��:	strConnect:����ʹ�õ��ַ���
�� �� ֵ:	bool
����˵��:	�������ݿ�
*************************************************************/
bool CDBAdoController::Connect(std::string strConnect)
{
	if (m_pConnection->State)
	{
		m_pConnection->Close();
	}
	try
	{
		HRESULT hr = m_pConnection->Open(_bstr_t(strConnect.c_str()), "", "", adModeUnknown);
		if (!SUCCEEDED(hr))
		{
			throw std::exception();
		}
		return true;
	}
	catch (_com_error e)
	{
		// �������ݿ�ʧ��
		CStringA strA(e.ErrorMessage());
		CString str(strA);
		AfxMessageBox(str);
		return false;
	}
}

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
bool CDBAdoController::Connect(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd)
{
	if (m_pConnection->State)
	{
		m_pConnection->Close();
	}
	std::string strConnect = ConnectStringBuilder(eDB, strDataSource, strIP, bHasPwd, strUserName, strPwd);
	return Connect(strConnect);
}

/*************************************************************
��������:	IsConnect
����˵��:	void
�� �� ֵ:	bool
����˵��:	��ǰ�Ƿ��Ѿ�����
*************************************************************/
bool CDBAdoController::IsConnected() const
{
	return static_cast<bool>(m_pConnection->State != 0);
}

/*************************************************************
��������:	ExecuteNonQuery
����˵��:	strCommand: SQL���
�� �� ֵ:	int: Ӱ������
����˵��:	ִ�з�select��ѯ���
*************************************************************/
int CDBAdoController::ExecuteNonQuery(const std::string& strCommand)
{
	if (!IsConnected())
	{
		AfxMessageBox(_T("���ݿ�δ����"));
		return 0;
	}
	if (strCommand.find("select") == std::string::npos)
	{
		AfxMessageBox(_T("��ǰ����ֻ��ִ�з�select�����"));
		return 0;
	}
	_variant_t effectLineCount = 0;
	m_pConnection->Execute(_bstr_t(strCommand.c_str()), &effectLineCount, adCmdText);

	return (int)effectLineCount.intVal;
}

/*************************************************************
��������:	ExecuteReader
����˵��:	strCommand: SQL���
�� �� ֵ:	CDBAdoRecordReader: ����ֵ����
����˵��:	ִ��select��ѯ���
*************************************************************/
CDBAdoRecordReader CDBAdoController::ExecuteReader(const std::string& strCommand)
{
	if (!IsConnected())
	{
		AfxMessageBox(_T("���ݿ�δ����"));
		return CDBAdoRecordReader();
	}
	if (strCommand.find("select") != std::string::npos)
	{
		AfxMessageBox(_T("��ǰ����ֻ��ִ��select��ѯ���"));
		return CDBAdoRecordReader();
	}
	_variant_t conn = _variant_t((IDispatch *)m_pConnection, true);

	try
	{
		if (m_pRecordset->State)
		{
			m_pRecordset->Close();
		}
		m_pRecordset->Open(_variant_t(strCommand.c_str()), conn, adOpenStatic, adLockOptimistic, adCmdText);
		return CDBAdoRecordReader(m_pRecordset);
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("��ѯ�����з�������"));
		return CDBAdoRecordReader();
	}

	return CDBAdoRecordReader();
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�
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
std::string CDBAdoController::ConnectStringBuilder(EDatabaseProvider eDB, std::string strDataSource, std::string strIP, bool bHasPwd, std::string strUserName, std::string strPwd)
{
	std::string connectstring;
	switch (eDB)
	{
	case CDBAdoController::eAccess2000:
	{
		connectstring += "Provider=Microsoft.Jet.OLEDB.4.0;DataSource=";
		if (strIP.length() != 0)
		{
			connectstring += "\\\\" + strIP + "\\" + strDataSource + ";";
		}
		else
		{
			connectstring += strDataSource + "\";";
		}
		if (bHasPwd)
		{
			connectstring += strUserName + ";";
			connectstring += strPwd + ";";
		}
	}
	break;
	case CDBAdoController::eAccess2010:
	{
		connectstring += "Provider=Microsoft.ACE.OLEDB.12.0;DataSource=";
		if (strIP.length() != 0)
		{
			connectstring += "\\\\" + strIP + "\\" + strDataSource + ";";
		}
		else
		{
			connectstring += strDataSource + ";";
		}
		if (bHasPwd)
		{
			connectstring += strUserName + ";";
			connectstring += strPwd + ";";
		}
	}
	break;
	case CDBAdoController::eODBC:
	{
		//FIXIT: Զ�������ַ��������
		connectstring += "Provider=MADASQL;DSN=" + strDataSource + ";UID=" +
			strUserName + ";PWD=" + strPwd + ";";
	}
		break;
	case CDBAdoController::eOracle:
	{
		//FIXIT: Զ�������ַ��������
		connectstring += "Provider=MSDAORA;DataSource=" + strDataSource + ";User ID=" +
			strUserName + ";Password=" + strPwd + ";";

	}
		break;
	case CDBAdoController::eSqlServer:
	{
		if (strUserName != "")
		{
			connectstring += "Provider=SQLOLEDB;DataSource=" + strIP + ";Initial Catalog=" +
				strDataSource + ";UserID=" + strUserName + ";Password=" + strPwd + ";";
		}
		else
		{
			connectstring += "Provider=SQLOLEDB;DataSource=.;Initial Catalog=" +
				strDataSource + ";Integrated Security=SSPI;";
		}

	}
		break;
	default:
		break;
	}
	return connectstring;

}

#pragma endregion ˽�нӿ�

#pragma endregion CDBAdoController


#pragma region CDBAdoRecordReader
CDBAdoRecordReader::CDBAdoRecordReader()
	: m_recordSetPtr(nullptr)
{

}

CDBAdoRecordReader::CDBAdoRecordReader(_RecordsetPtr ptr)
	: m_recordSetPtr(ptr)
{

}

CDBAdoRecordReader::CDBAdoRecordReader(const CDBAdoRecordReader& lhs)
{
	m_recordSetPtr = lhs.m_recordSetPtr->Clone(adLockOptimistic);
}

CDBAdoRecordReader::~CDBAdoRecordReader()
{
	if (m_recordSetPtr->State)
	{
		m_recordSetPtr->Close();
		m_recordSetPtr.Release();
	}
}

#pragma region �����ӿ�
bool CDBAdoRecordReader::MoveNext()
{
	if (!m_recordSetPtr)
	{
		return false;
	}

	HRESULT hr = m_recordSetPtr->MoveNext();

	if (!SUCCEEDED(hr))
	{
		return false;
	}
	if (m_recordSetPtr->ADOEOF)
	{
		return false;
	}
	return true;
}


bool CDBAdoRecordReader::MoveFirst()
{
	if (!m_recordSetPtr)
	{
		return false;
	}
	m_recordSetPtr->MoveFirst();
	return true;
}

int CDBAdoRecordReader::RecordCount()
{
	if (!m_recordSetPtr)
	{
		return 0;
	}
	return m_recordSetPtr->RecordCount;
}

int CDBAdoRecordReader::FieldCount()
{
	if (!m_recordSetPtr)
	{
		return 0;
	}
	return m_recordSetPtr->Fields->Count;
}

_variant_t CDBAdoRecordReader::operator[](const size_t nIndex) const
{
	try
	{
		return m_recordSetPtr->GetCollect(_variant_t((long)nIndex));
	}
	catch (_com_error e)
	{
		//std::cerr << e.Description() << std::endl;
		return NULL;
	}
	return NULL;
}

_variant_t CDBAdoRecordReader::operator[](const std::string key) const
{
	try
	{
		return m_recordSetPtr->GetCollect(_variant_t(key.c_str()));
	}
	catch (_com_error e)
	{
		//std::cerr << e.Description() << std::endl;
		return NULL;
	}
	return NULL;
}

#pragma endregion �����ӿ�


#pragma region ˽�нӿ�

#pragma endregion ˽�нӿ�

#pragma endregion CDBAdoRecordReader