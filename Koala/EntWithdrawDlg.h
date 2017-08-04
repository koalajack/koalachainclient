#pragma once


// CEntWithdrawDlg 对话框

class CEntWithdrawDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntWithdrawDlg)

public:
	CEntWithdrawDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntWithdrawDlg();

// 对话框数据
	enum { IDD = IDD_ENTWITHDRAW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	string m_strScriptId;
public:
	afx_msg void OnBnClickedOk();
	CString m_strWithdrawAddr;
	double m_fMoney;
	virtual BOOL OnInitDialog();
};
