#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"

// CAddrBookDlg 对话框

class CAddrBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrBookDlg)

public:
	CAddrBookDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddrBookDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListCtrlCl m_lstAddrBook;
	virtual BOOL OnInitDialog();
public:
	string m_strCurSelectAddr;
	double m_fCurMoney;
	string m_strCurSelectShortAddr;
	BOOL m_bAllShow;
private:
	vector<uistruct::LISTADDR_t> vecAddr;
};
