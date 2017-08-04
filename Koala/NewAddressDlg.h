#pragma once

#include "StaticTrans.h"
#include "ShadeButtonST.h"

// CNewAddressDlg 对话框

class CNewAddressDlg: public CDialogEx
{
	DECLARE_DYNAMIC(CNewAddressDlg)

public:
	CNewAddressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewAddressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENERATE_NEWADDRES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton          m_rBtnNewAdd;
	CStaticTrans     m_label   ;  //

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonScdz();
	CString m_strNum;
};
