#pragma once
#include "afxwin.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"


// CEntRigeistDlg 对话框

class CEntRigeistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntRigeistDlg)

public:
	CEntRigeistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEntRigeistDlg();

// 对话框数据
	enum { IDD = IDD_ENTREGIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbtnOk();
	CEdit m_edtCompanyName;
	CString m_strCompanyName;
public:
	

};

extern bool IsAllDigtal(const char* pData);