
// StudentDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BLL/define/EnumType.h"

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
#pragma region 学生端
	CButton m_btn_hand_s;		// 点击举手
	CButton m_btn_submit_s;		// 点击提交作业
	CButton m_btn_question_s;	// 点击进入问题库
#pragma endregion 学生端

#pragma region 教师端
	CButton m_btn_lock_t;			// 点击锁屏
	CButton m_btn_show_demo_t;		// 点击演示
	CButton m_btn_roll_call_t;		// 点击点名
	CButton m_btn_ask_t;			// 点击提问
	CButton m_btn_pen_t;			// 点击教鞭
	CButton m_btn_question_t;		// 点击进入问题库
	CButton m_btn_file_trans_t;		// 点击文件传输
	CButton m_btn_setting_t;		// 点击配置
#pragma endregion 教师端


	CListCtrl m_listCtrl;

	CImageList	m_listImage;

private:
	int		m_nBtnWidth;
	int		m_nBtnHeight;


	EClientType	m_appClientType;

public:
	void SetClientType(EClientType e) 
	{
		m_appClientType = e;
	}

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
