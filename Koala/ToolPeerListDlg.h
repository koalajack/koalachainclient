#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"

// CToolPeerListDlg 对话框

struct LIST_PEER_DATA
{
	string addr;
	string addrlocal;
	ULONGLONG banscore;
	ULONGLONG btyesrecv;
	ULONGLONG bytessend;
	ULONGLONG conntime;
	bool inbound;
	ULONGLONG lastrecv;
	ULONGLONG lastsend;
	ULONGLONG pingtime;
	string services;
	ULONGLONG startinghigh;
	string subver;
	bool syncnode;
	string version;

	LIST_PEER_DATA() {addr = ""; addrlocal = ""; banscore = 0; btyesrecv = 0; bytessend = 0; conntime = 0; inbound = false;
						lastrecv = 0; lastsend = 0; pingtime = 0; services = ""; startinghigh = 0; subver = ""; syncnode = false; version = "";};
};


class CToolPeerListDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CToolPeerListDlg)

public:
	CToolPeerListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolPeerListDlg();

	void ShowJson(string rpcname,string jsomstr);

// 对话框数据
	enum { IDD = IDD_TOOLPEERLIST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlCl m_lstPeerList;
	void GetPeerlist(map<string,LIST_PEER_DATA>& PeerListInfo);
	void SetInfo(LIST_PEER_DATA& data);
	int FindAddr(string strAddr);
	void InsertListData(int nRow, LIST_PEER_DATA& data);
	void ModifyListData(int nRow, LIST_PEER_DATA& data);
	string Second2HMS(ULONGLONG ullSecond);
	map<string,LIST_PEER_DATA> m_PeerListInfo;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnItemchangedListPeerlist(NMHDR *pNMHDR, LRESULT *pResult);

private:
	unsigned int m_nIndex;
public:
	afx_msg void OnNMClickListPeerlist(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
