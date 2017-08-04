// TradeRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "TradeRecordDlg.h"
#include "afxdialogex.h"
#include "TxDetailDlg.h"


// CTradeRecordDlg 对话框

IMPLEMENT_DYNAMIC(CTradeRecordDlg, CDialogEx)

CTradeRecordDlg::CTradeRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTradeRecordDlg::IDD, pParent)
{
	m_nConut = 0 ;
	m_pagesize = 14;
	m_offset = 0;
}

CTradeRecordDlg::~CTradeRecordDlg()
{
}

void CTradeRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOBOX_ALL, m_cboAll);
	DDX_Control(pDX, IDC_COMBOBOX_SEND, m_cboSend);
	DDX_Control(pDX, IDC_LIST_TRADERECORD, m_lstRecord);
	DDX_Control(pDX, IDC_STATIC_PAGE, m_staPage);
}


BEGIN_MESSAGE_MAP(CTradeRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBTN_TRADE_UP, &CTradeRecordDlg::OnBnClickedMfcbtnTradeUp)
	ON_BN_CLICKED(IDC_MFCBTN_TRADE_NEX, &CTradeRecordDlg::OnBnClickedMfcbtnTradeNex)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_ALL, &CTradeRecordDlg::OnCbnSelchangeComboboxAll)
	ON_CBN_SELCHANGE(IDC_COMBOBOX_SEND, &CTradeRecordDlg::OnCbnSelchangeComboboxSend)
	ON_MESSAGE(MSG_USER_TRANSRECORD_UI , &CTradeRecordDlg::OnShowListCtrl )
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRADERECORD, &CTradeRecordDlg::OnNMDblclkListTraderecord)
END_MESSAGE_MAP()


// CTradeRecordDlg 消息处理程序


BOOL CTradeRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		//GetDlgItem(IDC_MFCBTN_TRADE_UP)->SetWindowText("Previous page");
		//GetDlgItem(IDC_MFCBTN_TRADE_NEX)->SetWindowText("Next page");
	}

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[4]  = {
		{theApp.m_bChinese ? "日期" : "Date",      200},
		{theApp.m_bChinese ? "类型" : "Type",      100},
		{theApp.m_bChinese ? "地址" : "Address",      300}, 
		{theApp.m_bChinese ? "金额" : "Amount",      150}
	};
	m_lstRecord.SetBkColor(RGB(255,255,255));       
	m_lstRecord.SetRowHeigt(23);               
	m_lstRecord.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstRecord.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstRecord.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstRecord.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <4 ; i++  ) {
		m_lstRecord.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_cboAll.InsertString(0, theApp.m_bChinese ? _T("全部") : _T("All"));
	m_cboAll.InsertString(1,theApp.m_bChinese ? _T("接收") : _T("Receive"));
	m_cboAll.InsertString(2,theApp.m_bChinese ? _T("发送") : _T("Send"));
	m_cboAll.InsertString(3,theApp.m_bChinese ? _T("挖矿所得") : _T("Mining"));
	m_cboAll.InsertString(4,theApp.m_bChinese ? _T("合约") : _T("Contract"));
	m_cboAll.InsertString(5,theApp.m_bChinese ? _T("激活") : _T("Actived"));

	m_cboSend.InsertString(0,theApp.m_bChinese ? _T("全部") : _T("All"));
	m_cboSend.InsertString(1,theApp.m_bChinese ? _T("今天") : _T("Today"));
	m_cboSend.InsertString(2,theApp.m_bChinese ? _T("本周") : _T("This week"));
	m_cboSend.InsertString(3,theApp.m_bChinese ? _T("本月") : _T("This month"));
	m_cboSend.InsertString(4,theApp.m_bChinese ? _T("上月") : _T("Last month"));
	m_cboSend.InsertString(5,theApp.m_bChinese ? _T("今年") : _T("This year"));
	m_cboSend.InsertString(6,theApp.m_bChinese ? _T("昨天") : _T("Yesterday"));

	m_cboAll.SetCurSel(0);
	m_cboSend.SetCurSel(0);

	OninitializeList();

	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_TRANSRECORD_UI ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void  CTradeRecordDlg::OninitializeList()
{
	ShowComboxCotent();
}


string CTradeRecordDlg::GetConditonTime(){
	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	string strSendTime;
	strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);

	CString text;
	int sel = m_cboSend.GetCurSel();
	if (sel < 0)
	{
		return _T("");
	}
	m_cboSend.GetLBText(sel,text);
	uistruct::TRANSRECORDLIST pListInfo;

	string conditon;

	if (strcmp(text,  theApp.m_bChinese ? _T("全部") : _T("All") ) == 0)
	{
		return _T("");
	}else if (strcmp(text, theApp.m_bChinese ? _T("今天") : _T("Today") ) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);

		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon += strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text, theApp.m_bChinese ? _T("本周") : _T("This week") ) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = 0;
		if (curTime.wDayOfWeek == 0)
		{
			mincurtime = UiFun::SystemTimeToTimet(curTime);
		}else{
			INT64 differ = 86400*curTime.wDayOfWeek;
			mincurtime = UiFun::SystemTimeToTimet(curTime);
			mincurtime = maxcurtime -mincurtime;
			mincurtime = maxcurtime - (differ+mincurtime);
		}

		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("本月") : _T("This month")) == 0)
	{
		curTime.wDay =1;
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);

		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("上月"): _T("Last month")) == 0)
	{
		INT64 maxcurtime = 0;
		INT64 mincurtime = 0;
		curTime.wDay =1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		maxcurtime = UiFun::SystemTimeToTimet(curTime);
		if (curTime.wMonth == 1)
		{
			curTime.wMonth = 12;
			curTime.wYear -=1;

		}else{
			curTime.wMonth -= 1;
		}
		mincurtime = UiFun::SystemTimeToTimet(curTime);

		conditon= strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("今年") : _T("This year")) == 0)
	{
		curTime.wMonth =1;
		curTime.wDay=1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);

		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("昨天") : _T("Yesterday")) == 0)
	{
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);
		INT64 mincurtime = maxcurtime - 24*60*60;

		conditon=strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}
	return _T("");
}

