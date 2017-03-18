// DlgReg.cpp : 实现文件
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgReg.h"
#include "afxdialogex.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/CoreDefine.h"


// CDlgReg 对话框

IMPLEMENT_DYNAMIC(CDlgReg, CDialogEx)

CDlgReg::CDlgReg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgReg::IDD, pParent)
{

}

CDlgReg::~CDlgReg()
{
}

void CDlgReg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgReg, CDialogEx)
	ON_BN_CLICKED(IDD_DLG_REG, &CDlgReg::OnRegUser)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgReg::OnRegCancel)
END_MESSAGE_MAP()


// CDlgReg 消息处理程序



void CDlgReg::OnRegCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CDlgReg::OnRegUser()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strName = _T("");
	CString strPetName = _T("");
	CString strPwd = _T("");
	CString strRePwd = _T("");

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
	GetDlgItem(IDC_EDIT_PET_NAME)->GetWindowText(strPetName);
	if (strPetName.IsEmpty())
	{
		AfxMessageBox(_T("昵称不能为空"));
		return;
	}
	else if (strPetName.GetLength() > 30)
	{
		AfxMessageBox(_T("昵称长度不能超过50个字符"));
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
	GetDlgItem(IDC_EDIT_REPWD)->GetWindowText(strRePwd);
	if (strRePwd != strPwd)
	{
		AfxMessageBox(_T("两次输入的密码不一致"));
		return;
	}

	// 准备将数据传给服务器进行判断
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	// 1、将当前窗口句柄交给消息处理器，用于接收消息
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2、将数据转换成string的格式
	CStringA strNameA(strName);
	CStringA strPetNameA(strPetName);
	CStringA strPwdA(strPwd);
	
	ST_MsgReg msg;
	msg.stMsgHead.nSubType = eTeacher;

	memcpy(msg.stRegInfo.strUserName, strNameA.GetBuffer(), strNameA.GetLength());
	memcpy(msg.stRegInfo.strPetName, strPetNameA.GetBuffer(), strPetNameA.GetLength());
	memcpy(msg.stRegInfo.strUserPwd, strPwdA.GetBuffer(), strPwdA.GetLength());
	
	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgReg));


}
