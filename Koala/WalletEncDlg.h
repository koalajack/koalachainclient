#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"


// CWalletEncDlg �Ի���

class CWalletEncDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CWalletEncDlg)

public:
	CWalletEncDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWalletEncDlg();

// �Ի�������
	enum { IDD = IDD_WALLET_ENC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnEncOk();
//	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnEncCancel();


};
