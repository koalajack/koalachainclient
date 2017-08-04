// AddrSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "AddrSelectDlg.h"
#include "afxdialogex.h"
#include "NewAddressDlg.h"
#include "SignAccountsDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "KoalaDlg.h"

// CAddrSelectDlg 对话框

IMPLEMENT_DYNAMIC(CAddrSelectDlg, CDialogEx)

CAddrSelectDlg::CAddrSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddrSelectDlg::IDD, pParent)
{
	
}

CAddrSelectDlg::~CAddrSelectDlg()
{
}

void CAddrSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDRSELECT, m_lstAddrSelect);
	//DDX_Control(pDX, IDC_EDIT_ITEM, m_Edit);
}


BEGIN_MESSAGE_MAP(CAddrSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADDRSELECT_NEW, &CAddrSelectDlg::OnBnClickedAddrselectNew)
	ON_MESSAGE(MSG_USER_RECIVE_UI , &CAddrSelectDlg::OnShowListCtrl )
	ON_BN_CLICKED(IDC_ADDRSELECT_ACTIVATE, &CAddrSelectDlg::OnBnClickedAddrselectActivate)
	ON_BN_CLICKED(IDC_ADDRSELECT_COPY, &CAddrSelectDlg::OnBnClickedAddrselectCopy)
	ON_BN_CLICKED(IDC_ADDRSELECT_EXPORT, &CAddrSelectDlg::OnBnClickedAddrselectExport)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADDRSELECT, &CAddrSelectDlg::OnDblclkListAddrselect)

	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_ADDRSELECT, &CAddrSelectDlg::OnEndlabeleditListAddrselect)
//	ON_NOTIFY(NM_CLICK, IDC_LIST_ADDRSELECT, &CAddrSelectDlg::OnClickListAddrselect)
	ON_BN_CLICKED(IDC_MFCBTN_FILTER, &CAddrSelectDlg::OnBnClickedMfcbtnFilter)
	ON_BN_CLICKED(IDC_MFCBTN_CLEAR, &CAddrSelectDlg::OnBnClickedMfcbtnClear)
	ON_BN_CLICKED(IDC_MFCBTN_COLLECTION, &CAddrSelectDlg::OnBnClickedMfcbtnCollection)
END_MESSAGE_MAP()


// CAddrSelectDlg 消息处理程序


BOOL CAddrSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditCreated = FALSE;				//初始化标志位，表示还没有创建编辑框
	//m_lstAddrSelect.EditFilter(0, TRUE);

	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[4]  = {
		{theApp.m_bChinese ? "标签" : "Label" ,      200},
		{theApp.m_bChinese ? "地址" : "Address",      250},
		{theApp.m_bChinese ? "状态" : "Status",      100}, 
		{theApp.m_bChinese ? "余额" : "Balance",      200}
	};
	m_lstAddrSelect.SetBkColor(RGB(255,255,255));       
	m_lstAddrSelect.SetRowHeigt(23);               
	m_lstAddrSelect.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstAddrSelect.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstAddrSelect.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstAddrSelect.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <4 ; i++  ) {
		m_lstAddrSelect.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	ShowListInfo();

	m_RowCount = m_lstAddrSelect.GetItemCount();  //获取行数

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC_LABEL)->SetWindowText("Label:");
		GetDlgItem(IDC_STATIC_ADDR)->SetWindowText("Address:");
		GetDlgItem(IDC_MFCBTN_FILTER)->SetWindowText("Query");
		GetDlgItem(IDC_MFCBTN_CLEAR)->SetWindowText("Clear");
		GetDlgItem(IDC_MFCBTN_COLLECTION)->SetWindowText("Collect");
		GetDlgItem(IDC_ADDRSELECT_NEW)->SetWindowText("New");
		GetDlgItem(IDC_ADDRSELECT_ACTIVATE)->SetWindowText("Activate");
		GetDlgItem(IDC_ADDRSELECT_COPY)->SetWindowText("Copy");
		GetDlgItem(IDC_ADDRSELECT_EXPORT)->SetWindowText("Export");
	}


	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_RECIVE_UI ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAddrSelectDlg::ShowListInfo()
{
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 order by reg_id"), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_lstAddrSelect.DeleteAllItems();

	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;

		uistruct::LISTADDR_t address = const_it->second;

		m_lstAddrSelect.InsertItem( i , address.Label.c_str()) ;

		m_lstAddrSelect.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;

		if (address.bSign == 1)
		{
			strShowData=_T(theApp.m_bChinese ? "已激活" : "Activated") ;
		}else{
			strShowData=_T(theApp.m_bChinese ? "未激活" : "Not Active");
		}

		m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		strShowData = strprintf("%.2f" , address.fMoney ) ;
		m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		i++;
	}

}


