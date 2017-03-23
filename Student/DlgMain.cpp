
// StudentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "afxdialogex.h"
#include "DlgMain.h"

#include "Logic/CoreDefine.h"

#include "UI/DlgQuestion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDlgMain 对话框
#define IDT_DISPLAY 100


CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMain::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nBtnWidth = 80;
	m_nBtnHeight = 30;
}

CDlgMain::~CDlgMain()
{
	if (m_pLockScreen == NULL)
	{
		delete m_pLockScreen;
		m_pLockScreen = NULL;
	}
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
	ON_MESSAGE(EWND_MSG_CLIENT_RECV, &CDlgMain::OnServerMsgResult)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_HAND, &CDlgMain::OnBnClickedBtnHand)
	ON_BN_CLICKED(IDC_BTN_SUBMIT, &CDlgMain::OnBnClickedBtnSubmit)
	ON_BN_CLICKED(IDC_BTN_QUESTION, &CDlgMain::OnBnClickedBtnQuestion)
END_MESSAGE_MAP()


// CDlgMain 消息处理程序

BOOL CDlgMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	int nSW = GetSystemMetrics(SM_CXSCREEN);
	int nSH = GetSystemMetrics(SM_CYSCREEN);

	CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());

	m_listCtrl.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 50);
	m_listCtrl.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(2, _T("状态"), LVCFMT_LEFT, 80);
	m_listCtrl.InsertColumn(3, _T("IP地址"), LVCFMT_LEFT, 150);

	CRect rc(static_cast<int>(nSW * 0.1), static_cast<int>(nSH * 0.1), static_cast<int>(nSW * 0.9), static_cast<int>(nSH * 0.9));
	MoveWindow(rc);

	AskForClientList();

	if (m_pLockScreen == NULL)
	{
		delete m_pLockScreen;
		m_pLockScreen = NULL;
	}
 	m_pLockScreen = new CDlgLockScreen;
 	m_pLockScreen->Create(IDD_DLG_LOCK_SCREEN, this);
 	m_pLockScreen->MoveWindow(rc);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDlgMain::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDlgMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
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

// 接收消息的函数
LRESULT CDlgMain::OnServerMsgResult(WPARAM wParam, LPARAM lParam)
{
	switch ((EMsgType)lParam)
	{
	case eMsgAskClientListResult:
	{
		UpdateList(wParam);
		CMsgHelperMain::GetInstance().SetHwnd(GetSafeHwnd());
	}
	break;
	case eMsgFileTransmit:
	{
		TransmitFileData(wParam);
	}
	break;
	case eMsgQuestion:
	{
		ShowQuestion(wParam);
	}
	break;
	case eMsgLockScreen:
	{
		m_pLockScreen->ShowWindow(SW_SHOW);
	}
	break;
	case eMsgUnLockScreen:
	{
		m_pLockScreen->ShowWindow(SW_HIDE);
	}
	break;
	case eMsgBeginDisplay:
	{
// 		m_pLockScreen->ShowWindow(SW_SHOW);
// 		CGdiClient::GetInstance().Init(m_pLockScreen->GetSafeHwnd());
// 		CGdiClient::GetInstance().UpdateRegionalScreen(TRUE);
// 		SetTimer(IDT_DISPLAY, 100, 0);
	}
	break;
	case eMsgEndDisplay:
	{
// 		m_pLockScreen->ShowWindow(SW_HIDE);
// 		CGdiClient::GetInstance().Exit();
// 		KillTimer(IDT_DISPLAY);
	}
	break;

	default:
		break;
	}

	return 0;
}

#pragma region 

/*************************************************************
函数名称:	SearchUserList
参数说明:	vecClient（OUT）: 输出参数，返回查询到的用户列表
返 回 值:	void
功能说明:	查询用户列表，用于记录显示
*************************************************************/
void CDlgMain::AskForClientList()
{
	ST_MsgHead stHead;
	stHead.nSubType = eStudent;
	stHead.msgType = eMsgAskClientList;
	CTCPNet::GetInstance().SendToServer(&stHead, sizeof(ST_MsgHead));
}

