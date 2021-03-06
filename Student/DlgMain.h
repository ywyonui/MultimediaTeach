
// StudentDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgMain 对话框
class CDlgMain : public CDialogEx
{
// 构造
public:
	CDlgMain(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected: // 控件
	CButton m_btn_hand;		// 点击举手
	CButton m_btn_submit;	// 点击提交作业
	CButton m_btn_question;	// 点击进入问题库
	CListCtrl m_listCtrl;

	CImageList	m_listImage;

private:
	int		m_nBtnWidth;
	int		m_nBtnHeight;

protected:
	// 更新列表的数据，初始化插入标题，否则插入数据
	void UpdateList(BOOL bInitFlag);

	// 调整按钮位置的通用函数
	void MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE);



// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
};
