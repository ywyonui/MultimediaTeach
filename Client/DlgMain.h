
// StudentDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BLL/define/EnumType.h"

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
#pragma region ѧ����
	CButton m_btn_hand_s;		// �������
	CButton m_btn_submit_s;		// ����ύ��ҵ
	CButton m_btn_question_s;	// ������������
#pragma endregion ѧ����

#pragma region ��ʦ��
	CButton m_btn_lock_t;			// �������
	CButton m_btn_show_demo_t;		// �����ʾ
	CButton m_btn_roll_call_t;		// �������
	CButton m_btn_ask_t;			// �������
	CButton m_btn_pen_t;			// ����̱�
	CButton m_btn_question_t;		// ������������
	CButton m_btn_file_trans_t;		// ����ļ�����
	CButton m_btn_setting_t;		// �������
#pragma endregion ��ʦ��


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
	// �����б�����ݣ���ʼ��������⣬�����������
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
