
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#include <string>

#include "Logic/MsgHelperMain.h"
#include "DataBase/DBMySQL.h"
#include "BLL/CoreDefine.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ME, m_static_me);
	DDX_Control(pDX, IDC_STATIC_T, m_static_t);
	DDX_Control(pDX, IDC_STATIC_S, m_static_s);
	DDX_Control(pDX, IDC_EDIT_ME, m_edit_me);
	DDX_Control(pDX, IDC_EDIT_T, m_edit_t);
	DDX_Control(pDX, IDC_EDIT_S, m_edit_s);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(EWND_MSG_SERVER_RECV, &CServerDlg::OnMsgServerRecv)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	int nSW = GetSystemMetrics(SM_CXSCREEN);
	int nSH = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(static_cast<int>(nSW * 0.1), static_cast<int>(nSH * 0.1), static_cast<int>(nSW * 0.8), static_cast<int>(nSH * 0.8));
	
	std::string strReturn = "";
	// 连接数据库
	if (!CDBMySQL::GetInstance().ConnMySQL("192.168.18.100", 3306, "multimediateach", "test", "123456", "UTF-16", strReturn))
	{
		m_edit_me.SetWindowText(_T("数据库连接成功"));
	}
	else
	{
		CStringA strA(strReturn.c_str());
		CString str(strA);
		m_edit_me.SetWindowText(str);
	}
	
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();
	msgHelper.SetHwnd(this->GetSafeHwnd());

	CString str;
	m_edit_me.GetWindowText(str);
	str += _T("\r\n");

	if (CTCPNet::GetInstance().InitNet(1234, &msgHelper))
	{
		str += _T("服务器启动成功");
		m_edit_me.SetWindowText(str);
	}
	else
	{
		str += _T("服务器启动失败");
		m_edit_me.SetWindowText(str);
	}
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	int nW = static_cast<int>((cx - 80) / 3);
	int nH = cy - 70;
	int nX = 20;

	if (m_static_me.GetSafeHwnd())
	{
		m_static_me.MoveWindow(nX, 20, 150, 30);
		nX += 20 + nW;
	}
	if (m_static_t.GetSafeHwnd())
	{
		m_static_t.MoveWindow(nX, 20, 150, 30);
		nX += 20 + nW;
	}
	if (m_static_s.GetSafeHwnd())
	{
		m_static_s.MoveWindow(nX, 20, 150, 30);
		nX = 20;
	}
	if (m_edit_me.GetSafeHwnd())
	{
		m_edit_me.MoveWindow(nX, 50, nW, nH);
		nX += 20 + nW;
	}
	if (m_edit_t.GetSafeHwnd())
	{
		m_edit_t.MoveWindow(nX, 50, nW, nH);
		nX += 20 + nW;
	}
	if (m_edit_s.GetSafeHwnd())
	{
		m_edit_s.MoveWindow(nX, 50, nW, nH);
	}
}


void CServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码

	// 断开数据库连接
	CDBMySQL::GetInstance().DisConnMySQL();
}


afx_msg LRESULT CServerDlg::OnMsgServerRecv(WPARAM wParam, LPARAM lParam)
{
	ST_MsgHead* pHead = (ST_MsgHead*)wParam;
	if (!pHead)
	{
		return 0;
	}
	CWnd* pWnd = &m_edit_t;
	if (pHead->clientType == eStudent)
	{
		pWnd = &m_edit_s;
	}

	CString str;
	pWnd->GetWindowText(str);
	if (!str.IsEmpty())
	{
		str += _T("\r\n");
	}
	CStringA strA(((std::string*)lParam)->c_str());
	str += strA;
	pWnd->SetWindowText(str);
	return 0;
}
