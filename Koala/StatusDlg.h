#pragma once
#include "StaticTrans.h"
#include "XProgressCtrl.h"
// CStatusDlg 对话框

class CStatusDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg();   // 标准构造函数
	virtual ~CStatusDlg();

// 对话框数据
	enum { IDD = IDD_STATUS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	CStatic		     *m_ProgressWnd;	//Progress窗口
	CStaticTrans     m_strNeting;		//网络同步 
	CStaticTrans     m_strHeight;		//高度 
	CStaticTrans     m_strVersion;      //版本
public:
	HBITMAP		     m_pBmp; 
	CBitmap          m_SignalBmpArray[4];  //信号图片
	CBitmap          m_LockBmpArray[2];  //锁的图片
	int              m_nSigIndex;
	int              m_nLockIndex;
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             LoadGifing( BOOL bState );
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnShowProgressCtrl(  WPARAM wParam, LPARAM lParam ) ;
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	int  ShowProgressCtrl();
	void ShowLockCtrl();
	void OnIniLockParam();
	void ShowNetCount();
	string GetUIVersion();
public:
	BOOL        m_bProgressType;
	int         m_ProgressMax  ;

	CXProgressCtrl m_progress;
	CRect  m_rectSignal;
	BOOL   m_bIsWalletUI;
	BOOL   m_bIsShowProgress;    /// false 出现进度条 true 不显示进度条
	CRect  m_rectLock;
	CString m_strNetName;
	int  m_nConnectCount;
public:
	void SetAppStepfee(int fuelrate);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
