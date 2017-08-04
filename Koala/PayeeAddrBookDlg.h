#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"


// CPayeeAddrBookDlg �Ի���

class CPayeeAddrBookDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPayeeAddrBookDlg)

public:
	CPayeeAddrBookDlg(CWnd* pParent = NULL, CString strTip = _T(""));   // ��׼���캯��
	virtual ~CPayeeAddrBookDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PAY_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString    m_strTip      ;
	map<string,uistruct::ADDRBOOK_t> m_mapAddrInfo;
	uistruct::ADDRBOOK_t m_selectAddr;
	int                hitRow;
	int                hitCol;
	CEdit              editItem;
	CString            m_prehittex;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
	bool LoadAddrBook();
public:
	CListCtrlCl m_listCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAddaddrbook();
	afx_msg void OnBnClickedButtonDeleitem();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
