
// StudentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "afxdialogex.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgMain 对话框



CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nBtnWidth = 80;
	m_nBtnHeight = 30;
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_HAND_S, m_btn_hand_s);
	DDX_Control(pDX, IDC_BTN_SUBMIT_S, m_btn_submit_s);
	DDX_Control(pDX, IDC_BTN_QUESTION_S, m_btn_question_s);

	DDX_Control(pDX, IDC_BTN_LOCK_T, m_btn_lock_t);
	DDX_Control(pDX, IDC_BTN_SHOW_DEMO_T, m_btn_show_demo_t);
	DDX_Control(pDX, IDC_BTN_ROLL_CALL_T, m_btn_roll_call_t);
	DDX_Control(pDX, IDC_BTN_ASK_T, m_btn_ask_t);
	DDX_Control(pDX, IDC_BTN_PEN_T, m_btn_pen_t);
	DDX_Control(pDX, IDC_BTN_QUESTION_T, m_btn_question_t);
	DDX_Control(pDX, IDC_BTN_FILE_TRANS_T, m_btn_file_trans_t);
	DDX_Control(pDX, IDC_BTN_SETTING_T, m_btn_setting_t);

	DDX_Control(pDX, IDC_LIST_CMP, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgMain 消息处理程序

BOOL CDlgMain::OnInitDialog()
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

	if (m_appClientType == eStudent)
	{
		m_btn_lock_t.ShowWindow(SW_HIDE);
		m_btn_show_demo_t.ShowWindow(SW_HIDE);
		m_btn_roll_call_t.ShowWindow(SW_HIDE);
		m_btn_ask_t.ShowWindow(SW_HIDE);
		m_btn_pen_t.ShowWindow(SW_HIDE);
		m_btn_question_t.ShowWindow(SW_HIDE);
		m_btn_file_trans_t.ShowWindow(SW_HIDE);
		m_btn_setting_t.ShowWindow(SW_HIDE);
	}
	else if (m_appClientType == eTeacher)
	{
		m_btn_hand_s.ShowWindow(SW_HIDE);
		m_btn_submit_s.ShowWindow(SW_HIDE);
		m_btn_question_s.ShowWindow(SW_HIDE);
	}


	// 更新列表
	UpdateList(TRUE);




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDlgMain::OnPaint()
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
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	int nW = cx - 40;
	int nH = cy;
	int nX = 20;
	int nY = 20;

	if (m_appClientType == eStudent)
	{
		MoveBtn(m_btn_lock_t, nX, nY, cx);
		MoveBtn(m_btn_show_demo_t, nX, nY, cx);
		MoveBtn(m_btn_roll_call_t, nX, nY, cx);
		MoveBtn(m_btn_ask_t, nX, nY, cx);
		MoveBtn(m_btn_pen_t, nX, nY, cx);
		MoveBtn(m_btn_question_t, nX, nY, cx);
		MoveBtn(m_btn_file_trans_t, nX, nY, cx);
		MoveBtn(m_btn_setting_t, nX, nY, cx, TRUE);
	}
	else if (m_appClientType == eTeacher)
	{
		MoveBtn(m_btn_hand_s, nX, nY, cx);
		MoveBtn(m_btn_submit_s, nX, nY, cx);
		MoveBtn(m_btn_question_s, nX, nY, cx, TRUE);
	}

	nY += m_nBtnHeight + 20;
	nH = cy - nY - 20;
	if (m_listCtrl.GetSafeHwnd())
	{
		m_listCtrl.MoveWindow(20, nY, nW, nH);
	}
}


#pragma region 
// 更新界面列表
void CDlgMain::UpdateList(BOOL bInitFlag)
{
	if (bInitFlag)
	{
		m_listCtrl.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
		m_listCtrl.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 80);
		m_listCtrl.InsertColumn(2, _T("状态"), LVCFMT_LEFT, 80);
		CRect rcClient;
		GetClientRect(rcClient);
		m_listCtrl.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 150);
	}
	else
	{

	}
}

void CDlgMain::MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn)
{
	if (wnd.GetSafeHwnd())
	{
		wnd.MoveWindow(nX, nY, m_nBtnWidth, m_nBtnHeight);
		if (!bIsLastBtn)
		{
			nX += m_nBtnWidth;
			if (nX + m_nBtnWidth > cx - 20)
			{
				nY += m_nBtnHeight + 5;
				nX = 20;
			}
		}
	}
}

#pragma endregion