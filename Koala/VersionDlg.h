#pragma once
#include "afxcmn.h"


// CVersionDlg �Ի���

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CVersionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVersionDlg();

// �Ի�������
	enum { IDD = IDD_VERSION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	string GetUIVersion();
	CRichEditCtrl m_richeditVer;
	afx_msg void OnBnClickedOk();
};
