// OverviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "OverviewDlg.h"
#include "afxdialogex.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"
#include "AddrSelectDlg.h"

// COverviewDlg 对话框

IMPLEMENT_DYNAMIC(COverviewDlg, CDialogEx)

COverviewDlg::COverviewDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COverviewDlg::IDD, pParent)
{

}

COverviewDlg::~COverviewDlg()
{
}

void COverviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RECE_TRADE, m_lstReceTrade);
	DDX_Control(pDX, IDC_CREDITCONFIRM, m_staUnconfirmMoney);
	DDX_Control(pDX, IDC_CREDITAVAIL, m_staUsageMoney);
	DDX_Control(pDX, IDC_EDT_QUOTA_BYTIME, m_staQuotaByTime);
	DDX_Control(pDX, IDC_EDT_QUOTA_BYDAY, m_staQuotaByDay);
	DDX_Control(pDX, IDC_EDT_QUOTA_REMAIN, m_staQuotaRemain);
}


BEGIN_MESSAGE_MAP(COverviewDlg, CDialogEx)
	ON_MESSAGE(MSG_USER_MAIN_UI , &COverviewDlg::OnShowListCtrl)
END_MESSAGE_MAP()


// COverviewDlg 消息处理程序


BOOL COverviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC80)->SetWindowText("Balance:");
		GetDlgItem(IDC_STATIC81)->SetWindowText("Available Balance");
		GetDlgItem(IDC_STATIC82)->SetWindowText("To be confirmed");
		GetDlgItem(IDC_STATIC_QUOTA)->SetWindowText("Quota:");
		GetDlgItem(IDC_STATIC_TIME_TITLE)->SetWindowText("Each quota:");
		GetDlgItem(IDC_STATIC_DAY_TITLE)->SetWindowText("Day quota:");
		GetDlgItem(IDC_STATIC_LEFT_TITLE)->SetWindowText("Remain quota:");
		GetDlgItem(IDC_STATIC85)->SetWindowText("Recent transactions:");
	}
	*/
	// TODO:  在此添加额外的初始化

	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[4]  = {
		{theApp.m_bChinese ? "源地址" : "Source Address" ,      300},
		{theApp.m_bChinese ? "转账金额" : "Amount" ,      100},
		{theApp.m_bChinese ? "目的地址" : "Destination Address",      300}
	};
	m_lstReceTrade.SetBkColor(RGB(255,255,255));       
	m_lstReceTrade.SetRowHeigt(23);               
	m_lstReceTrade.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstReceTrade.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstReceTrade.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstReceTrade.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <3 ; i++  ) {
		m_lstReceTrade.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	OnInitialCtrlText();

	m_staUnconfirmMoney.SetFont(100, _T("黑体"));
	m_staUsageMoney.SetFont(100, _T("黑体"));
	m_staQuotaByTime.SetFont(100, _T("黑体"));
	m_staQuotaByDay.SetFont(100, _T("黑体"));
	m_staQuotaRemain.SetFont(100, _T("黑体"));

	if(theApp.m_strCurEntAddress.length() == 0)
	{
		GetDlgItem(IDC_STATIC_QUOTA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DAY_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEFT_TITLE)->ShowWindow(SW_HIDE);
	}
	else
	{
		SetEntInfo(theApp.m_strCurEntAddress);
	}
	
	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_MAIN_UI );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COverviewDlg::SetEntInfo(string strEntAddr)
{
	static BOOL bInited = FALSE;

	CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;

	if( 2 == theApp.m_bIsAdmin)
	{
		GetDlgItem(IDC_STATIC_QUOTA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DAY_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEFT_TITLE)->ShowWindow(SW_HIDE);
		return;
	}

	if(pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
	{
		GetDlgItem(IDC_STATIC_QUOTA)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TIME_TITLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DAY_TITLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_LEFT_TITLE)->ShowWindow(SW_SHOW);
	}
	else
	{
		if(bInited) 
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, "地址不存在或未激活", "提示", MB_OK);
			else
				::MessageBoxEx(m_hWnd, "The address does not exist or is not active", "tip", MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		}
		GetDlgItem(IDC_STATIC_QUOTA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TIME_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DAY_TITLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LEFT_TITLE)->ShowWindow(SW_HIDE);
		bInited = TRUE;
		return;
	}

	bInited = TRUE;

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}

	LIST_APP_DATA appData;
	BOOL bFound = pCompanyInfoDlg->FindEntDataByAddr(strEntAddr, appData);
	if(bFound)
	{
		CString strByTime;
		strByTime.Format("%0.2f", appData.fQuotaByTime);

		m_staQuotaByTime.SetWindowText(strByTime);
		m_staQuotaByTime.UpdateCtrl();

		CString strByDay;
		strByDay.Format("%0.2f", appData.fQuotaByDay);
		m_staQuotaByDay.SetWindowText(strByDay);
		m_staQuotaByDay.UpdateCtrl();

		CString strLeftByDay;
		if(appData.bWithdrawByTodayValid){
			if(appData.fQuotaByDay < appData.fWithdrawByToday)
			{
				strLeftByDay = "0.00";
			}
			else
			{
				strLeftByDay.Format("%0.2f", ((appData.fQuotaByDay - appData.fWithdrawByToday)));	
			}
			
		}
		else
		{
			strLeftByDay = strByDay;
		}
		m_staQuotaRemain.SetWindowText(strLeftByDay);
		m_staQuotaRemain.UpdateCtrl();

		pCompanyInfoDlg->ShowWithdrawBtn();
		pCompanyInfoDlg->ShowRechargeBtn();
	}

	//pCompanyInfoDlg->OnTimer(0);
}

