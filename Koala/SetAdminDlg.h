#pragma once

#include "AddrSelectDlg.h"
#include "afxwin.h"
// CSetAdminDlg �Ի���

class CSetAdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAdminDlg)

public:
	CSetAdminDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAdminDlg();

// �Ի�������
	enum { IDD = IDD_SETADMIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAdminSelect();

	CEdit m_edtAdmin;
	afx_msg void OnBnClickedOk();
	CString m_strAdmin;
	string m_strShortAddr;
	BOOL m_bShowAdmin;
	virtual BOOL OnInitDialog();
};
