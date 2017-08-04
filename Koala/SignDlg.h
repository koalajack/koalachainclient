#pragma once
#include "afxcmn.h"
#include "SelectRecvAddr.h"
#include "afxwin.h"
#include "btnst.h"


// CSignDlg 对话框

class CSignDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CSignDlg)

public:
	CSignDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignDlg();

// 对话框数据
	enum { IDD = IDD_M_SIGN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedBtnSelectaddr();
//	CEdit m_editRecvAddr;
	CString m_strRecvAddr;
	CEdit m_editAddr;						//接收地址

	CString m_strSignMessage;
	CRichEditCtrl m_redtSign;				//消息

	CEdit m_editSignature;					//生成的消息签名
	CString m_strSignature;
	afx_msg void OnBnClickedButtonMSign();
	
	
	afx_msg void OnBnClickedButtonBoardcopy();
	afx_msg void OnBnClickedButtonBoardpaste();
	afx_msg void OnBnClickedButtonDelete();
	
//	CButtonST m_btnSelectAddr;
	CString m_SignTip;
};
