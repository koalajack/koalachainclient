// SendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "SendDlg.h"
#include "afxdialogex.h"
#include "AddrBookDlg.h"
#include "PayeeAddrBookDlg.h"

// CSendDlg 对话框

IMPLEMENT_DYNAMIC(CSendDlg, CDialogEx)

CSendDlg::CSendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendDlg::IDD, pParent)
	, m_strAddrFrom(_T(""))
	, m_strAddrTo(_T(""))
	, m_strLabel(_T(""))
	, m_fMoney(0.0f)
	, m_strMessage(_T(""))
{

}

CSendDlg::~CSendDlg()
{
}

void CSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITFROM, m_edtFrom);
	DDX_Text(pDX, IDC_EDITFROM, m_strAddrFrom);
	DDX_Text(pDX, IDC_EDITTO, m_strAddrTo);
	DDX_Text(pDX, IDC_EDITLABEL, m_strLabel);
	DDX_Text(pDX, IDC_EDITSUM, m_fMoney);
	DDX_Text(pDX, IDC_MESSAGE, m_strMessage);
	DDX_Control(pDX, IDC_EDITTO, m_edtTo);
	DDX_Control(pDX, IDC_EDITLABEL, m_edtLabel);
}


BEGIN_MESSAGE_MAP(CSendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBTN_SEND, &CSendDlg::OnBnClickedMfcbtnSend)
	ON_BN_CLICKED(IDC_MFCBTN_FROM, &CSendDlg::OnBnClickedMfcbtnFrom)
	ON_BN_CLICKED(IDC_MFCBTN_TO, &CSendDlg::OnBnClickedMfcbtnTo)
	ON_MESSAGE(MSG_USER_SEND_UI , &CSendDlg::OnShowListaddrData  )
END_MESSAGE_MAP()


// CSendDlg 消息处理程序


BOOL CSendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		//GetDlgItem(IDC_STATIC444)->SetWindowText("From:");
		//GetDlgItem(IDC_STATIC445)->SetWindowText("Pay to:");
		//GetDlgItem(IDC_STATIC446)->SetWindowText("Label:");
		//GetDlgItem(IDC_STATIC447)->SetWindowText("Amount:");
		//GetDlgItem(IDC_MFCBTN_SEND)->SetWindowText("Send");
	}


	// TODO:  在此添加额外的初始化
	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_SEND_UI ) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSendDlg::OnBnClickedMfcbtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}

	UpdateData(TRUE);
	
	if(m_strAddrFrom.IsEmpty())
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("发送地址不能为空") ,  _T("提示"),MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Send address can not be empty") ,  _T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_strAddrTo.IsEmpty())
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("接收地址不能为空") ,  _T("提示"),MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The receiving address can not be empty") ,  _T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(!strcmp(m_strAddrFrom.GetString(), m_strAddrTo.GetString()))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("发送地址和目的地址不能相同") ,  _T("提示"),MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The sending and destination addresses can not be the same") ,  _T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fMoney == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("发送金额不能为0") ,_T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The sending amount can not be zero") ,_T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fMoney > m_curAddrMoney ) 
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("账户余额不足") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Account balance is insufficient") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}


	if (!theApp.m_bIsSyncBlock )
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("同步未完成,不能发送交易") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Synchronization is not completed and can not be sent") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}


	string strCommand = strprintf("%s %s %s %lld","sendtoaddress" ,m_strAddrFrom.GetString() ,m_strAddrTo.GetString() ,(ULONGLONG) (m_fMoney * 1.0 *COIN));
	string strShowData ;

	CString strDisplay;
	strDisplay.Format(_T("%s%.4lfklc%s%s"),theApp.m_bChinese ? _T("转账") : _T("Transfer"), m_fMoney, theApp.m_bChinese ? _T("至") : _T("to") ,m_strAddrTo.GetString());

	int res = 0;
	if(theApp.m_bChinese)
		res = ::MessageBox(m_hWnd, strDisplay ,theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OKCANCEL );
	else
		res = ::MessageBoxEx(m_hWnd, strDisplay ,theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OKCANCEL, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	if (IDCANCEL == res )
	{
		return;
	}

	strShowData = _T("");
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("OnBnClickedSendtrnsfer rpccmd sendtoaddress error");
		return;
	}
	BOOL bRes = FALSE ;
	strShowData = root.toStyledString();
	int pos = strShowData.find("hash");
	if ( pos >=0 ) {
		//插入到数据库
		string strHash = root["hash"].asString() ;
		string strCond;
		strCond = strprintf(" hash='%s' ", strHash.c_str());
		int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond) ;

		if ( 0 == nItem ) {

			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
			postmsg.SetData(strHash);
			theApp.m_msgQueue.push(postmsg);
		}
	}

	string strData;
	if ( pos >=0 ) {
		strData = strprintf(" %s\n\n %s:%s\n\n %s:%s\n\n %s：%.4lf\n  hash:%s",theApp.m_bChinese ? _T("转账交易发送成功") : _T("The transfer transaction was sent successfully"),
			theApp.m_bChinese ? _T("源地址") : _T("Source Address"), m_strAddrFrom.GetString(),
			theApp.m_bChinese ? _T("目的地址") : _T("Destination Address"),m_strAddrTo.GetString(),
			theApp.m_bChinese ? _T("金额") : _T("Amount"), m_fMoney,root["hash"].asCString()) ;
	}else{
		strData = theApp.m_bChinese ? _T("转账失败!") : _T("Transfer failed!");
	}

	if(theApp.m_bChinese)
		::MessageBox(m_hWnd, strData.c_str() , _T("提示") ,MB_OK );
	else
		::MessageBoxEx(m_hWnd, strData.c_str() , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	//// 插入数据库,将收款人添加到地址簿
	CString label;
	GetDlgItem(IDC_EDITLABEL)->GetWindowTextA(label);
	CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
	uistruct::ADDRBOOK_t addr;
	addr.address = m_strAddrTo.GetBuffer(0);
	addr.label = m_strLabel;
	string temp = addr.ToJson();
	postmsg.SetData(temp);
	theApp.m_msgQueue.push(postmsg);
	
}


