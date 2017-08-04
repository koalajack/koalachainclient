// EntAddrBookDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "EntAddrBookDlg.h"
#include "afxdialogex.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"

// CEntAddrBookDlg 对话框

IMPLEMENT_DYNAMIC(CEntAddrBookDlg, CDialogEx)

CEntAddrBookDlg::CEntAddrBookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntAddrBookDlg::IDD, pParent)
{

}

CEntAddrBookDlg::~CEntAddrBookDlg()
{
}

void CEntAddrBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_ENT_ADDRBOOK, m_lstEntAddrbook);
}


BEGIN_MESSAGE_MAP(CEntAddrBookDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntAddrBookDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEntAddrBookDlg 消息处理程序


BOOL CEntAddrBookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return FALSE;
	}

	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[2]  = {
		{theApp.m_bChinese ? "序号" : "NO" ,      100},
		{theApp.m_bChinese ? "地址" : "Address",      350}
	};
	m_lstEntAddrbook.SetBkColor(RGB(255,255,255));       
	m_lstEntAddrbook.SetRowHeigt(23);               
	m_lstEntAddrbook.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstEntAddrbook.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstEntAddrbook.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstEntAddrbook.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i < 2 ; i++  ) {
		m_lstEntAddrbook.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	vector<string> vecAddr;
	pCompanyInfoDlg->GetAllEntAddr(vecAddr);

	for(int i = 0; i < (int)vecAddr.size(); i++)
	{
		CString strIndex;
		strIndex.Format("%d", i + 1);
		m_lstEntAddrbook.InsertItem(i, strIndex);
		m_lstEntAddrbook.SetItemText(i, 1, vecAddr[i].c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEntAddrBookDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstEntAddrbook.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstEntAddrbook.GetNextSelectedItem(pos) ;
		m_strEntAddr = m_lstEntAddrbook.GetItemText(nRow, 1);

	}
	else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请选择一个地址"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please select an address"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	CDialogEx::OnOK();
}
