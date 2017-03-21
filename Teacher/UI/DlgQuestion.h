#pragma once


// CDlgQuestion 对话框

class CDlgQuestion : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQuestion)

public:
	CDlgQuestion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuestion();

// 对话框数据
	enum { IDD = IDD_DLG_QUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSubmit();
};