void CAddrSelectDlg::OnBnClickedAddrselectNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CNewAddressDlg dlg;
	dlg.DoModal();
}

void CAddrSelectDlg::ModifyListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_msgUIRecDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) <= 0)
	{
		TRACE("map ModifyListCtrlItem ERROR");
		return;
	}
	
	m_MapAddrInfo[addr.address]=addr;
	
	int count = m_lstAddrSelect.GetItemCount();
	for(int i = 0; i < count; i++)
	{
		CString str = m_lstAddrSelect.GetItemText(i, 1); // 这个函数名具体忘了，就是取得每个item第0列的值
		if(strcmp(str,addr.address.c_str()) == 0)
		{
		
			int nSubIdx = 0;
			string  strShowData;
	
	
			m_lstAddrSelect.SetItemText(i , nSubIdx , addr.Label.c_str() ) ;


			m_lstAddrSelect.SetItemText(i , ++nSubIdx , addr.address.c_str() ) ;

			if (addr.bSign == 1)
			{
				strShowData=_T(theApp.m_bChinese ? "已激活" : "Activated");
			}else{
				strShowData = _T(theApp.m_bChinese ? "未激活" : "Not Active") ;
			}

			m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

			strShowData = strprintf("%.2f" ,addr.fMoney ) ;
			m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;
			break;
		}
	}
}
void   CAddrSelectDlg::InsertListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_msgUIRecDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) > 0){
		TRACE("map InsertListCtrlItem ERROR");
		return ;
	}

	int count = m_lstAddrSelect.GetItemCount();

	int nSubIdx = 0,i =count;

	string  strShowData;

	//m_listCtrl.SetItemText(i , ++nSubIdx , addr.RegID.c_str() ) ;


	m_lstAddrSelect.InsertItem( i , addr.Label.c_str() ) ; //标签
	
	m_MapAddrInfo[addr.address]=addr;

	    //地址
	m_lstAddrSelect.SetItemText( i , ++nSubIdx, addr.address.c_str() ); 

	if (addr.bSign == 1)
	{
		strShowData= _T(theApp.m_bChinese ? "已激活" : "Activated");
	}else{
		strShowData= _T(theApp.m_bChinese ? "未激活" : "Not Active") ;
	}

	m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

	strShowData =strprintf("%.2f" , addr.fMoney ) ;
	m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

}

LRESULT  CAddrSelectDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{
			ModifyListCtrlItem();
			break;
		}
		break;
	case WM_UP_NEWADDRESS:
		{
			InsertListCtrlItem();
			break;
		}
		break;
	default:
		break;

	}
	
	return 1;
}