void CSendDlg::OnBnClickedMfcbtnFrom()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddrBookDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		m_strAddrFrom.Format("%s", dlg.m_strCurSelectAddr.c_str());
		m_curAddrMoney = dlg.m_fCurMoney;
		//UpdateData(FALSE);
		m_edtFrom.SetWindowText(m_strAddrFrom);
	}
}


void CSendDlg::OnBnClickedMfcbtnTo()
{
	// TODO: 在此添加控件通知处理程序代码
	CPayeeAddrBookDlg addrbook;
	if(IDOK == addrbook.DoModal())
	{
		uistruct::ADDRBOOK_t addr;
		addrbook.GetAddrbook(addr);

		m_strAddrTo = addr.address.c_str();
		m_strLabel = addr.label.c_str();
		//UpdateData(FALSE);
		m_edtTo.SetWindowText(m_strAddrTo);
		m_edtLabel.SetWindowText(m_strLabel);
	}
}

BOOL CSendDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message==WM_KEYDOWN)
	{
		BOOL bCtrl=::GetKeyState(VK_CONTROL)&0x8000;
		BOOL bShift=::GetKeyState(VK_SHIFT)&0x8000;

		// only gets here if CTRL key is pressed
		BOOL bAlt=::GetKeyState(VK_MENU)&0x8000;

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

		switch( pMsg->wParam )
		{
		case 'V':
			{
				if (GetDlgItem(IDC_EDITTO) == this->GetFocus())
				{
					HWND hWnd = GetSafeHwnd(); // 获取安全窗口句柄
					::OpenClipboard(hWnd); // 打开剪贴板
					HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);// 获取剪贴板数据句柄
					DWORD dwLength = GlobalSize(hClipMemory); // 返回指定内存区域的当前大小
					LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); // 锁定内存
					CString message = CString(lpClipMemory); // 保存得到的文本数据
					GlobalUnlock(hClipMemory); // 内存解锁
					::CloseClipboard(); // 关闭剪贴板
					if (bCtrl)
					{
						uistruct::ADDRBOOK_t addrBook;
						string strCond;
						strCond = strprintf(" address='%s' ", message);
						int nItem =  theApp.m_SqliteDeal.GetAddressBookItem( strCond, &addrBook) ;
						if (addrBook.label != _T(""))
						{
							GetDlgItem(IDC_EDITLABEL)->SetWindowText(addrBook.label.c_str());
						}

					}

				}

			}
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CSendDlg::OnShowListaddrData( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{

			break;
		}
		break;
	case WM_UP_NEWADDRESS:
		{
			break;
		}
		break;
	default:
		break;

	}
	return 0 ;
}