void COverviewDlg::SetEntInfoEx()
{

	CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;

	if( 2 == theApp.m_bIsAdmin)
	{
		return;
	}

	if(pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
	{
	}
	else
	{
		return;
	}

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}

	LIST_APP_DATA appData;
	BOOL bFound = pCompanyInfoDlg->FindEntDataByAddr(theApp.m_strCurEntAddress, appData);
	if(bFound)
	{
		CString strByTime;
		strByTime.Format("%0.2f", appData.fQuotaByTime);

		m_staQuotaByTime.SetWindowText(strByTime);
		m_staQuotaByTime.UpdateCtrl();

		CString strByDay;
		strByDay.Format("%0.2f", appData.fQuotaByDay);
		m_staQuotaByDay.SetWindowText(strByDay);
		m_staQuotaByDay.UpdateCtrl();

		CString strLeftByDay;
		if(appData.bWithdrawByTodayValid){
			if(appData.fQuotaByDay < appData.fWithdrawByToday)
			{
				strLeftByDay = "0.00";
			}
			else
			{
				strLeftByDay.Format("%0.2f", ((appData.fQuotaByDay - appData.fWithdrawByToday)));	
			}	
		}
		else
		{
			strLeftByDay = strByDay;
		}
		m_staQuotaRemain.SetWindowText(strLeftByDay);
		m_staQuotaRemain.UpdateCtrl();

	}
}

void COverviewDlg::OnInitialCtrlText() 
{
	string strCommand,strShowData;
	strCommand  = "0";
	string strCond;

	double dmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_wallet_address") ,_T("money"), _T(" 1=1 "));

	
	if (-1 == dmoney)
	{
		m_staUsageMoney.SetWindowText(_T("0.0")) ;
	}else{
		string strMoney;
		strMoney = strprintf("%.3lf", dmoney);
		strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
		m_staUsageMoney.SetWindowText(strMoney.c_str()) ;
		m_staUsageMoney.UpdateCtrl();
	}

	strCond=" confirm_height = 0 ";

	dmoney =  theApp.m_SqliteDeal.GetTableItemSum("t_transaction" , _T("money") , strCond) ;


	if (dmoney<0)
	{
		m_staUnconfirmMoney.SetWindowText(_T("0.0")) ;
	}else{
		string strMoney;
		strMoney = strprintf("%.3lf", dmoney);
		m_staUnconfirmMoney.SetWindowText(strMoney.c_str()) ;
		m_staUnconfirmMoney.UpdateCtrl();
	}
	

	uistruct::TRANSRECORDLIST pTransaction;
	string strSource;
	strCond=" tx_type='COMMON_TX' order by confirmed_time desc limit 6";
	theApp.m_SqliteDeal.GetTransactionList(strCond,&pTransaction);

	string desStr ="";
	if (pTransaction.size() != 0  ) {
		int nSubIdx = 0 , i = 0 ;
		string strShowaddr ;
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin() ; const_it != pTransaction.end()&&i<6 ; const_it++ ) {

			strSource = strprintf("%.4f",const_it->money);
			strShowaddr= strprintf("%s",const_it->addr.c_str());
			desStr = strprintf("%s",const_it->desaddr.c_str());
		
			int nItem = m_lstReceTrade.GetItemCount();
			if(nItem < i + 1)
			{
				m_lstReceTrade.InsertItem(i, strShowaddr.c_str());
			}
			m_lstReceTrade.SetItemText(i, 1, strSource.c_str());
			m_lstReceTrade.SetItemText(i, 2, desStr.c_str());
			i++;
		}
	}
}

