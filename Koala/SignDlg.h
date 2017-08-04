#pragma once
#include "afxcmn.h"
#include "SelectRecvAddr.h"
#include "afxwin.h"
#include "btnst.h"


// CSignDlg �Ի���

class CSignDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CSignDlg)

public:
	CSignDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSignDlg();

// �Ի�������
	enum { IDD = IDD_M_SIGN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedBtnSelectaddr();
//	CEdit m_editRecvAddr;
	CString m_strRecvAddr;
	CEdit m_editAddr;						//���յ�ַ

	CString m_strSignMessage;
	CRichEditCtrl m_redtSign;				//��Ϣ

	CEdit m_editSignature;					//���ɵ���Ϣǩ��
	CString m_strSignature;
	afx_msg void OnBnClickedButtonMSign();
	
	
	afx_msg void OnBnClickedButtonBoardcopy();
	afx_msg void OnBnClickedButtonBoardpaste();
	afx_msg void OnBnClickedButtonDelete();
	
//	CButtonST m_btnSelectAddr;
	CString m_SignTip;
};
