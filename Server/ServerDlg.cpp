
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#include <string>

#include "Logic/MsgHelperMain.h"
#include "DataBase/DBMySQL.h"
#include "BLL/CoreDefine.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerDlg �Ի���



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ME, m_static_me);
	DDX_Control(pDX, IDC_STATIC_T, m_static_t);
	DDX_Control(pDX, IDC_STATIC_S, m_static_s);
	DDX_Control(pDX, IDC_EDIT_ME, m_edit_me);
	DDX_Control(pDX, IDC_EDIT_T, m_edit_t);
	DDX_Control(pDX, IDC_EDIT_S, m_edit_s);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(EWND_MSG_SERVER_RECV, &CServerDlg::OnMsgServerRecv)
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	int nSW = GetSystemMetrics(SM_CXSCREEN);
	int nSH = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(static_cast<int>(nSW * 0.1), static_cast<int>(nSH * 0.1), static_cast<int>(nSW * 0.8), static_cast<int>(nSH * 0.8));
	
	std::string strReturn = "";
	// �������ݿ�
	if (!CDBMySQL::GetInstance().ConnMySQL("192.168.18.100", 3306, "multimediateach", "test", "123456", "UTF-16", strReturn))
	{
		m_edit_me.SetWindowText(_T("���ݿ����ӳɹ�"));
	}
	else
	{
		CStringA strA(strReturn.c_str());
		CString str(strA);
		m_edit_me.SetWindowText(str);
	}
	
	CMsgHelperMain& msgHelper = CMsgHelperMain::GetInstance();
	msgHelper.SetHwnd(this->GetSafeHwnd());

	CString str;
	m_edit_me.GetWindowText(str);
	str += _T("\r\n");

	if (CTCPNet::GetInstance().InitNet(1234, &msgHelper))
	{
		str += _T("�����������ɹ�");
		m_edit_me.SetWindowText(str);
	}
	else
	{
		str += _T("����������ʧ��");
		m_edit_me.SetWindowText(str);
	}
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	int nW = static_cast<int>((cx - 80) / 3);
	int nH = cy - 70;
	int nX = 20;

	if (m_static_me.GetSafeHwnd())
	{
		m_static_me.MoveWindow(nX, 20, 150, 30);
		nX += 20 + nW;
	}
	if (m_static_t.GetSafeHwnd())
	{
		m_static_t.MoveWindow(nX, 20, 150, 30);
		nX += 20 + nW;
	}
	if (m_static_s.GetSafeHwnd())
	{
		m_static_s.MoveWindow(nX, 20, 150, 30);
		nX = 20;
	}
	if (m_edit_me.GetSafeHwnd())
	{
		m_edit_me.MoveWindow(nX, 50, nW, nH);
		nX += 20 + nW;
	}
	if (m_edit_t.GetSafeHwnd())
	{
		m_edit_t.MoveWindow(nX, 50, nW, nH);
		nX += 20 + nW;
	}
	if (m_edit_s.GetSafeHwnd())
	{
		m_edit_s.MoveWindow(nX, 50, nW, nH);
	}
}


void CServerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  �ڴ˴������Ϣ����������

	// �Ͽ����ݿ�����
	CDBMySQL::GetInstance().DisConnMySQL();
}


afx_msg LRESULT CServerDlg::OnMsgServerRecv(WPARAM wParam, LPARAM lParam)
{
	ST_MsgHead* pHead = (ST_MsgHead*)wParam;
	if (!pHead)
	{
		return 0;
	}
	CWnd* pWnd = &m_edit_t;
	if (pHead->clientType == eStudent)
	{
		pWnd = &m_edit_s;
	}

	CString str;
	pWnd->GetWindowText(str);
	if (!str.IsEmpty())
	{
		str += _T("\r\n");
	}
	CStringA strA(((std::string*)lParam)->c_str());
	str += strA;
	pWnd->SetWindowText(str);
	return 0;
}
