#pragma once


// CDlgLockScreen �Ի���

class CDlgLockScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLockScreen)

public:
	CDlgLockScreen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLockScreen();

// �Ի�������
	enum { IDD = IDD_DLG_LOCK_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

private:
	bool	m_bIsFullScreen;

	HDC		m_hMemDC;
	HBITMAP	m_hDDBitmap;

	int		iVScrollPos, iHScrollPos;
	RECT	m_Rect;

	long	m_iTimerInterval;

public:
	void SetIsFullScreen(bool bFlag);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
};