void CAddrSelectDlg::OnBnClickedAddrselectActivate()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_verAMAddr.clear();

	if (!theApp.m_bIsSyncBlock)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("同步未完成,不能发送激活交易"),_T("提示") ,MB_OK );
		else 
			::MessageBoxEx(m_hWnd, _T("Synchronization is not completed and can not be sent to activate the transaction"),_T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		return;
	}

	int nSubIdx = 0 , i = 0 ;

	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;

		uistruct::LISTADDR_t address = const_it->second;

		if (address.bSign == 1)
		{
			
		}else{
			AMADDR amAddr;
			amAddr.strAddr = address.address.c_str();
			amAddr.fMoney = address.fMoney;
			theApp.m_verAMAddr.push_back(amAddr);
		}
	}

	if(theApp.m_verAMAddr.empty())
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("没有可激活的地址") ,_T("提示") ,MB_OK );
		else 
			::MessageBoxEx(m_hWnd, _T("There is no address that can be activated") ,_T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		return;
	}

	
	//CString StrShow;
	//POSITION pos = m_lstAddrSelect.GetFirstSelectedItemPosition() ;
	//if ( pos ) {
	//	do 
	//	{
	//		int nRow = m_lstAddrSelect.GetNextSelectedItem(pos) ;
	//		CString str = m_lstAddrSelect.GetItemText(nRow, 1);
	//		string addr = strprintf("%s",str);
	//		if(!m_MapAddrInfo.count(addr))
	//		{
	//			TRACE("ERROR");
	//			StrShow.Format(_T("地址不存在"));

	//			::MessageBox(m_hWnd, StrShow ,_T("提示") ,MB_OK );
	//			return;
	//		}
	//		uistruct::LISTADDR_t te =  m_MapAddrInfo[addr];
	//		//uistruct::LISTADDR_t * pAddrItem = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
	//		if (te.fMoney <=0)
	//		{
	//			StrShow.Format(_T("账户余额为零,不能激活!"));
	//			
	//			::MessageBox(m_hWnd, StrShow ,_T("提示") ,MB_OK );
	//			return;
	//		}
	//		if(te.bSign) 
	//		{
	//			StrShow.Format(_T("账户已激活!"));

	//			::MessageBox(m_hWnd, StrShow, _T("提示") ,MB_OK );
	//			return;
	//		}
	//		//theApp.m_strAddress.Format(_T("%s") ,te.address.c_str() ) ;
	//		theApp.m_verStrAddress.push_back(te.address.c_str());
	//		
	//	} while (pos);

		/*CSignAccountsDlg dlg;
		dlg.DoModal();

		}else{
		StrShow.Format(_T("请选择地址!"));

		::MessageBox(m_hWnd, StrShow , _T("提示"),MB_OK );
		}*/

		CSignAccountsDlg dlg(this);
		if(IDOK == dlg.DoModal())
		{
			std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
			for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {

			}
		}
}


void CAddrSelectDlg::OnBnClickedAddrselectCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	string StrShow;
	POSITION pos = m_lstAddrSelect.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstAddrSelect.GetNextSelectedItem(pos) ;
		//uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		CString source =m_lstAddrSelect.GetItemText(nRow, 1);
		//文本内容保存在source变量中
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			char * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(source));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
			StrShow = _T(theApp.m_bChinese ? "地址已复制到剪贴板" : "The address has been copied to the clipboard");

			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, StrShow.c_str() , _T(theApp.m_bChinese ?  "提示" : "tip") ,MB_OK );
			else
				::MessageBoxEx(m_hWnd, StrShow.c_str() , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		}
	}
	else{
		StrShow = _T(theApp.m_bChinese ?  "请选择地址!" : "Please select an address!");

		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, StrShow.c_str() , _T(theApp.m_bChinese ? "提示" : "tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, StrShow.c_str() , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}

void CAddrSelectDlg::GetActiveAddr(vector<uistruct::LISTADDR_t>& vecAddr)
{
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		uistruct::LISTADDR_t addr = const_it->second;
		if(addr.bSign == 1) {
			vecAddr.push_back(addr);
		}
	}
}

void CAddrSelectDlg::GetAllAddr(vector<uistruct::LISTADDR_t>& vecAddr)
{
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		uistruct::LISTADDR_t addr = const_it->second;
		vecAddr.push_back(addr);
	}
}

BOOL CAddrSelectDlg::AddrIsExsit(string addr)
{
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		uistruct::LISTADDR_t address = const_it->second;
		if((address.bSign == 1) && (address.address == addr)) {
			return TRUE;
		}
	}

	return FALSE;
}


