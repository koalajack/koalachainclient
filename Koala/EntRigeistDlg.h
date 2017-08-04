#pragma once
#include "afxwin.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"


// CEntRigeistDlg �Ի���

class CEntRigeistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntRigeistDlg)

public:
	CEntRigeistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntRigeistDlg();

// �Ի�������
	enum { IDD = IDD_ENTREGIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbtnOk();
	CEdit m_edtCompanyName;
	CString m_strCompanyName;
public:
	

};

extern bool IsAllDigtal(const char* pData);