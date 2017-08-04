#pragma once

#include "afxcmn.h"
#include "ListCtrlCl.h"

#include <map>
#include "afxwin.h"
using namespace std;

// CAddrSelectDlg 对话框

class CAddrSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrSelectDlg)

public:
	CAddrSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddrSelectDlg();

// 对话框数据
	enum { IDD = IDD_ADDR_SELECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAddrselectNew();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
private:
	void ShowListInfo();
	void ShowListFilterInfo();
	void ModifyListCtrlItem();
	void InsertListCtrlItem();
public:
	void GetActiveAddr(vector<uistruct::LISTADDR_t>& vecAddr);
	void GetAllAddr(vector<uistruct::LISTADDR_t>& vecAddr);
	BOOL AddrIsExsit(string addr);
public:
	CListCtrlCl m_lstAddrSelect;
	map<string,uistruct::LISTADDR_t> m_MapAddrInfo;
	map<string,uistruct::LISTADDR_t> m_MapFilterAddrInfo;
	afx_msg void OnBnClickedAddrselectActivate();
	afx_msg void OnBnClickedAddrselectCopy();
	afx_msg void OnBnClickedAddrselectExport();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnBnClickedAddrselectSearch();
	//afx_msg void OnBnClickedMfcbtnFilter();
	afx_msg void OnDblclkListAddrselect(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnEndlabeleditListAddrselect(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CEdit m_Edit;
	int m_Item;
	int m_SubItem;

	BOOL m_EditCreated;			//标记编辑框是否被创建;


	int m_RowCount;
	BOOL m_NeedSave;

	afx_msg void OnEndlabeleditListAddrselect(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnClickListAddrselect(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMfcbtnFilter();
	afx_msg void OnBnClickedMfcbtnClear();
	afx_msg void OnBnClickedMfcbtnCollection();
};