void CAddrSelectDlg::OnBnClickedAddrselectExport()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = m_lstAddrSelect.GetItemCount();
	if (count == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T(theApp.m_bChinese ? "没有记录可以导出！" : "No record can be exported") ,_T(theApp.m_bChinese ? "提示" : "tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T(theApp.m_bChinese ? "没有记录可以导出！" : "No record can be exported") ,_T(theApp.m_bChinese ? "提示" : "tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		return;
	}
	CFileDialog dlg(FALSE,NULL,_T(theApp.m_bChinese ? "接收记录" : "Receive record"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,theApp.m_bChinese ?  "文件 (*.xls)|*.xls||" : "File (*.xls)|*.xls||");
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString strFile = dlg.GetPathName();
	if (!((CKoalaDlg*)(theApp.m_pMainWnd))->GetFileName(strFile,_T(".xls")))
	{
		return;
	}

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listheadr[5]  = {
		{theApp.m_bChinese ? _T("序号") : _T("NO") ,      50},
		{theApp.m_bChinese ? _T("标签") : _T("Label") ,    30},
		{theApp.m_bChinese ? _T("地址") : _T("Address") ,    40},
		{theApp.m_bChinese ? _T("激活状态") : _T("Active state") ,    10},
		{theApp.m_bChinese ? _T("余额") : _T("Balance"),  40}
	};

	COleVariant

	covTrue((short)TRUE),

	covFalse((short)FALSE),

	covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR);

	CApplication   app;

	CWorkbooks   books;

	CWorkbook   book;

	CWorksheets   sheets;

	CWorksheet   sheet;

	CRange   range;

	CFont0   font;



	if (!app.CreateDispatch(_T("Excel.Application")))

	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("可能是没有装office 导致创建失败！") , _T("提示") ,MB_OK );
		else 
			::MessageBoxEx(m_hWnd, _T("There may not be an installed office that caused the creation to fail！") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));


	int   iCol;

	int   m_cols   =   5;

	int   m_rows = count;

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL

	{

		UiFun::GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//得到第iCol+1列  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//设置列宽 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//设置行的高度

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array

	saRet.Create(VT_BSTR,   2,   numElements);

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	long   index[2];

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	for(int i = 0; i < count; i++)
	{
		CString strNo; //= m_lstAddrSelect.GetItemText(i, 0);
		strNo.Format("%d", i + 1);
		BSTR   bstr   =   strNo.AllocSysString();
		index[0] = i;
		index[1] = 0;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strLabel = m_lstAddrSelect.GetItemText(i, 0);
		bstr   =   strLabel.AllocSysString();
		index[0] = i;
		index[1] = 1;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strAddress = m_lstAddrSelect.GetItemText(i, 1);
		bstr   =   strAddress.AllocSysString();
		index[0] = i;
		index[1] = 2;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strActive = m_lstAddrSelect.GetItemText(i, 2);
		bstr   =   strActive.AllocSysString();
		index[0] = i;
		index[1] = 3;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);
		CString strMoney = m_lstAddrSelect.GetItemText(i, 3);
		bstr   =   strMoney.AllocSysString();
		index[0] = i;
		index[1] = 4;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);
	}



	range.put_Value2(COleVariant(saRet));


	saRet.Detach();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	book.SaveCopyAs(COleVariant(strFile));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	//book.ReleaseDispatch();  

	//books.ReleaseDispatch();  
	book.Close (covOptional, covOptional,covOptional);// 关闭Workbook对象
	books.Close(); 
	app.Quit();

	app.ReleaseDispatch();

}


BOOL CAddrSelectDlg::PreTranslateMessage(MSG* pMsg)
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


//void CAddrSelectDlg::OnBnClickedAddrselectSearch()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	CFuzzySearchDlg dlg;
//
//	dlg.DoModal();
//
//}


