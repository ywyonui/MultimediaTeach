// DlgLogin.cpp : 实现文件
//
#include "stdafx.h"
#include "Teacher.h"
#include "afxdialogex.h"
#include "DlgSetting.h"
#include "DlgReg.h"
#include "Logic/MsgHelperMain.h"

#include "BLL/CoreDefine.h"

#include <string>

// CDlgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSetting, CDialogEx)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetting::IDD, pParent)
	, m_nIPCount(1)
{

}

CDlgSetting::~CDlgSetting()
{
	for (auto& it : m_vecIpAddress)
	{
		if (it)
		{
			delete it;
			it = nullptr;
		}
	}
	m_vecIpAddress.clear();
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit_count);
	DDX_Text(pDX, IDC_EDIT1, m_nIPCount);
	DDV_MinMaxInt(pDX, m_nIPCount, 1, MAX_STUDENT_CLIENT);
	DDX_Control(pDX, IDC_STATIC_COUNT, m_static_count);
	DDX_Control(pDX, IDC_BTN_SUBMIT, m_btn_submit);
}

BEGIN_MESSAGE_MAP(CDlgSetting, CDialogEx)
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgSetting::OnServerMsgResult)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CDlgSetting::OnBnClickedBtnSubmit)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CDlgSetting::OnEnChangeIPAddressCount)
END_MESSAGE_MAP()

#pragma region 其他函数
void MoveIPAddressCtrl(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE)
{
	if (wnd.GetSafeHwnd())
	{
		wnd.MoveWindow(nX, nY, 150, 24);
		if (!bIsLastBtn)
		{
			nX += 160;
			if (nX + 160 > cx - 20)
			{
				nY += 24 + 5;
				nX = 10;
			}
		}
	}
}


#pragma endregion 其他函数


LRESULT CDlgSetting::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL CDlgSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strIniPath = theApp.GetModulePath() + _T("\\..\\config.ini");
	m_nIPCount = GetPrivateProfileInt(_T("Client"), _T("IPCount"), 0, strIniPath);
	UpdateData(FALSE);
	for (int i = 0; i < m_nIPCount; i++)
	{
		CString strName;
		strName.Format(_T("IP%d"), i + 1);
		WCHAR buffer[16];
		GetPrivateProfileString(_T("Client"), strName, _T(""), buffer, 16, strIniPath);
		// 创建IP地址输入框
		CIPAddressCtrl* pIP = new CIPAddressCtrl;
		pIP->Create(WS_VISIBLE | WS_CHILD, CRect(10, 50, 160, 74), this, IP_ADDRESS_ID + i);
		pIP->SetWindowText(buffer);
		m_vecIpAddress.push_back(pIP);
	}

	int nSW = GetSystemMetrics(SM_CXSCREEN);
	int nSH = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(static_cast<int>(nSW * 0.15), static_cast<int>(nSH * 0.15), static_cast<int>(nSW * 0.7), static_cast<int>(nSH * 0.7));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CDlgSetting::OnBnClickedBtnSubmit()
{
	// TODO:  在此添加控件通知处理程序代码
	SaveSettingToConfig();

	CDialogEx::OnOK();
}

void CDlgSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	int nX = 10;
	int nY = 50;

	for (size_t i = 0; i < m_vecIpAddress.size(); i++)
	{
		MoveIPAddressCtrl(*(m_vecIpAddress.at(i)), nX, nY, cx);
	}

}


void CDlgSetting::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
}

void CDlgSetting::SaveSettingToConfig()
{
	CString strIniPath = theApp.GetModulePath() + _T("\\..\\config.ini");
	m_nIPCount = m_vecIpAddress.size();
	CString str;
	str.Format(_T("%d"), m_nIPCount);
	WritePrivateProfileString(_T("Client"), _T("IPCount"), str, strIniPath);
	
	ST_MsgSettingStudentIP msg;
	msg.nSize = m_nIPCount;
	msg.stMsgHead.nSubType = eTeacher;
	
	for (int i = 0; i < m_nIPCount; i++)
	{
		CString strName;
		strName.Format(_T("IP%d"), i + 1);
		m_vecIpAddress.at(i)->GetWindowText(str);
		WritePrivateProfileString(_T("Client"), strName, str, strIniPath);
		CStringA strIPA(str);
		memcpy(msg.arrClient[i], strIPA.GetBuffer(), 16);
	}

	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgSettingStudentIP));
}


void CDlgSetting::OnEnChangeIPAddressCount()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nCount = m_vecIpAddress.size();
	while (m_nIPCount < nCount)
	{
		CIPAddressCtrl* pIp = m_vecIpAddress.at(--nCount);
		if (pIp)
		{
			delete pIp;
			pIp = NULL;
		}
		m_vecIpAddress.pop_back();
	}
	while (m_nIPCount > nCount)
	{
		// 创建IP地址输入框
		CIPAddressCtrl* pIP = new CIPAddressCtrl;
		pIP->Create(WS_VISIBLE | WS_CHILD, CRect(10, 50, 160, 74), this, IP_ADDRESS_ID + nCount++);
		m_vecIpAddress.push_back(pIP);
	}

	CRect rc;
	GetClientRect(rc);
	int cx = rc.Width();
	int nX = 10;
	int nY = 50;

	for (size_t i = 0; i < m_vecIpAddress.size(); i++)
	{
		MoveIPAddressCtrl(*(m_vecIpAddress.at(i)), nX, nY, cx);
	}

}
