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

//按钮状态
protected:
	BOOL m_bOver;						//鼠标位于按钮之上时该值为true，反之为flase
	BOOL m_bFocus;						//按钮为当前焦点所在时该值为true
	BOOL m_bTracking;					//鼠标按下没有松开时值为true
	BOOL m_bSelected;					//鼠标按钮时为true
	CPen m_BoundryPen;					//按钮的外边框

	CPen m_BoundryInPen;                 //鼠标位于按钮上时的内边框
	CPen m_BoundryInSel;				//鼠标获取焦点时的内边框
		

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

