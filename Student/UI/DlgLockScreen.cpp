// UI/DlgLockScreen.cpp : 实现文件
//

#include "stdafx.h"
#include "Student.h"
#include "DlgLockScreen.h"
#include "afxdialogex.h"

#include "BLL/CoreDefine.h"

// CDlgLockScreen 对话框

IMPLEMENT_DYNAMIC(CDlgLockScreen, CDialogEx)

CDlgLockScreen::CDlgLockScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLockScreen::IDD, pParent)
	, m_bIsFullScreen(false)
{

}

CDlgLockScreen::~CDlgLockScreen()
{
}

void CDlgLockScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLockScreen, CDialogEx)
//	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgLockScreen 消息处理程序


BOOL CDlgLockScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	// 异常:  OCX 属性页应返回 FALSE
}

void CDlgLockScreen::SetIsFullScreen(bool bFlag)
{
	m_bIsFullScreen = bFlag;
}


BOOL CDlgLockScreen::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	// WM_SYSKEYDOWN  表示ALT键按下
	if (WM_SYSKEYDOWN == pMsg->message && VK_F4 == pMsg->wParam)
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgLockScreen::OnPaint()
{
	CPaintDC dc(this); 

// 	struct	ST_GdiClientList	*pNode;
// 	int					iX1, iX2, iY1, iY2;
// 
// 	auto gdi = CGdiClient::GetInstance();
// 
// 	if (gdi.fDIBitmap)
// 	{
// 		CRect rc;
// 		GetClientRect(&rc);
// 		if (!gdi.fConnected)
// 		{
// 			m_hMemDC = CreateCompatibleDC(dc);
// 			m_hDDBitmap = CreateCompatibleBitmap(dc, gdi.iScreenWidth, gdi.iScreenHeight);
// 			SelectObject(m_hMemDC, m_hDDBitmap);
// 		}
// 		pNode = gdi.GdiStart.pNext;
// 		while (pNode)
// 		{
// 			if (pNode->Gdi.fDIBitmap)
// 			{
// 				StretchDIBits(m_hMemDC,
// 					pNode->Gdi.iWidth1, pNode->Gdi.iHeight1,
// 					pNode->Gdi.lpBitmapIH->biWidth, pNode->Gdi.lpBitmapIH->biHeight,
// 					0, 0,
// 					pNode->Gdi.lpBitmapIH->biWidth, pNode->Gdi.lpBitmapIH->biHeight,
// 					(LPBYTE)pNode->Gdi.lpBitmapIH + (pNode->Gdi.lpBitmapIH->biSize + (1 << pNode->Gdi.lpBitmapIH->biBitCount) * sizeof(RGBQUAD)),
// 					(LPBITMAPINFO)pNode->Gdi.lpBitmapIH, DIB_RGB_COLORS, SRCCOPY);
// 			}
// 
// 			if (gdi.fGridX)
// 			{
// 				iX1 = pNode->Gdi.iWidth1;
// 				iY1 = pNode->Gdi.iHeight1;
// 				iY2 = pNode->Gdi.iHeight2;
// 
// 				MoveToEx(m_hMemDC, iX1, iY1, NULL);
// 				LineTo(m_hMemDC, iX1, iY2);
// 			}
// 
// 			if (gdi.fGridY)
// 			{
// 				iX1 = pNode->Gdi.iWidth1;
// 				iX2 = pNode->Gdi.iWidth2;
// 				iY1 = pNode->Gdi.iHeight1;
// 				MoveToEx(m_hMemDC, iX1, iY1, NULL);
// 				LineTo(m_hMemDC, iX2, iY1);
// 			}
// 
// 			pNode = pNode->pNext;
// 		}
// 
// 		BitBlt(dc, 0, 0, gdi.iScreenWidth, gdi.iScreenHeight, m_hMemDC, 0, 0, SRCCOPY);
// 		if (!gdi.fConnected)
// 		{
// 			DeleteObject(m_hDDBitmap);
// 			DeleteDC(m_hMemDC);
// 		}
// 	}


}