string CTradeRecordDlg::GetConditonTxType(int &operate){
	CString text;
	int sel = m_cboAll.GetCurSel();
	if (sel < 0)
	{
		return _T("");
	}
	m_cboAll.GetLBText(sel,text);
	uistruct::TRANSRECORDLIST pListInfo;

	string conditon;

	if (strcmp(text,theApp.m_bChinese ? _T("全部") : _T("All")) == 0)
	{
		return _T("");
	}else if (strcmp(text,theApp.m_bChinese ? "接收" : "Receive") == 0)
	{
		operate = 1;

		conditon=" tx_type='COMMON_TX' and (state =2 or state =3)";
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("发送") : _T("Send")) == 0)
	{
		operate = 2;
		conditon=" tx_type='COMMON_TX' and (state =1 or state =3)";
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("挖矿所得") : _T("Mining")) == 0)
	{
		conditon=" tx_type='REWARD_TX'";
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("合约") : _T("Contract")) == 0)
	{
		conditon=" tx_type='CONTRACT_TX'";
		return conditon;
	}else if (strcmp(text,theApp.m_bChinese ? _T("激活") : _T("Actived")) == 0)
	{
		conditon=" tx_type='REG_ACCT_TX'";
		return conditon;
	}
	return "";
}

void  CTradeRecordDlg::ShowComboxCotent()
{
	int operate = 0;

	uistruct::TRANSRECORDLIST pListInfo;
	string condtion = GetConditonStr(operate);
	string strCond0 =  condtion;

	string tempConditon = "confirm_height=0 and ";
	tempConditon +=strCond0;
	strCond0 = tempConditon;

	theApp.m_SqliteDeal.GetTransactionList(strCond0, &pListInfo); 

	uistruct::TRANSRECORDLIST pListInfo1;
	string strCond = " confirm_height!=0 and "+ condtion;;// + _T(" LIMIT 17 OFFSET 0") ;
	strCond += strprintf(" LIMIT %d OFFSET 0",(m_pagesize-pListInfo.size()));
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pListInfo1); 

	m_offset = pListInfo.size();
	pListInfo.insert(pListInfo.end(),pListInfo1.begin(),pListInfo1.end());

	int nPage = 0 ;
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem("t_transaction",strCond);
	//	if ( nItem == 0 ) return ;
	if ( nItem%m_pagesize != 0  ) {
		nPage = nItem/m_pagesize + 1 ;
	}else {
		nPage =  nItem/m_pagesize ;
	}
	string temp;
	temp =strprintf(theApp.m_bChinese ? "%s%d页" : "%s %d pages",theApp.m_bChinese ? _T("共") : _T("total") ,nPage );
	m_staPage.SetWindowText(temp.c_str());

	//Invalidate();

	OnShowListCtrl(pListInfo);
}

