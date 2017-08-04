#pragma once

#include "AddrSelectDlg.h"
#include "afxwin.h"
// CSetAdminDlg 对话框

class CSetAdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAdminDlg)

public:
	CSetAdminDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAdminDlg();

// 对话框数据
	enum { IDD = IDD_SETADMIN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
