#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "afxbutton.h"
#include "afxwin.h"

// CCompanyInfoDlg 对话框

#define NUM_IN_PAGE  14			//一页显示多少行


struct LIST_APP_DATA
{
	string scriptId;
	string entName;
	string entAddr;
	double fQuotaByTime;
	double fQuotaByDay;
	double fBalance;
	double fWithdrawByToday;
	int    nHeight;
	bool bWithdrawByTodayValid;

	LIST_APP_DATA() {fQuotaByTime = 0; fQuotaByDay = 0; fBalance = 0; fWithdrawByToday = 0; nHeight = -1; bWithdrawByTodayValid = false;}
};

struct LIST_DATA
{
	string scriptId;
	string strEntName;
	string strEntAddr;
	string strOutQuotaByTime;
	string strOutQuotaByDay;
	string strBalance;
	string strSumBalance;

	bool operator >(const LIST_DATA& Pinfo) const;

	LIST_DATA(){scriptId = ""; strEntName = ""; strEntAddr = ""; strOutQuotaByTime = ""; strOutQuotaByDay = ""; strBalance = ""; strSumBalance = "";}
};


class CCompanyInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCompanyInfoDlg)

public:
	CCompanyInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompanyInfoDlg();

// 对话框数据
	enum { IDD = IDD_COMPANYINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	void ReadListApp(const Json::Value &root);
	LIST_APP_DATA GetAppValue(const Json::Value &root);
public:
	//查找企业名是否存在
	BOOL FindEnt(string strEntName);
	void InsertEntName(string strEntName);
	BOOL FindEntAddr(string strEntAddr);
	BOOL FindEntDataByAddr(string strEntAddr, LIST_APP_DATA& appData);
	void InsertEntCfgInfo(string strScriptId, string strAddr, double ullByTime, double ullByDay);
	BOOL FindEntAddByScriptId(string strScriptId, string& entAddr);
	void GetAllEntAddr(vector<string>& vecAddr);
	BOOL FindAppDataByScriptId(string strScriptId, LIST_APP_DATA** app_data);
	void ShowWithdrawBtn(BOOL bShow = TRUE);
	void ShowRechargeBtn(BOOL bShow = TRUE);
	void HideAdminBtns(BOOL bHide = TRUE);
	BOOL RefreshListCtrl();
	BOOL GetEntAddr(string strScriptId, string& strEntAddr);
	void GetHeight();
public:
	CListCtrlCl m_lstInfo;
	
	map<string,LIST_APP_DATA> m_listapp;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfcbtnRegister();
	afx_msg void OnBnClickedMfcbtnConfig();
	afx_msg void OnBnClickedMfcbtnRecharge();
	afx_msg void OnBnClickedMfcbtnWithdraw();
	CMFCButton m_btnRegister;
	CMFCButton m_btnConfig;
	CMFCButton m_btnRecharge;
	afx_msg void OnBnClickedMfcbtnInfoUp();
	afx_msg void OnBnClickedMfcbtnInfoNext();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
private:
	int m_nCurrentPage;			//当前页
	int m_nTotalPage;			//总共页数
	int m_nSerialNum;			//序号
	unsigned int m_uSaveNum;	
	
	void ShowListNode(LIST_DATA *node);
	//void AddListNode(LIST_DATA *node);

	void GetEntInfo();
	void GetAllInfo();

public:
	CStatic m_staPage;

	vector<LIST_DATA> m_VecListData;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CMFCButton m_btnWithDraw;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
