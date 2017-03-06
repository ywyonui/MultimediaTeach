
// StudentDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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
	CButton m_btn_hand;		// �������
	CButton m_btn_submit;	// ����ύ��ҵ
	CButton m_btn_question;	// ������������
	CListCtrl m_listCtrl;

	CImageList	m_listImage;

private:
	int		m_nBtnWidth;
	int		m_nBtnHeight;

protected:
	// �����б������ݣ���ʼ��������⣬�����������
	void UpdateList(BOOL bInitFlag);

	// ������ťλ�õ�ͨ�ú���
	void MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE);



// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
};