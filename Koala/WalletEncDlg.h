#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"


// CWalletEncDlg 对话框

class CWalletEncDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CWalletEncDlg)

public:
	CWalletEncDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWalletEncDlg();

// 对话框数据
	enum { IDD = IDD_WALLET_ENC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnEncOk();
//	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnEncCancel();


};
