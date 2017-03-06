#pragma once
/***************************************************************************************/
/**
	文件名称:	DlgReg.h	
	创建时间:	2016/10/23	00:41:58
	作    者:	ywyonui
	功能说明:	注册窗口
**/
/***************************************************************************************/

// CDlgReg 对话框

class CDlgReg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReg)

public:
	CDlgReg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgReg();

// 对话框数据
	enum { IDD = IDD_DLG_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	afx_msg void OnRegUser();
	afx_msg void OnRegCancel();
};
