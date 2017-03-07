// DlgLogin.cpp : 实现文件
//
#include "stdafx.h"
#include "Student.h"
#include "afxdialogex.h"
#include "DlgLogin.h"
#include "DlgReg.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/define/MsgInfo.h"

#include <string>

// CDlgLogin 对话框

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogin::IDD, pParent)
{

}

CDlgLogin::~CDlgLogin()
{
}

/*************************************************************
功能说明:	返回登陆成功之后的用户信息的引用
*************************************************************/
ST_ShowUserInfo& CDlgLogin::GetShowUserInfo()
{
	return m_stShowUserInfo;
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlgLogin::OnLogin)
	ON_BN_CLICKED(IDC_BTN_REG, &CDlgLogin::OnRegUser)
	ON_MESSAGE(EWND_MSG_LOGIN, &CDlgLogin::OnWndMsgLogin)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序


void CDlgLogin::OnLogin()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strName = _T("");
	CString strPwd = _T("");

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
	if (strName.IsEmpty()) 
	{
		AfxMessageBox(_T("账号不能为空"));
		return;
	}
	else if (strName.GetLength() > 30)
	{
		AfxMessageBox(_T("账号长度不能超过30个字符"));
		return;
	}
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);
	if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	else if (strPwd.GetLength() > 30)
	{
		AfxMessageBox(_T("密码长度不能超过30个字符"));
		return;
	}
	
	// 准备将数据传给服务器进行判断
	CMsgHelperClient& msgHelper = CMsgHelperClient::GetInstance();

	// 1、将当前窗口句柄交给消息处理器，用于接收消息
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2、将数据转换成string的格式
	CStringA strNameA(strName);
	CStringA strPwdA(strPwd);

	ST_MsgLogin msg;

	memcpy(msg.stLoginInfo.strUserName, strNameA.GetBuffer(), strNameA.GetLength());
	memcpy(msg.stLoginInfo.strUserPwd, strPwdA.GetBuffer(), strPwdA.GetLength());

	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgLogin));
}


void CDlgLogin::OnRegUser()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgReg dlg;
	if (IDOK == dlg.DoModal())
	{

	}

}


LRESULT CDlgLogin::OnWndMsgLogin(WPARAM wParam, LPARAM lParam)
{
	if (wParam)
	{
		memcpy(&m_stShowUserInfo, (void*)wParam, sizeof(ST_ShowUserInfo));
	}
	CDialogEx::OnOK();
	return 0;
}

BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // 屏蔽回车
			OnLogin();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
