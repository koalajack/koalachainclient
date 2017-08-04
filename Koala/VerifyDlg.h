#pragma once
#include "afxcmn.h"
//#include "SelectSendAddr.h"
#include "PayeeAddrBookDlg.h"


// CVerifyDlg 对话框

class CVerifyDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CVerifyDlg)

public:
	CVerifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVerifyDlg();

// 对话框数据
	enum { IDD = IDD_M_CONFIRM_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CRichEditCtrl m_redtConfirm;
	afx_msg void OnBnClickedBtnSelectsendaddr();
	afx_msg void OnBnClickedButtonMConfirm();
	CString m_editSendAddr;
	CString m_richeditMessage;
	CString m_editSignature;
	afx_msg void OnBnClickedButtonDelete();
	CString m_ConfirmTip;
	afx_msg void OnStnClickedStatic33();
};
