#pragma once
#include "DialogBase.h"

// CModifyPasswordDlg �Ի���

class CModifyPasswordDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CModifyPasswordDlg)

public:
	CModifyPasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModifyPasswordDlg();

// �Ի�������
	enum { IDD = IDD_MODIFY_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedCancel();
};
