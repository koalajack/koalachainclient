#include "StdAfx.h"
#include "DrawButton.h"


CDrawButton::CDrawButton(void)
{
	m_BoundryPen.CreatePen(PS_INSIDEFRAME | PS_SOLID, 0.5, GetSysColor(COLOR_GRAYTEXT));
	
	m_BoundryInPen.CreatePen(PS_INSIDEFRAME, 0, RGB(255,255,255) );
		
	m_bOver = FALSE;
	m_bTracking = FALSE;
	m_bSelected = FALSE;
}


CDrawButton::~CDrawButton(void)
{
}

BEGIN_MESSAGE_MAP(CDrawButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CDrawButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CDrawButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;

	InvalidateRect(NULL);
	return 0;
}
LRESULT CDrawButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return 0;
}

void CDrawButton::SetBitmaps(int nBitmapIn)
{
	m_hBitmap = (HBITMAP)::LoadImage(NULL, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
} 

HBITMAP CDrawButton::SetBitmaps(HBITMAP hBitmap)
{
	HBITMAP hOldBitmap = m_hBitmap;

	m_hBitmap = hBitmap;

	return hOldBitmap;
}

void CDrawButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// TODO:  添加您的代码以绘制指定项
	
	CDC dc;
	dc.Attach(lpDIS->hDC);
	CRect rect(lpDIS->rcItem);
	COLORREF clr = GetSysColor(COLOR_BTNFACE);


	//dc.FillSolidRect(rect, clr);
	dc.FillSolidRect(rect, clr);
	dc.SetBkMode(TRANSPARENT);

	if(m_bOver)
	{
		POINT pt;
		pt.x = 5;
		pt.y = 5;
		CPen *hOldPen = dc.SelectObject(&m_BoundryPen);
		dc.RoundRect(&rect,pt);

		dc.SelectObject(hOldPen);
		//dc.DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);
		

	}

	if((lpDIS->itemState & ODS_SELECTED) & m_bOver)
	{
		m_bSelected = TRUE;
		/*CRect rectIn;
		rectIn.left = rect.left + 1;
		rectIn.top = rect.left + 1;

		rectIn.right = rect.right - 1;
		rectIn.bottom = rect.bottom - 1;

		POINT pt;
		pt.x = 3;
		pt.y = 3;
		CPen *hOldPen = dc.SelectObject(&m_BoundryInPen);
		dc.RoundRect(&rect,pt);*/
		long nWidth = rect.Width();
		long nHeight = rect.Height();
		CRect rtFill;
		for (long i = 1; i < nHeight-1; ++i)
		{
			rtFill.SetRect(1, i, nWidth-1, i+1);//周围留一个像素用来画边框
			dc.FillSolidRect(&rtFill, RGB(GetRValue(clr)-i, GetGValue(clr)-i, GetBValue(clr)-i));
		}
		
		dc.DrawEdge(rect, BDR_SUNKENINNER | BDR_RAISEDOUTER, BF_RECT);
		//rect.OffsetRect(1,1);
		
		//dc.SelectObject(hOldPen);	
	}

	if(m_bSelected)
	{
		long nWidth = rect.Width();
		long nHeight = rect.Height();
		CRect rtFill;
		for (long i = 1; i < nHeight-1; ++i)
		{
			rtFill.SetRect(1, i, nWidth-1, i+1);//周围留一个像素用来画边框
			dc.FillSolidRect(&rtFill, RGB(GetRValue(clr)-i, GetGValue(clr)-i, GetBValue(clr)-i));
		}

		dc.DrawEdge(rect, BDR_SUNKENINNER, BF_RECT);
		//rect.OffsetRect(1,1);
	}

	int n = rect.Height();
	n = (n-14) / 2;
	/*if(lpDIS->itemState &ODS_SELECTED)
	{
		n = n+2;
	}*/
	DrawIconEx(lpDIS->hDC, n,n, m_hIcon, 14, 14, 0, 0, DI_NORMAL);
	CString str;
	GetWindowText(str);
	dc.SetBkMode(TRANSPARENT);

	CRect rc = rect;
	rc.left = n + 24;
	dc.DrawText(str, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	

	dc.Detach();
}


void CDrawButton::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, BS_OWNERDRAW | BS_AUTOCHECKBOX);

	CButton::PreSubclassWindow();
}

void CDrawButton::SetFocusState(BOOL bSelectState)
{
	m_bSelected = bSelectState;

	InvalidateRect(NULL);
}


HICON CDrawButton::SetIcon(HICON hIcon)
{
	HICON hOldIcon = m_hIcon;

	m_hIcon = hIcon;

	return hOldIcon;
}

void CDrawButton::SetIcon(UINT nID)
{
	m_hIcon = AfxGetApp()->LoadIcon(nID);
}

void CDrawButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
}
