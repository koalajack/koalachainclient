#pragma once
#include "NewRecvAddr.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "SignDlg.h"

// CSelectRecvAddr 对话框

class CSelectRecvAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectRecvAddr)

public:
	CSelectRecvAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectRecvAddr();

// 对话框数据
	enum { IDD = IDD_SELECTRECVADDR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNewaddr();
	// 选择收币地址
	CListCtrlCl m_lstRecvAddr;
	virtual BOOL OnInitDialog();
	map<string,uistruct::LISTADDR_t> m_MapAddrInfo;

	void ShowListInfo();

	afx_msg void OnBnClickedBtnSelectaddr();

	CString m_SelectRecvAddr;
};
