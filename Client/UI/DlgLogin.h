#pragma once
/***************************************************************************************/
/**
	文件名称:	DlgLogin.h	
	创建时间:	2016/10/20	22:59:32
	作    者:	ywyonui
	功能说明:	登陆界面
**/
/***************************************************************************************/
#include "BLL/define/MsgInfo.h"


class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLogin();

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

private:

public:
	/*************************************************************
	功能说明:	返回登陆成功之后的用户信息的引用
	*************************************************************/

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLogin();
	afx_msg void OnRegUser();
	// 自定义消息处理
	afx_msg LRESULT OnWndMsgLogin(WPARAM, LPARAM);


};
