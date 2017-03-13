// UI/DlgDisplay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Teacher.h"
#include "DlgDisplay.h"
#include "afxdialogex.h"

#include "BLL/CoreDefine.h"


// CDlgDisplay �Ի���

IMPLEMENT_DYNAMIC(CDlgDisplay, CDialogEx)

CDlgDisplay::CDlgDisplay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDisplay::IDD, pParent)
	, m_bIsFullScreen(false)
{

}

CDlgDisplay::~CDlgDisplay()
{
}

void CDlgDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDisplay, CDialogEx)
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgDisplay::OnServerMsgResult)
END_MESSAGE_MAP()


// CDlgDisplay ��Ϣ�������


BOOL CDlgDisplay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

// ������Ϣ�ĺ���
LRESULT CDlgDisplay::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
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

void CDlgDisplay::SetIsFullScreen(bool bFlag)
{
	m_bIsFullScreen = bFlag;
}


BOOL CDlgDisplay::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	// WM_SYSKEYDOWN  ��ʾALT������
	if (WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

