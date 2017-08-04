#pragma once
#include "afxwin.h"


// CEntCfgDlg 对话框

class CEntCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntCfgDlg)

public:
	CEntCfgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntCfgDlg();

// 对话框数据
	enum { IDD = IDD_ENTCFG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	string m_strScriptId;
	CString m_strAccountAddr;
	double m_fQuotaByTime;
	double m_fQuotaByDay;
	BOOL m_bHadConfig;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_edtAddr;
private:
	BOOL IsActived(string strAddr);
};
