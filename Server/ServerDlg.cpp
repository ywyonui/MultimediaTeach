
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

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
	DDX_Control(pDX, IDC_STATIC_T, m_static_t);
	DDX_Control(pDX, IDC_STATIC_S, m_static_s);
	DDX_Control(pDX, IDC_EDIT_T, m_edit_t);
	DDX_Control(pDX, IDC_EDIT_S, m_edit_s);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
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
	int nW = static_cast<int>((cx - 60) / 2);
	int nH = cy - 70;

	if (m_static_t.GetSafeHwnd())
	{
		m_static_t.MoveWindow(20, 20, 150, 30);
	}
	if (m_static_s.GetSafeHwnd())
	{
		m_static_s.MoveWindow(40 + nW, 20, 150, 30);
	}
	if (m_edit_t.GetSafeHwnd())
	{
		m_edit_t.MoveWindow(20, 50, nW, nH);
	}
	if (m_edit_s.GetSafeHwnd())
	{
		m_edit_s.MoveWindow(40 + nW, 50, nW, nH);
	}
}