// 更新界面列表
void CDlgMain::UpdateList(WPARAM wParam)
{
	m_listCtrl.DeleteAllItems();

	ST_MsgAskClientListResult& msg = *((ST_MsgAskClientListResult*)wParam);

	for (int i = 0; i < msg.nSize; i++)
	{
		CString str;
		CStringA strA;
		str.Format(_T("%d"), i + 1);
		int nRow = m_listCtrl.InsertItem(i, str);
		if (msg.arrClient[i].eCT == eTeacher)
		{
			str = _T("教师");
		}
		else if (msg.arrClient[i].eCT == eStudent)
		{
			str = _T("学生");
		}
		m_listCtrl.SetItemText(nRow, 1, str);
		switch (msg.arrClient[i].eCS)
		{
		case eClientConnect:
		{
			str = _T("已连接");
		}
		break;
		case eClientDisConnect:
		{
			str = _T("已断开");
		}
		break;
		case eClientLogin:
		{
			str = _T("已登陆");
		}
		break;
		default:
			break;
		}
		m_listCtrl.SetItemText(nRow, 2, str);
		strA = msg.arrClient[i].arrIP;
		str = strA;
		m_listCtrl.SetItemText(nRow, 3, str);
	}

}

void CDlgMain::TransmitFileData(WPARAM wParam)
{
	ST_MsgFileTransmit& msg = *((ST_MsgFileTransmit*)wParam);

	m_vecFileData.push_back(msg);
	if (msg.stMsgHead.nSubType == MAX_FILE_TRANS_SIZE)
	{
		return;
	}
	CString strFilePath = CStudentApp::GetModulePath();
	CStringA strFileNameA(msg.arrFileName);
	strFilePath += _T("\\");
	strFilePath += strFileNameA;

	CFile file;
	BOOL bFlag = file.Open(strFilePath, CFile::typeBinary | CFile::modeWrite | CFile::modeCreate);
	if (!bFlag)
	{
		MessageBox(_T("打开或创建文件失败"));
		return;
	}
	TRACE("recv file data %d\n", msg.stMsgHead.nSubType);

	file.Seek(CFile::end, 0);
	for (auto& it: m_vecFileData)
	{
		file.Write(it.arrData, it.stMsgHead.nSubType);
	}
	m_vecFileData.clear();

	file.Close();
}

void CDlgMain::ShowQuestion(WPARAM wParam)
{
	ST_MsgFileTransmit& msg = *((ST_MsgFileTransmit*)wParam);
	
	CStringA strTitleA = msg.arrFileName;
	CStringA strContentA = msg.arrData;

	CString strTitle(strTitleA);
	CString strContent(strContentA);

	CDlgQuestion dlg(this);
	dlg.SetQuestion(strTitle, strContent);
	dlg.DoModal();
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

void CDlgMain::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	if (m_pLockScreen == NULL)
	{
		m_pLockScreen->SendMessage(WM_DESTROY);
		delete m_pLockScreen;
		m_pLockScreen = NULL;
	}
}


void CDlgMain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDT_DISPLAY)
	{
//		CGdiClient::GetInstance().UpdateRegionalScreen(FALSE);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgMain::OnBnClickedBtnHand()
{
	// TODO:  在此添加控件通知处理程序代码
	MessageBox(_T("后续功能尚在开发中，敬请期待！"));
}


void CDlgMain::OnBnClickedBtnSubmit()
{
	// TODO:  在此添加控件通知处理程序代码
	MessageBox(_T("后续功能尚在开发中，敬请期待！"));
}


void CDlgMain::OnBnClickedBtnQuestion()
{
	// TODO:  在此添加控件通知处理程序代码
	MessageBox(_T("后续功能尚在开发中，敬请期待！"));
}
