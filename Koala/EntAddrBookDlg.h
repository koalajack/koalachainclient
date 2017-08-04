#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"

// CEntAddrBookDlg 对话框

class CEntAddrBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntAddrBookDlg)

public:
	CEntAddrBookDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntAddrBookDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENT_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlCl m_lstEntAddrbook;
	string m_strEntAddr;
	afx_msg void OnBnClickedOk();
};