//void CAddrSelectDlg::OnBnClickedMfcbtnFilter()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CButton *pLabel = (CButton *)GetDlgItem(IDC_CHECK_LABEL);
//	CButton *pAddr = (CButton *)GetDlgItem(IDC_CHECK_ADDR);
//	CString strLabel;
//	CString strAddr;
//	
//	GetDlgItem(IDC_EDIT_FILTER_ADDR)->GetWindowTextA(strAddr);
//
//	if(pLabel->GetCheck() && (!pAddr->GetCheck()))
//	{
//		m_MapFilterAddrInfo.clear();
//		GetDlgItem(IDC_EDIT_FILTER_LABEL)->GetWindowText(strLabel);
//		strLabel = "label like '" + strLabel;
//		
//		strLabel += "%'";
//
//		theApp.m_SqliteDeal.GetWalletAddressList(strLabel.GetBuffer(), &m_MapFilterAddrInfo);
//		ShowListFilterInfo();
//	}
//	else if((!pLabel->GetCheck()) && (pAddr->GetCheck()))
//	{
//		m_MapFilterAddrInfo.clear();
//		GetDlgItem(IDC_EDIT_FILTER_ADDR)->GetWindowText(strAddr);
//		strAddr = "address like '" + strAddr;
//
//		strAddr += "%'";
//
//		theApp.m_SqliteDeal.GetWalletAddressList(strAddr.GetBuffer(), &m_MapFilterAddrInfo);
//		ShowListFilterInfo();
//	}
//	else if(pLabel->GetCheck() && (pAddr->GetCheck()))
//	{
//		m_MapFilterAddrInfo.clear();
//
//		GetDlgItem(IDC_EDIT_FILTER_LABEL)->GetWindowText(strLabel);
//		strLabel = "label like '" + strLabel;
//		strLabel += "%'";
//
//		GetDlgItem(IDC_EDIT_FILTER_ADDR)->GetWindowText(strAddr);
//		strAddr = "address like '" + strAddr;
//		strAddr += "%'";
//
//		strLabel += " and ";
//		strLabel += strAddr;
//
//		theApp.m_SqliteDeal.GetWalletAddressList(strLabel.GetBuffer(), &m_MapFilterAddrInfo);
//		ShowListFilterInfo();
//
//	}
//	else
//	{
//		ShowListInfo();
//	}
//}

void CAddrSelectDlg::ShowListFilterInfo()
{
	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_lstAddrSelect.DeleteAllItems();

	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapFilterAddrInfo.begin() ; const_it != m_MapFilterAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;

		uistruct::LISTADDR_t address = const_it->second;

		m_lstAddrSelect.InsertItem( i , address.Label.c_str()) ;

		m_lstAddrSelect.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;

		if (address.bSign == 1)
		{
			strShowData=theApp.m_bChinese ? _T("已激活") : _T("Actived") ;
		}else{
			strShowData=theApp.m_bChinese ? _T("未激活") : _T("Not Active");
		}

		m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		strShowData = strprintf("%.2f" , address.fMoney ) ;
		m_lstAddrSelect.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		i++;
	}
}


