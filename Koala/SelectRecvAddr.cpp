// SelectRecvAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "SelectRecvAddr.h"
#include "afxdialogex.h"


// CSelectRecvAddr 对话框

IMPLEMENT_DYNAMIC(CSelectRecvAddr, CDialogEx)

CSelectRecvAddr::CSelectRecvAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectRecvAddr::IDD, pParent)
{
	
}

CSelectRecvAddr::~CSelectRecvAddr()
{
}

void CSelectRecvAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECVADDR, m_lstRecvAddr);
}


BEGIN_MESSAGE_MAP(CSelectRecvAddr, CDialogEx)
	
	ON_BN_CLICKED(IDC_BTN_SELECTADDR, &CSelectRecvAddr::OnBnClickedBtnSelectaddr)
END_MESSAGE_MAP()


// CSelectRecvAddr 消息处理程序




BOOL CSelectRecvAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC222)->SetWindowText("These are your wallet addresses that can be used for collection. It is recommended that you use a new address for each transaction.");
		GetDlgItem(IDC_BTN_SELECTADDR)->SetWindowText("Select");
		SetWindowText("Select receive address");
	}

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[4]  = {
		{theApp.m_bChinese ? "标签" : "Label",      200},
		{theApp.m_bChinese ? "地址" : "Address",      250},
	};
	m_lstRecvAddr.SetBkColor(RGB(255,255,255));       
	m_lstRecvAddr.SetRowHeigt(23);               
	m_lstRecvAddr.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstRecvAddr.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstRecvAddr.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstRecvAddr.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <2 ; i++  ) {
		m_lstRecvAddr.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	ShowListInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelectRecvAddr::ShowListInfo()
{
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 order by reg_id"), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_lstRecvAddr.DeleteAllItems();

	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;

		uistruct::LISTADDR_t address = const_it->second;

		m_lstRecvAddr.InsertItem( i , address.Label.c_str()) ;

		m_lstRecvAddr.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;

		if (address.bSign == 1)
		{
			strShowData=theApp.m_bChinese ? _T("已激活") : _T("Activated") ;
		}else{
			strShowData=theApp.m_bChinese ? _T("未激活") : _T("Inactivated");
		}

		m_lstRecvAddr.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		strShowData = strprintf("%.2f" , address.fMoney ) ;
		m_lstRecvAddr.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		i++;
	}

}




void CSelectRecvAddr::OnBnClickedBtnSelectaddr()
{
	// TODO: 在此添加控件通知处理程序代码
	string StrShow;
	POSITION pos = m_lstRecvAddr.GetFirstSelectedItemPosition() ;
	if ( pos )
	{
		int nRow = m_lstRecvAddr.GetNextSelectedItem(pos) ;
		//uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		m_SelectRecvAddr = m_lstRecvAddr.GetItemText(nRow, 1);
		//文本内容保存在source变量中
			//GetDlgItem(, IDD_M_SIGN_DLG);
		/*GetParent()->GetParent()->SetDlgItemText(IDC_EDIT_SIGNADDR, source);

		CString str;
		GetParent()->GetParent()->GetDlgItemText(IDC_EDIT_SIGNADDR, str);*/
		
		
	}
	CDialogEx::OnOK();
	
}
