// ReceiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ReceiveDlg.h"
#include "afxdialogex.h"
#include "RPDlg.h"
#include "AddrBookDlg.h"
#include "SqliteDeal.h"


// CReceiveDlg 对话框

IMPLEMENT_DYNAMIC(CReceiveDlg, CDialogEx)

CReceiveDlg::CReceiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReceiveDlg::IDD, pParent)
	, m_strRecvAddr(_T(""))
	, m_strRecvLabel(_T(""))
	, m_strMoney(_T(""))
	, m_strRecvMsg(_T(""))
{

}

CReceiveDlg::~CReceiveDlg()
{
}

void CReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAYRECORD, m_lstHistroy);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRecvAddr);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_strRecvLabel);
	DDX_Text(pDX, IDC_EDIT_SUM, m_strMoney);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strRecvMsg);
	DDX_Control(pDX, IDC_EDIT_SUM, m_editSum);
}

BEGIN_MESSAGE_MAP(CReceiveDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBTN_PAY, &CReceiveDlg::OnBnClickedMfcbtnPay)
	ON_BN_CLICKED(IDC_MFCBTN_RECE, &CReceiveDlg::OnBnClickedMfcbtnRece)
	ON_BN_CLICKED(IDC_MFCBTN_RESET, &CReceiveDlg::OnBnClickedMfcbtnReset)
END_MESSAGE_MAP()


// CReceiveDlg 消息处理程序


BOOL CReceiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		/*
		GetDlgItem(IDC_STATIC111)->SetWindowText("Receive:");
		GetDlgItem(IDC_STATIC112)->SetWindowText("Label:");
		GetDlgItem(IDC_STATIC113)->SetWindowText("Amount:");
		GetDlgItem(IDC_STATIC114)->SetWindowText("Message:");
		GetDlgItem(IDC_STATIC115)->SetWindowText("Request a payment history:");
		GetDlgItem(IDC_MFCBTN_PAY)->SetWindowText("Pay");
		GetDlgItem(IDC_MFCBTN_RESET)->SetWindowText("Rest");
		*/
	}

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[4]  = {
		{theApp.m_bChinese ? "日期" : "Date" ,      150},
		{theApp.m_bChinese ? "标签" : "Label",      150},
		{theApp.m_bChinese ? "消息" : "Message",      250}, 
		{theApp.m_bChinese ? "金额" : "Amount" ,      150}
	};
	m_lstHistroy.SetBkColor(RGB(255,255,255));       
	m_lstHistroy.SetRowHeigt(23);               
	m_lstHistroy.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstHistroy.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstHistroy.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstHistroy.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <4 ; i++  ) {
		m_lstHistroy.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	ShowListInfo();

	//theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_RECIVE_UI ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void long2time1(unsigned long nTime, CString& strTime)
{ 
	CTime objTime(nTime);    
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),
		objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond());
}

void CReceiveDlg::ShowListInfo()
{
	theApp.m_SqliteDeal.GetRPHistoryList(_T(""), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_lstHistroy.DeleteAllItems();

	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDRHISTORY_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		
		uistruct::LISTADDRHISTORY_t data = const_it->second;

		unsigned long time = data.datetime;
		CString strTime;
		long2time1(time, strTime);
		m_lstHistroy.InsertItem(i, strTime);
		m_lstHistroy.SetItemText(i, nSubIdx++, strTime);				
		m_lstHistroy.SetItemText(i , nSubIdx++ , data.label.c_str() ) ;
		m_lstHistroy.SetItemText(i , nSubIdx++ , data.message.c_str() ) ;
		strShowData = strprintf("%.2f" , data.fMoney ) ;
		m_lstHistroy.SetItemText(i, nSubIdx++, strShowData.c_str());
		
		i++;
	}

}


void CReceiveDlg::OnBnClickedMfcbtnPay()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	/*if (!IsAllDigtal(m_strMoney))
	{
	MessageBox(_T("金额栏含有非法输入"), _T("提示"));
	return;
	}*/
	CRPDlg dlg;
	
	dlg.m_strRecvAddr = m_strRecvAddr;
	dlg.m_strRecvLabel = m_strRecvLabel;
	dlg.m_strMoney = m_strMoney;
	dlg.m_strRecvMessage =  m_strRecvMsg;

	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	string strTime;
	strTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);

	//添加到数据库
	if (theApp.IsLockWallet())
	{
		return;
	}
	string strShowData ;
	string strSourceData;
	strSourceData =strprintf("'%d' , '%s' , '%s' , '%s' ,'%s'" , maxcurtime ,m_strRecvAddr, m_strRecvLabel, m_strRecvMsg ,m_strMoney) ;

	theApp.m_SqliteDeal.InsertTableItem(_T("t_payment_request"), strSourceData);

	int count = m_lstHistroy.GetItemCount();
	int nSubIdx = 0,i = count;

	m_lstHistroy.InsertItem(i, strTime.c_str());
	m_lstHistroy.SetItemText(i, nSubIdx++, strTime.c_str());				
	m_lstHistroy.SetItemText(i , nSubIdx++ , m_strRecvLabel ) ;
	m_lstHistroy.SetItemText(i , nSubIdx++ , m_strRecvMsg ) ;
	m_lstHistroy.SetItemText(i, nSubIdx++, m_strMoney);

	dlg.DoModal();

}

void CReceiveDlg::InsertListCtrlItem()
{

}


void CReceiveDlg::OnBnClickedMfcbtnRece()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddrBookDlg dlg;
	dlg.m_bAllShow = TRUE;
	if(IDOK == dlg.DoModal())
	{
		m_strRecvAddr.Format("%s", dlg.m_strCurSelectAddr.c_str());
	
		UpdateData(FALSE);
	}
}


void CReceiveDlg::OnBnClickedMfcbtnReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strRecvAddr = "";
	m_strRecvLabel = "";
	m_strMoney = "";
	m_strRecvMsg = "";

	UpdateData(FALSE);
}


BOOL CReceiveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{  

		switch(pMsg->wParam) 
		{  

		case VK_RETURN: //回车   
			return TRUE;  

		case VK_ESCAPE: //ESC  
			return TRUE;  

		}  

	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}
