// UI/DlgQuestion.cpp : 实现文件
//

#include "stdafx.h"
#include "Teacher.h"
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
	CString strTitle;
	CString strContent;

	GetDlgItem(IDC_EDIT_TITLE)->GetWindowText(strTitle);
	GetDlgItem(IDC_EDIT_CONTENT)->GetWindowText(strContent);

	CStringA strTitleA(strTitle);
	CStringA strContentA(strContent);

	ST_MsgFileTransmit msg;
	msg.stMsgHead.msgType = eMsgQuestion;
	memcpy(msg.arrFileName, strTitleA.GetBuffer(), MAX_PATH);
	memcpy(msg.arrData, strContentA.GetBuffer(), MAX_FILE_TRANS_SIZE);

	CTCPNet::GetInstance().SendToServer(&msg, sizeof(ST_MsgFileTransmit));

	CDialogEx::OnOK();
}
