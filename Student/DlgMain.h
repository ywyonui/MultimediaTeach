
// StudentDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "UI/DlgLockScreen.h"
#include <string>
#include <vector>
#include "BLL/CoreDefine.h"

// CDlgMain �Ի���
class CDlgMain : public CDialogEx
{
// ����
public:
	CDlgMain(CWnd* pParent = NULL);	// ��׼���캯��
	~CDlgMain();

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

	CString m_strFileName;
	std::vector<ST_MsgFileTransmit> m_vecFileData;

protected:
	/*************************************************************
	��������:	SearchUserList
	����˵��:	vecClient��OUT��: ������������ز�ѯ�����û��б�
	�� �� ֵ:	void
	����˵��:	��ѯ�û��б����ڼ�¼��ʾ
	*************************************************************/
	void AskForClientList();

	// �����б������
	void UpdateList(WPARAM wParam);

	// �����ļ�����
	void TransmitFileData(WPARAM wParam);

	// ��ʾ���ⴰ��
	void ShowQuestion(WPARAM wParam);

	// ������ťλ�õ�ͨ�ú���
	void MoveBtn(CWnd& wnd, int& nX, int& nY, int cx, BOOL bIsLastBtn = FALSE);


private:
	CDlgLockScreen* m_pLockScreen;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// �Զ�����Ϣ����
	afx_msg LRESULT OnServerMsgResult(WPARAM, LPARAM);

public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnHand();
	afx_msg void OnBnClickedBtnSubmit();
	afx_msg void OnBnClickedBtnQuestion();
};
