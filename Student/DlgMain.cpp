
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
	m_nBtnWidth = 80;
	m_nBtnHeight = 30;
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_HAND, m_btn_hand);
	DDX_Control(pDX, IDC_BTN_SUBMIT, m_btn_submit);
	DDX_Control(pDX, IDC_BTN_QUESTION, m_btn_question);
	DDX_Control(pDX, IDC_LIST_CMP, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgMain ��Ϣ��������

BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ����Ӷ���ĳ�ʼ������
	int nSW = GetSystemMetrics(SM_CXSCREEN);
	int nSH = GetSystemMetrics(SM_CYSCREEN);

	MoveWindow(static_cast<int>(nSW * 0.1), static_cast<int>(nSH * 0.1), static_cast<int>(nSW * 0.8), static_cast<int>(nSH * 0.8));

	// �����б�
	UpdateList(TRUE);




	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
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

	// TODO:  �ڴ˴�������Ϣ�����������
	int nW = cx - 40;
	int nH = cy;
	int nX = 20;
	int nY = 20;

	MoveBtn(m_btn_hand, nX, nY, cx);

	MoveBtn(m_btn_submit, nX, nY, cx);

	MoveBtn(m_btn_question, nX, nY, cx, TRUE);

	nY += m_nBtnHeight + 20;
	nH = cy - nY - 20;
	if (m_listCtrl.GetSafeHwnd())
	{
		m_listCtrl.MoveWindow(20, nY, nW, nH);
	}
}


#pragma region 
// ���½����б�
void CDlgMain::UpdateList(BOOL bInitFlag)
{
	if (bInitFlag)
	{
		m_listCtrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);
		m_listCtrl.InsertColumn(1, _T("����"), LVCFMT_LEFT, 80);
		m_listCtrl.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, 80);
		CRect rcClient;
		GetClientRect(rcClient);
		m_listCtrl.InsertColumn(3, _T("IP��ַ"), LVCFMT_LEFT, 150);
	}
	else
	{

	}
}


void CDlgMain::MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn)
{
	if (wnd.GetSafeHwnd())
	{
		wnd.MoveWindow(nX, nY, m_nBtnWidth, m_nBtnHeight);
		if (!bIsLastBtn)
		{
			nX += m_nBtnWidth;
			if (nX + m_nBtnWidth > cx - 20)
			{
				nY += m_nBtnHeight + 5;
				nX = 20;
			}
		}
	}
}

#pragma endregion