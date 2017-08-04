#pragma once


// CMsgSigDlg 对话框
#include "SignDlg.h"
#include "VerifyDlg.h"
#include "CoolTabCtrl.h"

class CMsgSigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgSigDlg)

public:
	CMsgSigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgSigDlg();

// 对话框数据
	enum { IDD = IDD_MESSAGE_SIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CSignDlg m_dlgSign;
	CVerifyDlg m_dlgVerify;
	//CMFCTabCtrl m_wndTab;
	CCoolTabCtrl m_wndTab;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