void long2time(unsigned long nTime, CString& strTime)
{ 
	CTime objTime(nTime);    
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),
		objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond());
}

void CTradeRecordDlg::OnShowListCtrl(uistruct::TRANSRECORDLIST pListInfo){

	m_lstRecord.DeleteAllItems();
	if (pListInfo.size() == 0)
	{
		return;
	}

	m_pListInfo.clear();
	m_pListInfo.insert(m_pListInfo.begin(),pListInfo.begin(),pListInfo.end());

	int count = (m_nConut)*m_pagesize+1;
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	
	std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
	for ( const_it = pListInfo.begin() ; const_it != pListInfo.end() ; const_it++ ) {

		nSubIdx = 0;
		unsigned long time = const_it->confirmedtime;

		CString strTime = "-";
		if(time != 0)
		{
			long2time(time, strTime);
		}
		m_lstRecord.InsertItem(i, strTime);					//日期

		string txtype = const_it->txtype;
		if (!strcmp(txtype.c_str(),"REWARD_TX"))
		{
			strShowData = theApp.m_bChinese ? _T("挖矿") : _T("Mining");
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData= theApp.m_bChinese ? _T("激活") : _T("Actived") ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			if (const_it->state == 3)
			{
				strShowData= theApp.m_bChinese ? _T("转账<平>") : _T("Transfer<In>");
			}else if (const_it->state == 2)
			{
				strShowData= theApp.m_bChinese ?  _T("转账<收>") : _T("Transfer<Receive>");
			}else if (const_it->state == 1)
			{
				strShowData= theApp.m_bChinese ? _T("转账<发>") : _T("Transfer<Send>") ;
			}else
			{
				strShowData= theApp.m_bChinese ? _T("转账") : _T("Transfer") ;
			}
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData= theApp.m_bChinese ? _T("合约") : _T("Contract");
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData= theApp.m_bChinese ? _T("注册") : _T("Register");
		}
		m_lstRecord.SetItemText( i , ++nSubIdx, strShowData.c_str());   //交易类型


		m_lstRecord.SetItemText(i , ++nSubIdx , const_it->addr.c_str() );   //源地址

		strShowData= strprintf("%.2f" , const_it->money ) ;
		m_lstRecord.SetItemText(i , ++nSubIdx , strShowData.c_str() );   //交易金额

		/*m_lstRecord.SetItemText(i , ++nSubIdx , const_it->desaddr.c_str() );   //目的地址


		string txhash= const_it->txhash.substr(0,25)+"...";
		strShowData = strprintf("%s", txhash.c_str()); 
		m_lstRecord.SetItemText(i , ++nSubIdx , strShowData.c_str() );  //交易hash
		*/

		//TRACE("item:%d, id:%s\n", i, const_it->txhash.c_str());

		i++;
	}
}

void CTradeRecordDlg::OnBnClickedMfcbtnTradeUp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut -= 1 ;
	ShowPageCotent(m_nConut);
}


void CTradeRecordDlg::OnBnClickedMfcbtnTradeNex()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut += 1 ;
	ShowPageCotent(m_nConut);
}


void CTradeRecordDlg::OnCbnSelchangeComboboxAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut = 0;
	ShowComboxCotent();
}


void CTradeRecordDlg::OnCbnSelchangeComboboxSend()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut = 0;
	ShowComboxCotent();
}

