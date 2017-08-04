// PayeeAddrBookDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "PayeeAddrBookDlg.h"
#include "afxdialogex.h"
#include "NewSendAddr.h"


// CPayeeAddrBookDlg �Ի���

IMPLEMENT_DYNAMIC(CPayeeAddrBookDlg, CDialogEx)

CPayeeAddrBookDlg::CPayeeAddrBookDlg(CWnd* pParent /*=NULL*/, CString strTip /*=_T("")*/)
	: CDialogEx(CPayeeAddrBookDlg::IDD, pParent)
{
	m_strTip = strTip ;

	hitRow = -1;
	hitCol = -1;
	m_prehittex = _T("");
}

CPayeeAddrBookDlg::~CPayeeAddrBookDlg()
{
}

void CPayeeAddrBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_PAY_ADDRBOOK, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CPayeeAddrBookDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPayeeAddrBookDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADDADDRBOOK, &CPayeeAddrBookDlg::OnBnClickedButtonAddaddrbook)
	ON_BN_CLICKED(IDC_BUTTON_DELEITEM, &CPayeeAddrBookDlg::OnBnClickedButtonDeleitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_PAY_ADDRBOOK, &CPayeeAddrBookDlg::OnNMDblclkList)
	ON_NOTIFY(NM_CLICK, IDC_LST_PAY_ADDRBOOK, &CPayeeAddrBookDlg::OnNMClickList)
END_MESSAGE_MAP()


// CPayeeAddrBookDlg ��Ϣ�������


BOOL CPayeeAddrBookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDC_BUTTON_ADDADDRBOOK)->SetWindowText("Add address");
		GetDlgItem(IDC_BUTTON_DELEITEM)->SetWindowText("Del address");
		SetWindowText("Select an address");
	}


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[2]  = {
		{theApp.m_bChinese ? "��ǩ" : "Label" ,      200},
		{theApp.m_bChinese ? "��ַ" : "Address",      300}
	};

	m_listCtrl.SetBkColor(RGB(255,255,255));       
	m_listCtrl.SetRowHeigt(23);               
	m_listCtrl.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_listCtrl.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_listCtrl.SetHeaderTextColor(RGB(0,0,0)); 
	m_listCtrl.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <2 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	LoadAddrBook();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

bool CPayeeAddrBookDlg::LoadAddrBook()
{
	theApp.m_SqliteDeal.GetAddressBookList(_T(" 1=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	m_listCtrl.DeleteAllItems();

	int nSubIdx = 0 , i = 0 ;
	string strShowData = _T("");
	std::map<string,uistruct::ADDRBOOK_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		uistruct::ADDRBOOK_t address = const_it->second;

		strShowData=strprintf("%s" ,address.label);
		m_listCtrl.InsertItem(i,address.label.c_str());

		strShowData=strprintf("%s" ,address.address);
		m_listCtrl.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;
		i++;
	}

	return TRUE ;
}

void CPayeeAddrBookDlg::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}


void CPayeeAddrBookDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString Label =m_listCtrl.GetItemText(nRow, 0);
		CString addr =m_listCtrl.GetItemText(nRow, 1);
		m_selectAddr.label = Label;
		m_selectAddr.address = addr;
	}else{
		StrShow.Format(theApp.m_bChinese ? _T("��ѡ���ַ") : _T("Please select an address"));
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, StrShow, theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, StrShow, theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	CDialogEx::OnOK();
}


void CPayeeAddrBookDlg::OnBnClickedButtonAddaddrbook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNewSendAddr addsendaddr;
	string strShowData;
	INT_PTR nResponse = addsendaddr.DoModal();
	if (nResponse == IDOK)
	{
		uistruct::ADDRBOOK_t addr;
		addsendaddr.GetAddrbook(addr);
		int count = m_listCtrl.GetItemCount();
		int nSubIdx = 0;

		m_listCtrl.InsertItem( count ,addr.label.c_str()) ;

		m_listCtrl.SetItemText(count , ++nSubIdx , addr.address.c_str() ) ;

		//// �������ݿ�
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
		string temp =addr.ToJson();
		postmsg.SetData(temp);
		theApp.m_msgQueue.push(postmsg);
	}
}


