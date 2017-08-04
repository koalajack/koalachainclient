#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "explorer1.h"


// CCmdHelpDlg 对话框

class CCmdHelpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCmdHelpDlg)

public:
	CCmdHelpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCmdHelpDlg();

// 对话框数据
	enum { IDD = IDD_CMDHELP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CExplorer1 m_WebView;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