LRESULT CTradeRecordDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	int type = (int)wParam;
	switch(type)
	{
	case WM_INSERT:
		{

			if ( 0 == m_nConut ) {   //只有当前是首页 , 才去显示
				m_lstRecord.DeleteAllItems();
				OninitializeList();
			}
		}
		break;
	case WM_REMOVETX:
		{
			if ( 0 == m_nConut ) {   //只有当前是首页 , 才去显示
				m_lstRecord.DeleteAllItems();
				OninitializeList();
			}
		}
		break;
	case WM_UPDATE:
		{
			if ( 0 == m_nConut ) {   //只有当前是首页 , 才去显示
				m_lstRecord.DeleteAllItems();
				OninitializeList();
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

void CTradeRecordDlg::ShowPageDataInfo(int nConut)
{
	nConut -=  1 ;
	m_nConut = nConut ;
	ShowPageCotent(nConut);
	return ;
}

string CTradeRecordDlg::GetConditonStr(int &operate)
{
	string condtion ="";
	string temp ="";

	temp = GetConditonTxType(operate);
	if (temp !="")
	{
		condtion = GetConditonTxType(operate);
	}

	if (condtion !="")
	{
		temp = GetConditonTime();
		if (temp !=_T(""))
		{
			condtion+=strprintf( " and %s",temp.c_str());
		}


	}else{
		temp = GetConditonTime();
		if (temp != _T(""))
		{
			condtion += strprintf( " %s",temp.c_str());
		}
	}

	if (condtion =="")
	{
		condtion ="1=1";
	}
	condtion+=" order by confirmed_time DESC";
	return condtion;
}

void  CTradeRecordDlg::ShowPageCotent(int pageIndex)
{
	if ( 0 > pageIndex ){
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("已经是最开始页了!") ,_T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("It's already the start page!") ,_T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return ;
	} 
	if (pageIndex == 0)
	{
		ShowComboxCotent();
		return;
	}
	string strCond ;
	uistruct::TRANSRECORDLIST pListInfo;
	int num = (pageIndex * m_pagesize - m_offset)<0?0:(pageIndex * m_pagesize - m_offset);
	std::stringstream strTemp;  
	string str;
	strTemp<< num;
	strTemp>>str;
	int operate = 0;
	string condtion = GetConditonStr(operate);
	strCond =  condtion + strprintf(" LIMIT %d OFFSET ",m_pagesize) + str ;
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pListInfo); 
	if ( 0 == pListInfo.size() ) {
		m_nConut -= 1 ;
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("已经是最后页了!") , _T("提示") ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("It's the last page already!") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		string strpage;
		strpage = strprintf("%d",m_nConut + 1);
		//GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());
		//Invalidate();
		return ;
	}
	int nPage = 0 ;
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , condtion );
	if ( nItem == 0 ) return ;
	if ( nItem%m_pagesize != 0  ) {
		nPage = nItem/m_pagesize + 1 ;
	}else {
		nPage =  nItem/m_pagesize ;
	}
	string temp;
	temp =strprintf(theApp.m_bChinese ? "%s%d页" : "%s %d pages",theApp.m_bChinese ? _T("共") : _T("total") ,nPage);
	//GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	//GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	m_staPage.SetWindowText(temp.c_str());

	string strpage;
	strpage = strprintf("%d",m_nConut + 1);
	//GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	//Invalidate();

	OnShowListCtrl(pListInfo);
}


void CTradeRecordDlg::OnNMDblclkListTraderecord(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(-1 != pNMItemActivate->iItem) 
	{  
		int nRow = pNMItemActivate->iItem;

		if (nRow <0 || nRow > (int)m_pListInfo.size())
		{
			return ;
		}
		uistruct::REVTRANSACTION_t  txdetail =m_pListInfo.at(nRow);

		if (txdetail.txhash != _T(""))
		{
			theApp.m_strAddress.Format(_T("%s") ,txdetail.ToJson().c_str() ) ;
		}else
		{		
			theApp.m_strAddress =_T("");
		}
		CTxDetailDlg dlg;
		dlg.DoModal();
	}  
	*pResult = 0;
}


BOOL CTradeRecordDlg::PreTranslateMessage(MSG* pMsg)
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
