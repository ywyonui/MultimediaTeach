#pragma once


// CDlgDisplay 对话框

class CDlgDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisplay)

public:
	CDlgDisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDisplay();

// 对话框数据
	enum { IDD = IDD_DLG_DISPLAY };

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
