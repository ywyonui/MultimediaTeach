// UI/DlgQuestion.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Student.h"
#include "DlgQuestion.h"
#include "afxdialogex.h"

#include "BLL/CoreDefine.h"
#include "Logic/MsgHelperMain.h"


// CDlgQuestion �Ի���

IMPLEMENT_DYNAMIC(CDlgQuestion, CDialogEx)

CDlgQuestion::CDlgQuestion(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgQuestion::IDD, pParent)
{

}

CDlgQuestion::~CDlgQuestion()
{
}

void CDlgQuestion::SetQuestion(const CString& strTitle, const CString& strContent)
{
	m_strTitle = strTitle;
	m_strContent = strContent;
}

void CDlgQuestion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgQuestion, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CDlgQuestion::OnBnClickedBtnSubmit)
END_MESSAGE_MAP()


// CDlgQuestion ��Ϣ�������


void CDlgQuestion::OnBnClickedBtnSubmit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


BOOL CDlgQuestion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(m_strTitle);
	GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(m_strContent);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
