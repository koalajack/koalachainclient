#pragma once


// CEntWithdrawDlg �Ի���

class CEntWithdrawDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntWithdrawDlg)

public:
	CEntWithdrawDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntWithdrawDlg();

// �Ի�������
	enum { IDD = IDD_ENTWITHDRAW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	string m_strScriptId;
public:
	afx_msg void OnBnClickedOk();
	CString m_strWithdrawAddr;
	double m_fMoney;
	virtual BOOL OnInitDialog();
};
