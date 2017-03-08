// DlgLogin.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Teacher.h"
#include "afxdialogex.h"
#include "DlgLogin.h"
#include "DlgReg.h"

#include "Logic/MsgHelperMain.h"

#include "BLL/define/EUIMsg.h"


#include <string>

// CDlgLogin �Ի���

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogin::IDD, pParent)
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlgLogin::OnLogin)
	ON_BN_CLICKED(IDC_BTN_REG, &CDlgLogin::OnRegUser)
	ON_MESSAGE(EWND_MSG_LOGIN_SUCCESS, &CDlgLogin::OnWndMsgLogin)
END_MESSAGE_MAP()


// CDlgLogin ��Ϣ�������


void CDlgLogin::OnLogin()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strName = _T("");
	CString strPwd = _T("");

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strName);
	if (strName.IsEmpty()) 
	{
		AfxMessageBox(_T("�˺Ų���Ϊ��"));
		return;
	}
	else if (strName.GetLength() > 30)
	{
		AfxMessageBox(_T("�˺ų��Ȳ��ܳ���30���ַ�"));
		return;
	}
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);
	if (strPwd.IsEmpty())
	{
		AfxMessageBox(_T("���벻��Ϊ��"));
		return;
	}
	else if (strPwd.GetLength() > 30)
	{
		AfxMessageBox(_T("���볤�Ȳ��ܳ���30���ַ�"));
		return;
	}
	
	// ׼�������ݴ��������������ж�
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	// 1������ǰ���ھ��������Ϣ�����������ڽ�����Ϣ
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2��������ת����string�ĸ�ʽ
	CStringA strNameA(strName);
	CStringA strPwdA(strPwd);

	ST_MsgLogin msg;
	msg.stMsgHead.clientType = eTeacher;

	memcpy(msg.stLoginInfo.strUserName, strNameA.GetBuffer(), strNameA.GetLength());
	memcpy(msg.stLoginInfo.strUserPwd, strPwdA.GetBuffer(), strPwdA.GetLength());

	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgLogin));
}


void CDlgLogin::OnRegUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDlgReg dlg;
	if (IDOK == dlg.DoModal())
	{

	}

}


LRESULT CDlgLogin::OnWndMsgLogin(WPARAM wParam, LPARAM lParam)
{
	CDialogEx::OnOK();
	return 0;
}

BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:    // ���λس�
			OnLogin();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
