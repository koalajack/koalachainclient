// AddrBookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "AddrBookDlg.h"
#include "afxdialogex.h"
#include "AddrSelectDlg.h"
#include "KoalaDlg.h"


// CAddrBookDlg 对话框

IMPLEMENT_DYNAMIC(CAddrBookDlg, CDialogEx)

CAddrBookDlg::CAddrBookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddrBookDlg::IDD, pParent)
{
	m_bAllShow = FALSE;
}

CAddrBookDlg::~CAddrBookDlg()
{
}

void CAddrBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_ADDRBOOK, m_lstAddrBook);
}


BEGIN_MESSAGE_MAP(CAddrBookDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddrBookDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddrBookDlg 消息处理程序


void CAddrBookDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstAddrBook.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstAddrBook.GetNextSelectedItem(pos) ;
		m_strCurSelectAddr = m_lstAddrBook.GetItemText(nRow, 1);
		uistruct::LISTADDR_t* addr = (uistruct::LISTADDR_t*)m_lstAddrBook.GetItemData(nRow);
		m_strCurSelectShortAddr = addr->RegID;
		m_fCurMoney = addr->fMoney;
	}
	else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请选择一个地址"), _T("提示"), MB_OK);
		else 
			::MessageBoxEx(m_hWnd, _T("Please select a address"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	CDialogEx::OnOK();
}


BOOL CAddrBookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Select enterprise account");
	}

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[3]  = {
		{theApp.m_bChinese ? "标签" : "Label",      150},
		{theApp.m_bChinese ? "地址" : "Address",      250},
		{theApp.m_bChinese ? "余额" : "Balance",      150}
	};
	m_lstAddrBook.SetBkColor(RGB(255,255,255));       
	m_lstAddrBook.SetRowHeigt(23);               
	m_lstAddrBook.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstAddrBook.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstAddrBook.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstAddrBook.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <3 ; i++  ) {
		m_lstAddrBook.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;

	if(!pAddrListDlg)
	{
		return TRUE;
	}

	if(m_bAllShow)
	{
		pAddrListDlg->GetAllAddr(vecAddr);
	}
	else
	{
		pAddrListDlg->GetActiveAddr(vecAddr);
	}

	int count = vecAddr.size();
	for(int i = 0; i < count; i++)
	{
		m_lstAddrBook.InsertItem(i, vecAddr[i].Label.c_str());
		m_lstAddrBook.SetItemText(i, 1, vecAddr[i].address.c_str());
		CString strMoney;
		strMoney.Format("%0.2f", vecAddr[i].fMoney);
		m_lstAddrBook.SetItemText(i, 2, strMoney);
		m_lstAddrBook.SetItemData(i, (DWORD_PTR)&vecAddr[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
