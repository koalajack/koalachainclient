
// KoalaDlg.h : 头文件
//

#pragma once

#include "BtnST.h"
#include "DrawButton.h"


// CKoalaDlg 对话框
#include "OverviewDlg.h"
#include "SendDlg.h"
#include "ReceiveDlg.h"
#include "TradeRecordDlg.h"
#include "CompanyInfoDlg.h"
#include "EntRigeistDlg.h"
#include "StatusDlg.h"
#include "AddrSelectDlg.h"
#include "BalloonTip.h"
#include "OutGifDlg.h"
#include "CmdHelpDlg.h"
#include "ToolDlg.h"
#include "MsgSigDlg.h"
#include <vector>
using namespace std;

class CKoalaDlg : public CDialogEx
{
// 构造
public:
	CKoalaDlg(CWnd* pParent = NULL);	// 标准构造函数

	~CKoalaDlg();

// 对话框数据
	enum { IDD = IDD_KOALA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnProfile();
	DECLARE_MESSAGE_MAP()
public:
	void  ActiveDlg(UINT nDlgID);
	void  InitialRpcCmd();
	void  ClosWalletWind();
	void  StopSever();
	void  DestroyDlg();
	void  CloseApp();
	void  CloseThread();
	void  ShowCompanyInfoBtn(BOOL bShow = TRUE);

	bool GetFileName(CString &fileName,CString strexe );

	void AddImportWalletAndBookAddr(CString fileName);
	void WriteExportWalletAndBookAddr(CString fileName);
	void WriteLanguageConfig(bool bChinese = true);

	void  SyncAddrInfo();
	void  WriteConfig();
	int JudgeType();
public:
	BOOL m_bEntAcctChecked;
	COverviewDlg* m_pOverviewDlg;
	CSendDlg* m_pSendDlg;
	CReceiveDlg* m_pReceiveDlg;
	CTradeRecordDlg* m_pTradeRecordDlg;
	CCompanyInfoDlg* m_pCompanyInfoDlg;
	//CEntRigeistDlg* m_pEntRegistDlg;
	CAddrSelectDlg* m_pAddrListDlg;

	CCmdHelpDlg* m_pCmdHelpDlg;
	CToolDlg* m_pToolDlg;
	CMsgSigDlg* m_MsgSigDlg;
	vector<CCmdHelpDlg*> m_vecCmdHelp;


	CStatusDlg* m_pStatusDlg;
	COutGifDlg* m_pOutGifDlg;
	CBalloonTip*  m_pBalloonTip; //冒泡提示
public:
	CDrawButton m_btnRece;
	CDrawButton m_btnSend;
	CDrawButton m_btnOverview;
	CDrawButton m_btnRecord;
	CDrawButton m_btnAddrSelect;
	CDrawButton m_btnCompanyInfo;
private:
	CRect m_rcMainDlg;
	CRect m_rcCmpInfo;
	BOOL m_bFirstQuit;
	CMenu m_Menu;
	
public:
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnRece();
	afx_msg void OnBnClickedBtnRecord();
	afx_msg void OnBnClickedBtnCompanyInfo();
	afx_msg void OnBnClickedBtnAddrlist();
	afx_msg void OnMenuOpenuri();
	afx_msg void OnMenuBackup();
//	afx_msg void OnMenuSign();
//	afx_msg void OnMenuVerify();
	afx_msg void OnMenuExport();
	afx_msg void OnMenuImport();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuSetAccount();
	afx_msg void OnMenuChangeAccount();
	afx_msg void OnMenuEncrypt();
	afx_msg void OnMenuChangePassword();
	afx_msg void OnMenuInfo();
	afx_msg void OnMenuPeerlist();
	afx_msg void OnMenuCmd();
	afx_msg void OnMenuCmdHelp();
	afx_msg void OnMenuVersion();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateMenuChangeAccount(CCmdUI *pCmdUI);	
	afx_msg void OnBnClickedBtnSeleteaddr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void         ClosWallet();
	void OnCloseWriteAppFee();
	afx_msg void OnMenuLock();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	HACCEL m_hAccelClose;
	
	afx_msg void OnMenuChinese();
	afx_msg void OnUpdateMenuChinese(CCmdUI *pCmdUI);
	afx_msg void OnMenuEnglish();
	afx_msg void OnUpdateMenuEnglish(CCmdUI *pCmdUI);
};
