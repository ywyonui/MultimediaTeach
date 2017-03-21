
// ServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	CStatic m_static_me;
	CStatic m_static_t;
	CStatic m_static_s;
	CEdit m_edit_me;
	CEdit m_edit_t;
	CEdit m_edit_s;

public:
	static CString GetModulePath();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	afx_msg LRESULT OnMsgServerRecv(WPARAM wParam, LPARAM lParam);
};
