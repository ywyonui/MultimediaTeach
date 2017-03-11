#pragma once
#include "afxwin.h"
#include "afxcmn.h"
/***************************************************************************************/
/**
	�ļ�����:	DlgLogin.h	
	����ʱ��:	2016/10/20	22:59:32
	��    ��:	ywyonui
	����˵��:	��½����
**/
/***************************************************************************************/
#include <vector>


class CDlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetting();

// �Ի�������
	enum { IDD = IDD_DLG_SETTING, IP_ADDRESS_ID = 101 };

private:

public:
	/*************************************************************
	����˵��:	���ص�½�ɹ�֮����û���Ϣ������
	*************************************************************/

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnSubmit();
	afx_msg void OnEnChangeIPAddressCount();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// �Զ�����Ϣ����
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);
	afx_msg void OnDestroy();

private:
	void SaveSettingToConfig();


protected:
	CEdit m_edit_count;
	CStatic m_static_count;
	CButton m_btn_submit;

	std::vector<CIPAddressCtrl*> m_vecIpAddress;	// �ؼ���С150x24

private:
	int m_nIPCount;
	bool m_bInitFlag;

};
