#pragma once
#include "NewRecvAddr.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "SignDlg.h"

// CSelectRecvAddr �Ի���

class CSelectRecvAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectRecvAddr)

public:
	CSelectRecvAddr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectRecvAddr();

// �Ի�������
	enum { IDD = IDD_SELECTRECVADDR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNewaddr();
	// ѡ���ձҵ�ַ
	CListCtrlCl m_lstRecvAddr;
	virtual BOOL OnInitDialog();
	map<string,uistruct::LISTADDR_t> m_MapAddrInfo;

	void ShowListInfo();

	afx_msg void OnBnClickedBtnSelectaddr();

	CString m_SelectRecvAddr;
};