void CAddrSelectDlg::OnDblclkListAddrselect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	LVHITTESTINFO info;
	info.pt = pNMItemActivate->ptAction;

	POSITION pos = m_lstAddrSelect.GetFirstSelectedItemPosition();
	int iIndex = m_lstAddrSelect.GetNextSelectedItem(pos);

	if(iIndex >= 0)
	{
		m_lstAddrSelect.SetFocus();
		CEdit *pEdit = m_lstAddrSelect.EditLabel(iIndex);

	}

	//if(pNMItemActivate->iSubItem != 0)
	//{
	//	return;
	//}

	//NM_LISTVIEW  *pEditCtrl = (NM_LISTVIEW *)pNMHDR;
	//CRect  EditRect;
	//
	//if (m_NeedSave)
	//	{  
	//		CString  EditStr;
	//		m_Edit.GetWindowText(EditStr); //获取输入的内容
	//		m_lstAddrSelect.SetItemText(m_Item,m_SubItem,EditStr); //m_Item,m_SubItem这两个变量的值是原来的单元格的值
	//	}

	//	m_Item = pEditCtrl->iItem;
	//	m_SubItem = pEditCtrl->iSubItem;
	//	
	//	if (m_Item == -1)
	//	{
	//		return;
	//	}

	//	if (m_Item < m_RowCount)
	//	{
	//		m_NeedSave = TRUE;
	//		m_lstAddrSelect.GetSubItemRect(m_Item,m_SubItem,LVIR_LABEL,EditRect);
	//		if (m_Edit.m_hWnd == NULL)
	//		{
	//			m_Edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_CENTER|ES_WANTRETURN ,
	//				CRect(0,0,0,0),this,IDC_EDIT_ITEM);

	//			//IDC_EDIT已近在头文件中定义,这个很重要,很多时候会忽略,
	//			//网上找到的实现编辑办法中这个都没说明
	//			//我定义为#define  IDC_EDIT 0xffff

	//			m_Edit.ShowWindow(SW_HIDE); //Edit创建完后隐藏
	//			
	//			m_Edit.SetFont(m_lstAddrSelect.GetFont(),FALSE);//设置字体

	//		}
	//		m_Edit.SetParent(&m_lstAddrSelect); //将list control设置为父窗口,生成的Edit才能正确定位,这个也很重要,

	//		EditRect.SetRect(EditRect.left,EditRect.top,EditRect.left+m_lstAddrSelect.GetColumnWidth(m_SubItem),EditRect.bottom);
	//		//m_list.GetColumnWidth(m_SubItem)获取列的宽度

	//		CString strItem = m_lstAddrSelect.GetItemText(m_Item,m_SubItem);
	//		m_Edit.MoveWindow(&EditRect);
	//		m_Edit.ShowWindow(SW_SHOW);
	//		m_Edit.SetWindowText(strItem);
	//		m_Edit.SetFocus();//设置为焦点
	//		m_Edit.SetSel(0,-1);  //0,-1表示单元格内容全选中

	//	}
	//	else
	//	{
	//		m_Edit.ShowWindow(SW_HIDE);
	//	}

	*pResult = 0;
}

void CAddrSelectDlg::OnEndlabeleditListAddrselect(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	CString pName = pDispInfo->item.pszText;
	int iIndex = pDispInfo->item.iItem;
	m_lstAddrSelect.SetFocus();

	if((!pName.IsEmpty())&& iIndex >= 0)
	{
		string strLabel;
		string strCondition;
		CString strAddr;

		m_lstAddrSelect.SetItemText(iIndex, pDispInfo->item.iSubItem, pName);

		strAddr = m_lstAddrSelect.GetItemText(iIndex, 1);

		strLabel = strprintf("label = '%s'", pName);
		strCondition = strprintf("address = '%s'",  strAddr);


		theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address"), strLabel, strCondition);
	}
	
	*pResult = 0;
}


//void CAddrSelectDlg::OnClickListAddrselect(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	POSITION pos = m_lstAddrSelect.GetFirstSelectedItemPosition();
//	int iIndex = m_lstAddrSelect.GetNextSelectedItem(pos);
//	CRect rt;
//
//	if(iIndex >= 0)
//	{
//		//m_lstAddrSelect.SetFocus();
//
//		//m_lstAddrSelect.GetSubItemRect(pNMItemActivate->iItem, pNMItemActivate->iSubItem, LVIR_LABEL, rt);
//
//		CEdit *pEdit = m_lstAddrSelect.EditLabel(iIndex);
//		//pEdit->MoveWindow(rt);
//	}
//	*pResult = 0;
//}


