
// StudentDlg.h : 头文件
//

#pragma once
#include <vector>
#include "BLL/CoreDefine.h"

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
	CButton m_btn_lock;			// 点击锁屏
	CButton m_btn_display;		// 点击演示
	CButton m_btn_roll_call;	// 点击点名
	CButton m_btn_ask;			// 点击提问
	CButton m_btn_pen;			// 点击教鞭
	CButton m_btn_question;		// 点击进入问题库
	CButton m_btn_file_trans;	// 点击文件传输
	CButton m_btn_setting;		// 点击配置
	CListCtrl m_listCtrl;

	CImageList	m_listImage;


private:
	int		m_nBtnWidth;
	int		m_nBtnHeight;


protected:

	/*************************************************************
	函数名称:	SearchUserList
	参数说明:	vecClient（OUT）: 输出参数，返回查询到的用户列表
	返 回 值:	void
	功能说明:	查询用户列表，用于记录显示
	*************************************************************/
	void AskForClientList();

	// 更新列表的数据，初始化插入标题，否则插入数据
	void UpdateList(WPARAM wParam);

	// 调整按钮位置的通用函数
	void MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// 自定义消息处理
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);

protected:
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnLock();
	afx_msg void OnBnClickedBtnDisplay();
	afx_msg void OnBnClickedBtnRollCall();
	afx_msg void OnBnClickedBtnAsk();
	afx_msg void OnBnClickedBtnPen();
	afx_msg void OnBnClickedBtnQuestion();
	afx_msg void OnBnClickedBtnFileTrans();
public:
	virtual BOOL DestroyWindow();
};
