#pragma once
/***************************************************************************************/
/**
	�ļ�����:	DlgLogin.h	
	����ʱ��:	2016/10/20	22:59:32
	��    ��:	ywyonui
	����˵��:	��½����
**/
/***************************************************************************************/
#include "BLL/define/MsgInfo.h"


class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogin();

// �Ի�������
	enum { IDD = IDD_DLG_LOGIN };

private:

public:
	/*************************************************************
	����˵��:	���ص�½�ɹ�֮����û���Ϣ������
	*************************************************************/

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLogin();
	afx_msg void OnRegUser();
	// �Զ�����Ϣ����
	afx_msg LRESULT OnWndMsgLogin(WPARAM, LPARAM);


};
