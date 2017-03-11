#pragma once


// CDlgLockScreen 对话框

class CDlgLockScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLockScreen)

public:
	CDlgLockScreen(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLockScreen();

// 对话框数据
	enum { IDD = IDD_DLG_LOCK_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	// 自定义消息处理
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);


private:
	bool	m_bIsFullScreen;

public:
	void SetIsFullScreen(bool bFlag);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
