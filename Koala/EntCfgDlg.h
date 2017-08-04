#pragma once
#include "afxwin.h"


// CEntCfgDlg �Ի���

class CEntCfgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntCfgDlg)

public:
	CEntCfgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntCfgDlg();

// �Ի�������
	enum { IDD = IDD_ENTCFG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
