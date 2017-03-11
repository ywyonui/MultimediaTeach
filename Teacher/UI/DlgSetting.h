#pragma once
#include "afxwin.h"
#include "afxcmn.h"
/***************************************************************************************/
/**
	文件名称:	DlgLogin.h	
	创建时间:	2016/10/20	22:59:32
	作    者:	ywyonui
	功能说明:	登陆界面
**/
/***************************************************************************************/
#include <vector>


class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetting();

// 对话框数据
	enum { IDD = IDD_DLG_SETTING, IP_ADDRESS_ID = 101 };

private:

public:
	/*************************************************************
	功能说明:	返回登陆成功之后的用户信息的引用
	*************************************************************/

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnSubmit();
	afx_msg void OnEnChangeIPAddressCount();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 自定义消息处理
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);
	afx_msg void OnDestroy();

private:
	void SaveSettingToConfig();


protected:
	CEdit m_edit_count;
	CStatic m_static_count;
	CButton m_btn_submit;

	std::vector<CIPAddressCtrl*> m_vecIpAddress;	// 控件大小150x24

private:
	int m_nIPCount;
	bool m_bInitFlag;

};
