
// StudentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Teacher.h"
#include "afxdialogex.h"
#include "DlgMain.h"

#include "UI/DlgSetting.h"

#include "Logic/MsgHelperMain.h"

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
	DDX_Control(pDX, IDC_BTN_LOCK, m_btn_lock);
	DDX_Control(pDX, IDC_BTN_SHOW_DEMO, m_btn_show_demo);
	DDX_Control(pDX, IDC_BTN_ROLL_CALL, m_btn_roll_call);
	DDX_Control(pDX, IDC_BTN_ASK, m_btn_ask);
	DDX_Control(pDX, IDC_BTN_PEN, m_btn_pen);
	DDX_Control(pDX, IDC_BTN_QUESTION, m_btn_question);
	DDX_Control(pDX, IDC_BTN_FILE_TRANS, m_btn_file_trans);
	DDX_Control(pDX, IDC_BTN_SETTING, m_btn_setting);

	DDX_Control(pDX, IDC_LIST_CMP, m_listCtrl);

}

BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgMain::OnServerMsgResult)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CDlgMain::OnBnClickedBtnSetting)
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

	CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());

	m_listCtrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, 50);
	m_listCtrl.InsertColumn(1, _T("����"), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(2, _T("״̬"), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(3, _T("IP��ַ"), LVCFMT_LEFT, 150);

	MoveWindow(static_cast<int>(nSW * 0.1), static_cast<int>(nSH * 0.1), static_cast<int>(nSW * 0.8), static_cast<int>(nSH * 0.8));

	AskForClientList();

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
	int nH = cy;
	int nX = 20;
	int nY = 20;

	MoveBtn(m_btn_lock, nX, nY, cx);
	MoveBtn(m_btn_show_demo, nX, nY, cx);
	MoveBtn(m_btn_roll_call, nX, nY, cx);
	MoveBtn(m_btn_ask, nX, nY, cx);
	MoveBtn(m_btn_pen, nX, nY, cx);
	MoveBtn(m_btn_question, nX, nY, cx);
	MoveBtn(m_btn_file_trans, nX, nY, cx);
	MoveBtn(m_btn_setting, nX, nY, cx, TRUE);


	nY += m_nBtnHeight + 20;
	nH = cy - nY - 20;
	if (m_listCtrl.GetSafeHwnd())
	{
		m_listCtrl.MoveWindow(20, nY, nW, nH);
	}

}


LRESULT CDlgMain::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
{
	switch ((EMsgType)lParam)
	{
	case eMsgAskClientListResult:
	{
		UpdateList(wParam);
	}
	break;

	default:
		break;
	}

	return 0;
}


#pragma region
/*************************************************************
��������:	SearchUserList
����˵��:	vecClient��OUT��: ������������ز�ѯ�����û��б�
�� �� ֵ:	void 
����˵��:	��ѯ�û��б����ڼ�¼��ʾ
*************************************************************/
void CDlgMain::AskForClientList()
{
	ST_MsgHead stHead;
	stHead.clientType = eTeacher;
	stHead.msgType = eMsgAskClientList;
	CTCPNet::GetInstance().SendToServer(&stHead, sizeof(ST_MsgHead));
}

// ���½����б�
void CDlgMain::UpdateList(WPARAM wParam)
{
	m_listCtrl.DeleteAllItems();

	ST_MsgAskClientListResult& st = *((ST_MsgAskClientListResult*)wParam);

	for (int i = 0; i < st.nSize; i++)
	{
		CString str;
		CStringA strA;
		str.Format(_T("%d"), i + 1);
		int nRow = m_listCtrl.InsertItem(0, str);
		if (st.arrClient[i].eCT == eTeacher)
		{
			str = _T("��ʦ");
		}
		else if (st.arrClient[i].eCT == eStudent)
		{
			str = _T("ѧ��");
		}
		m_listCtrl.SetItemText(nRow, 1, str);
		switch (st.arrClient[i].eCS)
		{
		case eClientConnect:
		{
			str = _T("������");
		}
		break;
		case eClientDisConnect:
		{
			str = _T("�ѶϿ�");
		}
		break;
		case eClientLogin:
		{
			str = _T("�ѵ�½");
		}
		break;
		default:
			break;
		}
		m_listCtrl.SetItemText(nRow, 2, str);
		strA = st.arrClient[i].arrIP;
		str = strA;
		m_listCtrl.SetItemText(nRow, 3, str);
	}

}

// ������ťλ��
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

void CDlgMain::OnBnClickedBtnSetting()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDlgSetting dlg;
	if (dlg.DoModal() == IDOK)
	{
		AskForClientList();
	}
}
