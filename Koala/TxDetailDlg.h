#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CTxDetailDlg 对话框

class CTxDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTxDetailDlg)

public:
	CTxDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTxDetailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWTXDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowTxDetail(CString jsontx);
	string GetContacrDetail(uistruct::REVTRANSACTION_t tx);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClose();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
};
