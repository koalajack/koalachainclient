#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"

// CEntAddrBookDlg �Ի���

class CEntAddrBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntAddrBookDlg)

public:
	CEntAddrBookDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntAddrBookDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENT_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlCl m_lstEntAddrbook;
	string m_strEntAddr;
	afx_msg void OnBnClickedOk();
};
