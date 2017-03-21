// DlgQuestion.cpp : 实现文件
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgQuestion.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CDlgQuestion 消息处理程序
