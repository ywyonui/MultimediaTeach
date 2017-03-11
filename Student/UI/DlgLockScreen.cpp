// UI/DlgLockScreen.cpp : 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "DlgLockScreen.h"
#include "afxdialogex.h"

#include "BLL/CoreDefine.h"

// CDlgLockScreen 对话框

IMPLEMENT_DYNAMIC(CDlgLockScreen, CDialogEx)

CDlgLockScreen::CDlgLockScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLockScreen::IDD, pParent)
	, m_bIsFullScreen(false)
{

}

CDlgLockScreen::~CDlgLockScreen()
{
}

void CDlgLockScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLockScreen, CDialogEx)
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgLockScreen::OnServerMsgResult)
END_MESSAGE_MAP()


// CDlgLockScreen 消息处理程序


BOOL CDlgLockScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rc;
	if (m_bIsFullScreen)
	{
		rc.left = 0;
		rc.top = 0;
		rc.right = GetSystemMetrics(SM_CXSCREEN);
		rc.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		GetParent()->GetWindowRect(rc);
	}
	MoveWindow(rc);

	CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// 接收消息的函数
LRESULT CDlgLockScreen::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
{
	switch ((EMsgType)lParam)
	{
	case eMsgUnLockScreen:
	{
		PostMessage(WM_COMMAND, IDOK, 0);
	}
	break;

	default:
		break;
	}

	return 0;
}

void CDlgLockScreen::SetIsFullScreen(bool bFlag)
{
	m_bIsFullScreen = bFlag;
}


BOOL CDlgLockScreen::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	// WM_SYSKEYDOWN  表示ALT键按下
	if (WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
