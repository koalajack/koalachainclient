#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CWalletPassPhrase 对话框

class CWalletPassPhrase : public CDialogEx
{
	DECLARE_DYNAMIC(CWalletPassPhrase)

public:
	CWalletPassPhrase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWalletPassPhrase();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETWALLETENTRP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
public:
	CStaticTrans     m_password;
	CStaticTrans     m_timeout;
	CStaticTrans     m_minue;
	
};
