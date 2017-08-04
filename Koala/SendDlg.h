#pragma once
#include "afxwin.h"

#include "FlatEdit.h"
#include "BtnST.h"


// CSendDlg �Ի���

class CSendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendDlg();

// �Ի�������
	enum { IDD = IDD_SEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtFrom;
	CString m_strAddrFrom;
	CString m_strAddrTo;
	double m_curAddrMoney;
	CString m_strLabel;
	double m_fMoney;
	afx_msg void OnBnClickedMfcbtnSend();
	afx_msg void OnBnClickedMfcbtnFrom();
	afx_msg void OnBnClickedMfcbtnTo();
	afx_msg LRESULT OnShowListaddrData(  WPARAM wParam, LPARAM lParam ) ;
	CString m_strMessage;
	CEdit m_edtTo;
	CEdit m_edtLabel;
};
