#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CEntInfoDlg 对话框

class CEntInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntInfoDlg)

public:
	CEntInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	string m_strEntInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