void COverviewDlg::SetCtrlText() 
{
	m_lstReceTrade.DeleteAllItems();
	CPostMsg postmsg;
	if (!theApp.m_msgUIMainDlgQueue.pop(postmsg))
	{
		LogPrint("INFO","m_UiManDlgQueue Emptry\r\n");
		return ;
	}

	uistruct::MINDLG_T maindlg; 
	string strTemp = postmsg.GetData();
	maindlg.JsonToStruct(strTemp.c_str());

	string strMoney = "";
	strMoney = strprintf("%s",maindlg.money.c_str());//)= _T(maindlg.money.c_str());
	strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
	m_staUsageMoney.SetWindowText(strMoney.c_str());
	m_staUsageMoney.UpdateCtrl();

	strMoney = strprintf("%s",maindlg.unconfirmmoney.c_str());
	strMoney = CSoyPayHelp::getInstance()->DisPlayMoney(strMoney);
	m_staUnconfirmMoney.SetWindowText(strMoney.c_str());
	m_staUnconfirmMoney.UpdateCtrl();

	string strCommand,strShowData;
	string strDes = "";
	string addr1 = maindlg.addr1;
	uistruct::REVTRANSACTION_t temp;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);

		strCommand =strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 1)
		{
			m_lstReceTrade.InsertItem(0, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(0, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(0, 2, strDes.c_str());

	}

	addr1 = maindlg.addr2;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);

		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 2)
		{
			m_lstReceTrade.InsertItem(1, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(1, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(1, 2, strDes.c_str());
	}

	addr1 = maindlg.addr3;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);

		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 3)
		{
			m_lstReceTrade.InsertItem(2, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(2, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(2, 2, strDes.c_str());

	}
	addr1 = maindlg.addr4;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes =strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 4)
		{
			m_lstReceTrade.InsertItem(3, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(3, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(3, 2, strDes.c_str());
	}
	addr1 = maindlg.addr5;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 5)
		{
			m_lstReceTrade.InsertItem(4, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(4, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(4, 2, strDes.c_str());
	}

	addr1 = maindlg.addr6;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		strCommand = strprintf("%.4f",temp.money*COIN);
		strShowData = strprintf("%s",temp.addr.c_str());
		strDes = strprintf("%s",temp.desaddr.c_str());

		int nItem = m_lstReceTrade.GetItemCount();
		if(nItem < 6)
		{
			m_lstReceTrade.InsertItem(5, strShowData.c_str());
		}
		m_lstReceTrade.SetItemText(5, 1, strCommand.c_str());
		m_lstReceTrade.SetItemText(5, 2, strDes.c_str());
	}

}

LRESULT COverviewDlg::OnShowListCtrl(WPARAM wParam, LPARAM lParam)
{

	//更新数据
	if (wParam == WM_UPWALLET)
	{
		//隐藏"钱包正在同步"static
		//			GetDlgItem(IDC_STATIC_WALLET)->ShowWindow(SW_HIDE);
	}else{
		SetCtrlText();
	}
	return 0 ;
}


BOOL COverviewDlg::PreTranslateMessage(MSG* pMsg)
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
