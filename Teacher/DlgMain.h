
// StudentDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include "BLL/CoreDefine.h"

// CDlgMain �Ի���
class CDlgMain : public CDialogEx
{
// ����
public:
	CDlgMain(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


protected: // �ؼ�
	CButton m_btn_lock;			// �������
	CButton m_btn_display;		// �����ʾ
	CButton m_btn_roll_call;	// �������
	CButton m_btn_ask;			// �������
	CButton m_btn_pen;			// ����̱�
	CButton m_btn_question;		// ������������
	CButton m_btn_file_trans;	// ����ļ�����
	CButton m_btn_setting;		// �������
	CListCtrl m_listCtrl;

	CImageList	m_listImage;


private:
	int		m_nBtnWidth;
	int		m_nBtnHeight;


protected:

	/*************************************************************
	��������:	SearchUserList
	����˵��:	vecClient��OUT��: ������������ز�ѯ�����û��б�
	�� �� ֵ:	void
	����˵��:	��ѯ�û��б����ڼ�¼��ʾ
	*************************************************************/
	void AskForClientList();

	// �����б�����ݣ���ʼ��������⣬�����������
	void UpdateList(WPARAM wParam);

	// ������ťλ�õ�ͨ�ú���
	void MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE);


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// �Զ�����Ϣ����
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
