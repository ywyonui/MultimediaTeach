#pragma once


// CDlgDisplay �Ի���

class CDlgDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDisplay)

public:
	CDlgDisplay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDisplay();

// �Ի�������
	enum { IDD = IDD_DLG_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	// �Զ�����Ϣ����
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);


private:
	bool	m_bIsFullScreen;

public:
	void SetIsFullScreen(bool bFlag);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
