#pragma once
/***************************************************************************************/
/**
	�ļ�����:	DlgReg.h	
	����ʱ��:	2016/10/23	00:41:58
	��    ��:	ywyonui
	����˵��:	ע�ᴰ��
**/
/***************************************************************************************/

// CDlgReg �Ի���

class CDlgReg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReg)

public:
	CDlgReg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgReg();

// �Ի�������
	enum { IDD = IDD_DLG_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnRegUser();
	afx_msg void OnRegCancel();
};
