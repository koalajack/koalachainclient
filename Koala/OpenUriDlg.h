#pragma once
#include "afxwin.h"


// COpenUriDlg 对话框

class COpenUriDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenUriDlg)

public:
	COpenUriDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenUriDlg();

// 对话框数据
	enum { IDD = IDD_OPENURI_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOpenuri();
	BOOL ParseURI(string strURI, string *pStrAddr, string *pStrLabel, string *pStrMsg, string *pAmount);	
	BOOL GetFileName(CString &fileName,CString strexe );
	CString m_strOpenURI;
	afx_msg void OnBnClickedBtnOk();

	CEdit m_ctlOpenURI;
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

protected:
	CBrush m_redbrush;  
	COLORREF m_redcolor;  
	COLORREF m_textcolor; // edit控件的字体颜色



public:
	string m_strAddr;
	string m_strLabel;
	string m_strMsg;
	string m_account;
	afx_msg void OnBnClickedBtnCancel();
};