void CAddrSelectDlg::OnBnClickedMfcbtnFilter()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strLabel;
	CString strAddr;
	GetDlgItem(IDC_EDIT_FILTER_LABEL)->GetWindowText(strLabel);
	GetDlgItem(IDC_EDIT_FILTER_ADDR)->GetWindowText(strAddr);

	if((strLabel != "") && (strAddr == ""))
	{
		m_MapFilterAddrInfo.clear();
		
		strLabel = "label like '" + strLabel;

		strLabel += "%'";

		theApp.m_SqliteDeal.GetWalletAddressList(strLabel.GetBuffer(), &m_MapFilterAddrInfo);
		ShowListFilterInfo();
	}
	else if((strLabel == "") && (strAddr != ""))
	{
		m_MapFilterAddrInfo.clear();
		
		strAddr = "address like '" + strAddr;

		strAddr += "%'";

		theApp.m_SqliteDeal.GetWalletAddressList(strAddr.GetBuffer(), &m_MapFilterAddrInfo);
		ShowListFilterInfo();
	}
	else if((strLabel != "") && (strAddr != ""))
	{
		m_MapFilterAddrInfo.clear();

		strLabel = "label like '" + strLabel;
		strLabel += "%'";

		strAddr = "address like '" + strAddr;
		strAddr += "%'";

		strLabel += " and ";
		strLabel += strAddr;

		theApp.m_SqliteDeal.GetWalletAddressList(strLabel.GetBuffer(), &m_MapFilterAddrInfo);
		ShowListFilterInfo();

	}
	else
	{
		ShowListInfo();
	}
}


void CAddrSelectDlg::OnBnClickedMfcbtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_FILTER_LABEL)->SetWindowText("");
	GetDlgItem(IDC_EDIT_FILTER_ADDR)->SetWindowText("");
}


void CAddrSelectDlg::OnBnClickedMfcbtnCollection()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}

	int res = 0;

	if(theApp.m_bChinese)
		res = ::MessageBox(m_hWnd, theApp.m_bChinese ? "是否进行钱包归集" : "Whether to carry out wallet collection" ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OKCANCEL );
	else
		res = ::MessageBoxEx(m_hWnd, theApp.m_bChinese ? "是否进行钱包归集" : "Whether to carry out wallet collection" ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OKCANCEL, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );

	if(IDOK == res)
	{
		CString strShow;
		POSITION pos = m_lstAddrSelect.GetFirstSelectedItemPosition() ;
		if ( pos ) { 
			{
				string strCommand;
				string strretshow;
				Json::Value root;
				Json::Reader reader;
				Json::Value value;

				int nRow = m_lstAddrSelect.GetNextSelectedItem(pos) ;
				CString str = m_lstAddrSelect.GetItemText(nRow, 1);
				string addr = strprintf("%s",str);
				if(!m_MapAddrInfo.count(addr))
				{
					TRACE("ERROR");
					strShow.Format(theApp.m_bChinese ? _T("地址不存在") : _T("The address does not exist"));

					if(theApp.m_bChinese)
						::MessageBox(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
					else
						::MessageBoxEx(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
					return;
				}

				strCommand = strprintf("%s %s","notionalpoolingbalance" ,addr );

				if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,strretshow))
				{
					TRACE("notionalpoolingbalance rpccmd error");
					return ;
				}

				if (!reader.parse(strretshow, root)) 
					return ;

				value = root["Tx"];

				if( 0 == value.size())
				{
					strShow.Format(theApp.m_bChinese ? _T("没有可转到目的地址的钱") : _T("There is no money to go to the destination address"));
					if(theApp.m_bChinese)
						::MessageBox(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
					else
						::MessageBoxEx(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
				}else
				{
					strShow.Format(theApp.m_bChinese ? _T("钱包归集命令发送成功,等待1~2分钟后到账") : _T("Wallet return command to send a success, wait 1 to 2 minutes after the account"));
					if(theApp.m_bChinese)
						::MessageBox(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
					else
						::MessageBoxEx(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK,MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
				}

			}

		}else{
			strShow.Format(theApp.m_bChinese ? _T("请选择归集目的地址") : _T("Please select the destination address"));
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
			else
				::MessageBoxEx(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		}
	}

}
