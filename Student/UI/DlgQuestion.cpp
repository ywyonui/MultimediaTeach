// UI/DlgQuestion.cpp : 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "DlgQuestion.h"
#include "afxdialogex.h"

#include "BLL/CoreDefine.h"
#include "Logic/MsgHelperMain.h"


// CDlgQuestion 对话框

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


// CDlgQuestion 消息处理程序


void CDlgQuestion::OnBnClickedBtnSubmit()
{
	// TODO:  在此添加控件通知处理程序代码

}


BOOL CDlgQuestion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(m_strTitle);
	GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText(m_strContent);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
