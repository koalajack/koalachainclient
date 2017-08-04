#pragma once
#include "afxwin.h"

#include "CJFlatComboBox.h"
#include "afxcmn.h"
#include "ListCtrlCl.h"


// CTradeRecordDlg �Ի���

class CTradeRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTradeRecordDlg)

public:
	CTradeRecordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTradeRecordDlg();

// �Ի�������
	enum { IDD = IDD_TRADE_RECODE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCJFlatComboBox m_cboAll;
	CCJFlatComboBox m_cboSend;
	CListCtrlCl m_lstRecord;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbtnTradeUp();
	afx_msg void OnBnClickedMfcbtnTradeNex();
	afx_msg void OnCbnSelchangeComboboxAll();
	afx_msg void OnCbnSelchangeComboboxSend();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
private:
	int                    m_pagesize;
	int                    m_offset;
	int                    m_nConut   ;     //��ҳ��
	uistruct::TRANSRECORDLIST m_pListInfo ;
private:
	void  OninitializeList();
	void  ShowComboxCotent();
	string GetConditonTxType(int &operate);
	string GetConditonTime();
	string GetConditonStr(int &operate);
	void  ShowPageCotent(int pageIndex);
	void OnShowListCtrl(uistruct::TRANSRECORDLIST pListInfo);
	void ShowPageDataInfo(int nConut);
public:
	CStatic m_staPage;
	afx_msg void OnNMDblclkListTraderecord(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
