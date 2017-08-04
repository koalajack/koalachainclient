#pragma once

#include "StaticTrans.h"
#include "ShadeButtonST.h"
#include "CJFlatComboBox.h"
#include "afxcmn.h"

// CSignAccountsDlg �Ի���

class CSignAccountsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSignAccountsDlg)

public:
	CSignAccountsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSignAccountsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SIGN_ACCOUNTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFont            m_fontGrid;//�������
public:
	CButton   m_rBtnSend;
	CStaticTrans     m_addr   ;  //
	CStaticTrans     m_fee   ;  //
	CCJFlatComboBox  m_cboUnit;
public:
	afx_msg void OnBnClickedButtonSend();
	void SetShowAddr(vector<AMADDR> &refaddr);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
	CListCtrl m_lstAddrAct;
	afx_msg void OnItemchangedListAddract(NMHDR *pNMHDR, LRESULT *pResult);

	map<string,uistruct::LISTADDR_t> m_MapAddrInfo;
};