void CPayeeAddrBookDlg::OnBnClickedButtonDeleitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int res = 0;
		if(theApp.m_bChinese)
			res = ::MessageBox(m_hWnd, _T(theApp.m_bChinese ? "�Ƿ�Ҫɾ�����տ��ַ" : "Whether you want to delete this payment address") , theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_YESNO );
		else
			res = ::MessageBoxEx(m_hWnd, _T(theApp.m_bChinese ? "�Ƿ�Ҫɾ�����տ��ַ" : "Whether you want to delete this payment address") , theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_YESNO , MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

		if ( IDYES == res ){
			int nRow = m_listCtrl.GetNextSelectedItem(pos) ;

			//// ɾ���������ݿ�
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_DELETERBOOK);
			uistruct::ADDRBOOK_t addr;
			CString Leble = m_listCtrl.GetItemText(nRow, 0); 
			CString address = m_listCtrl.GetItemText(nRow, 1); 
			addr.address = address;
			addr.label = Leble;
			string temp =addr.ToJson();
			postmsg.SetData(temp);
			theApp.m_msgQueue.push(postmsg);

			//// �ӿؼ���ɾ��
			m_listCtrl.DeleteItem(nRow);
		}
	}else{
		//StrShow.Format(_T("��ѡ���ַ!\n"));
		StrShow = theApp.m_bChinese ? _T("��ѡ���ַ") : _T("Please select an address");
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, StrShow.c_str() ,theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, StrShow.c_str() ,theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
}

void CPayeeAddrBookDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LVHITTESTINFO info;
	info.pt  =  pNMItemActivate -> ptAction;

	if (m_listCtrl.SubItemHitTest( & info)  !=   - 1  )
	{
		hitRow  =  info.iItem;
		hitCol  =  info.iSubItem;
		if ( hitCol == 1)
		{
			int nRow = pNMItemActivate->iItem;
			CString Label =m_listCtrl.GetItemText(nRow, 0);
			CString addr =m_listCtrl.GetItemText(nRow, 1);
			m_selectAddr.label = Label;
			m_selectAddr.address = addr;
			CDialogEx::OnOK();
			*pResult = 0;
			return;
		}
		if (editItem.m_hWnd  ==  NULL ) // editItemΪһ�����ؼ��� 
		{
			RECT rect;
			rect.left  =   0 ;
			rect.top  =   0 ;
			rect.bottom  =   15 ;
			rect.right  =   200 ;
			editItem .Create(WS_CHILD  |  ES_LEFT  |  WS_BORDER  |  ES_AUTOHSCROLL  |  ES_WANTRETURN  |  ES_MULTILINE, rect,  this ,  101 );
			editItem.SetFont( this -> GetFont(), FALSE);
		}
		/// �����޸�֮ǰ���ַ���
		editItem.GetWindowText(m_prehittex);
		CRect rect;
		m_listCtrl.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
		rect.top  +=   40 ;
		rect.left  +=   2 ;
		rect.right  -=   556 ;
		rect.bottom  +=   40 ;

		editItem.SetWindowText(m_listCtrl.GetItemText( info .iItem,  info .iSubItem));
		editItem.MoveWindow( & rect, TRUE);
		editItem.ShowWindow( 1 );
		editItem.SetSel(-1);
		editItem.SetFocus();
	}
	*pResult = 0;
}


void CPayeeAddrBookDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (editItem.m_hWnd  !=  NULL)
	{
		if( hitCol!= 0)
		{
			hitCol  =  hitRow  =   - 1 ;
			*pResult = 0;
		}
		editItem.ShowWindow( 0 );
		if (hitRow  !=   - 1 )
		{
			CString text;
			editItem.GetWindowText(text);
			m_listCtrl.SetItemText(hitRow, hitCol, text);
			if (text != m_prehittex)
			{
				CString addr = _T("");
				addr = m_listCtrl.GetItemText(hitRow,1);
				string strSourceData  , strW ;
				strSourceData = strprintf("Label = '%s'" , text  ) ;
				strW= strprintf("address = '%s'" , addr ) ;

				uistruct::DATABASEINFO_t DatabaseInfo;
				DatabaseInfo.strSource = strSourceData;
				DatabaseInfo.strWhere = strW ;
				DatabaseInfo.strTabName = _T("t_address_book");
				CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
				string strtemp = DatabaseInfo.ToJson();

				postmsg.SetData(strtemp.c_str());
				theApp.m_msgQueue.push(postmsg);
			}
		}
	}
	hitCol  =  hitRow  =   - 1 ;
	*pResult = 0;

}
