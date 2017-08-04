#pragma once
#include "StaticTrans.h"
#include "XProgressCtrl.h"
// CStatusDlg �Ի���

class CStatusDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg();   // ��׼���캯��
	virtual ~CStatusDlg();

// �Ի�������
	enum { IDD = IDD_STATUS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	CStatic		     *m_ProgressWnd;	//Progress����
	CStaticTrans     m_strNeting;		//����ͬ�� 
	CStaticTrans     m_strHeight;		//�߶� 
	CStaticTrans     m_strVersion;      //�汾
public:
	HBITMAP		     m_pBmp; 
	CBitmap          m_SignalBmpArray[4];  //�ź�ͼƬ
	CBitmap          m_LockBmpArray[2];  //����ͼƬ
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
	BOOL   m_bIsShowProgress;    /// false ���ֽ����� true ����ʾ������
	CRect  m_rectLock;
	CString m_strNetName;
	int  m_nConnectCount;
public:
	void SetAppStepfee(int fuelrate);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
