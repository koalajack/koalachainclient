#pragma once


// CReceiveDlg �Ի���
#include "ListCtrlCl.h"
#include "afxcmn.h"
#include "afxwin.h"

class CReceiveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReceiveDlg)

public:
	CReceiveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReceiveDlg();

// �Ի�������
	enum { IDD = IDD_RECEIVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlCl m_lstHistroy;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbtnPay();
	afx_msg void OnBnClickedMfcbtnRece();
	void InsertListCtrlItem();
	void ShowListInfo();

	CString m_strRecvAddr;
	CString m_strRecvLabel;
	CString m_strMoney;
	CString m_strRecvMsg;

	map<string,uistruct::LISTADDRHISTORY_t> m_MapAddrInfo;

	afx_msg void OnBnClickedMfcbtnReset();
	CEdit m_editSum;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
