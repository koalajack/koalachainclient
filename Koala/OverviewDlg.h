#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "afxwin.h"

// COverviewDlg 对话框

class COverviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COverviewDlg)

public:
	COverviewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COverviewDlg();

// 对话框数据
	enum { IDD = IDD_OVERVIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	LRESULT OnShowListCtrl(WPARAM wParam, LPARAM lParam);  //处理更新首页空间控件数据的消息
public:
	virtual BOOL OnInitDialog();
	void SetEntInfo(string strEntAddr);
	void SetEntInfoEx();
	CListCtrlCl m_lstReceTrade;
private:
	void SetCtrlText();                  //重新设置各控件的数据
	void OnInitialCtrlText();            //初始化所有控件数据
public:
	CStaticTrans m_staUnconfirmMoney;
	CStaticTrans m_staUsageMoney;
	CStaticTrans m_staQuotaByTime;
	CStaticTrans m_staQuotaByDay;
	CStaticTrans m_staQuotaRemain;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
