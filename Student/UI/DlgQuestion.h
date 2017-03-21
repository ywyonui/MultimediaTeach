#pragma once


// CDlgQuestion �Ի���

class CDlgQuestion : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQuestion)

public:
	CDlgQuestion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgQuestion();

// �Ի�������
	enum { IDD = IDD_DLG_QUESTION };


private:
	CString m_strTitle;
	CString m_strContent;

public:
	void SetQuestion(const CString& strTitle, const CString& strContent);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSubmit();
	virtual BOOL OnInitDialog();
};
