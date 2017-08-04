#pragma once
#include "DialogBase.h"

// CModifyPasswordDlg 对话框

class CModifyPasswordDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CModifyPasswordDlg)

public:
	CModifyPasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModifyPasswordDlg();

// 对话框数据
	enum { IDD = IDD_MODIFY_PASSWORD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedCancel();
};
