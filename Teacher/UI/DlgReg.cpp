// DlgReg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgReg.h"
#include "afxdialogex.h"

#include "Logic/MsgHelperMain.h"
#include "BLL/CoreDefine.h"


// CDlgReg �Ի���

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


// CDlgReg ��Ϣ�������



void CDlgReg::OnRegCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

void CDlgReg::OnRegUser()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strName = _T("");
	CString strPetName = _T("");
	CString strPwd = _T("");
	CString strRePwd = _T("");

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
	GetDlgItem(IDC_EDIT_PET_NAME)->GetWindowText(strPetName);
	if (strPetName.IsEmpty())
	{
		AfxMessageBox(_T("�ǳƲ���Ϊ��"));
		return;
	}
	else if (strPetName.GetLength() > 30)
	{
		AfxMessageBox(_T("�ǳƳ��Ȳ��ܳ���50���ַ�"));
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
	GetDlgItem(IDC_EDIT_REPWD)->GetWindowText(strRePwd);
	if (strRePwd != strPwd)
	{
		AfxMessageBox(_T("������������벻һ��"));
		return;
	}

	// ׼�������ݴ��������������ж�
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();

	// 1������ǰ���ھ��������Ϣ�����������ڽ�����Ϣ
	msgHelper.SetHwnd(this->GetSafeHwnd());

	// 2��������ת����string�ĸ�ʽ
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
