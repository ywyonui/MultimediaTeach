
// StudentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Student.h"
#include "afxdialogex.h"
#include "DlgMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgMain �Ի���



CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_HAND, m_btn_hand);
	DDX_Control(pDX, IDC_BTN_SUBMIT, m_btn_submit);
	DDX_Control(pDX, IDC_LIST_T, m_list_t);
	DDX_Control(pDX, IDC_LIST_S, m_list_s);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgMain ��Ϣ�������

BOOL CDlgMain::OnInitDialog()
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

void CDlgMain::OnPaint()
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
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDlgMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	int nW = cx - 40;
	int nH = cy - 210;

	if (m_btn_hand.GetSafeHwnd())
	{
		m_btn_hand.MoveWindow(20, 20, 60, 30);
	}
	if (m_btn_submit.GetSafeHwnd())
	{
		m_btn_submit.MoveWindow(20 + 60, 20, 100, 30);
	}
	if (m_list_t.GetSafeHwnd())
	{
		m_list_t.MoveWindow(20, 70, nW, 100);
	}
	if (m_list_s.GetSafeHwnd())
	{
		m_list_s.MoveWindow(20, 190, nW, nH);
	}
}
