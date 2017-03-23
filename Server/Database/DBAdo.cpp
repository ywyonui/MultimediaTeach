/***************************************************************************************/
/**
	文件名称:	DBADO.cpp	
	创建时间:	2016/10/19	21:23:23
	作    者:	ywyonui
	功能说明:	
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

#pragma region 公共接口
/*************************************************************
函数名称:	GetInstance
参数说明:	void
返 回 值:	CDBAdoController:	服务器消息处理对象
功能说明:	返回单例对象
*************************************************************/
CDBAdoController& CDBAdoController::GetInstance()
{
	static CDBAdoController m_hInstance;
	return m_hInstance;
}

/*************************************************************
函数名称:	Init
参数说明:	void
返 回 值:	bool
功能说明:	初始化对象
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
函数名称:	Connect
参数说明:	strConnect:连接使用的字符串
返 回 值:	bool
功能说明:	连接数据库
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
		// 连接数据库失败
		CStringA strA(e.ErrorMessage());
		CString str(strA);
		AfxMessageBox(str);
		return false;
	}
}

/*************************************************************
函数名称:	Connect
参数说明:	eDB:数据库类型，
			strDataSource：数据库路径，
			strIP：数据库IP，
			bHasPwd：是否使用账号密码，
			strUserName：数据库用户名，
			strPwd：数据库用户密码
返 回 值:	bool
功能说明:	初始化对象
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
函数名称:	IsConnect
参数说明:	void
返 回 值:	bool
功能说明:	当前是否已经连接
*************************************************************/
bool CDBAdoController::IsConnected() const
{
	return static_cast<bool>(m_pConnection->State != 0);
}

/*************************************************************
函数名称:	ExecuteNonQuery
参数说明:	strCommand: SQL语句
返 回 值:	int: 影响行数
功能说明:	执行非select查询语句
*************************************************************/
int CDBAdoController::ExecuteNonQuery(const std::string& strCommand)
{
	if (!IsConnected())
	{
		AfxMessageBox(_T("数据库未连接"));
		return 0;
	}
	if (strCommand.find("select") == std::string::npos)
	{
		AfxMessageBox(_T("当前函数只能执行非select的语句"));
		return 0;
	}
	_variant_t effectLineCount = 0;
	m_pConnection->Execute(_bstr_t(strCommand.c_str()), &effectLineCount, adCmdText);

	return (int)effectLineCount.intVal;
}

/*************************************************************
函数名称:	ExecuteReader
参数说明:	strCommand: SQL语句
返 回 值:	CDBAdoRecordReader: 返回值容器
功能说明:	执行select查询语句
*************************************************************/
CDBAdoRecordReader CDBAdoController::ExecuteReader(const std::string& strCommand)
{
	if (!IsConnected())
	{
		AfxMessageBox(_T("数据库未连接"));
		return CDBAdoRecordReader();
	}
	if (strCommand.find("select") != std::string::npos)
	{
		AfxMessageBox(_T("当前函数只能执行select查询语句"));
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
		AfxMessageBox(_T("查询过程中发生错误"));
		return CDBAdoRecordReader();
	}

	return CDBAdoRecordReader();
}

#pragma endregion 公共接口


#pragma region 私有接口
/*************************************************************
函数名称:	ConnectStringBuilder
参数说明:	eDB:数据库类型，
			strDataSource：数据库路径，
			strIP：数据库IP，
			bHasPwd：是否使用账号密码，
			strUserName：数据库用户名，
			strPwd：数据库用户密码
返 回 值:	string: 连接使用的字符串
功能说明:	组合连接字符串
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
		//FIXIT: 远程连接字符串待添加
		connectstring += "Provider=MADASQL;DSN=" + strDataSource + ";UID=" +
			strUserName + ";PWD=" + strPwd + ";";
	}
		break;
	case CDBAdoController::eOracle:
	{
		//FIXIT: 远程连接字符串待添加
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

#pragma endregion 私有接口

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

#pragma region 公共接口
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

#pragma endregion 公共接口


#pragma region 私有接口

#pragma endregion 私有接口

#pragma endregion CDBAdoRecordReader