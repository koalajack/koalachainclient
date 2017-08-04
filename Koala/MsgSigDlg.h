#pragma once


// CMsgSigDlg �Ի���
#include "SignDlg.h"
#include "VerifyDlg.h"
#include "CoolTabCtrl.h"

class CMsgSigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgSigDlg)

public:
	CMsgSigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgSigDlg();

// �Ի�������
	enum { IDD = IDD_MESSAGE_SIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
