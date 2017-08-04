#pragma once
#include "afxwin.h"
#include "BtnST.h"
class CDrawButton :
	public CButtonST
{
public:
	CDrawButton(void);
	~CDrawButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void PreSubclassWindow();
private:
	HICON m_hIcon;
	HBITMAP m_hBitmap;

//��ť״̬
protected:
	BOOL m_bOver;						//���λ�ڰ�ť֮��ʱ��ֵΪtrue����֮Ϊflase
	BOOL m_bFocus;						//��ťΪ��ǰ��������ʱ��ֵΪtrue
	BOOL m_bTracking;					//��갴��û���ɿ�ʱֵΪtrue
	BOOL m_bSelected;					//��갴ťʱΪtrue
	CPen m_BoundryPen;					//��ť����߿�

	CPen m_BoundryInPen;                 //���λ�ڰ�ť��ʱ���ڱ߿�
	CPen m_BoundryInSel;				//����ȡ����ʱ���ڱ߿�
		

public:
	HICON SetIcon(HICON hIcon);
	void SetIcon(UINT nID);
	void SetBitmaps(int nBitmapIn);
	HBITMAP SetBitmaps(HBITMAP hBitmap);
	void DrawInsideBorder(CDC *pDC, CRect* rect);

	void SetFocusState(BOOL bSelectState);
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

