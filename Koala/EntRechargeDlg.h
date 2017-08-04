#pragma once


// CEntRechargeDlg 对话框

class CEntRechargeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntRechargeDlg)

public:
	CEntRechargeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntRechargeDlg();

// 对话框数据
	enum { IDD = IDD_ENTRECHARGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	string m_strScriptId;
	CString m_strEntName;
	
	afx_msg void OnBnClickedMfcbtnRecFrom();
	virtual BOOL OnInitDialog();
	
	double m_fRecharge;
	CString m_strRechargeSrc;
	CString m_strInputScriptId;
	CString m_strVerifyEntName;
};
