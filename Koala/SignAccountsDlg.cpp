// SignAccountsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"
#include "KoalaDlg.h"

// CSignAccountsDlg 对话框

IMPLEMENT_DYNAMIC(CSignAccountsDlg, CDialogEx)

CSignAccountsDlg::CSignAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignAccountsDlg::IDD, pParent)
{
	
}
CSignAccountsDlg::~CSignAccountsDlg()
{

}

void CSignAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SA_SEND, m_rBtnSend);
	//DDX_Control(pDX, IDC_ST_ADDR, m_addr);
	DDX_Control(pDX, IDC_FEE, m_fee);
	DDX_Control(pDX, IDC_COMBO2, m_cboUnit);
	DDX_Control(pDX, IDC_LIST_ADDRACT, m_lstAddrAct);
}


BEGIN_MESSAGE_MAP(CSignAccountsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SA_SEND, &CSignAccountsDlg::OnBnClickedButtonSend)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ADDRACT, &CSignAccountsDlg::OnItemchangedListAddract)
END_MESSAGE_MAP()



void CSignAccountsDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}

	int nCount = m_lstAddrAct.GetItemCount();
	int nNum = 0;

	for(int i = 0; i < nCount; i++)
	{
		if(m_lstAddrAct.GetCheck(i))
		{
			//CString address = theApp.m_verStrAddress[i];
			CString address = m_lstAddrAct.GetItemText(i, 0);
			if ( _T("") != address ) {
				string strCommand , strShowData;CString strFee ;

				Json::Reader reader;  
				Json::Value root; 
				//if (!reader.parse(strShowData.GetString(), root)) 
				//	return  ;

				GetDlgItem(IDC_EDIT_FEE)->GetWindowText(strFee);
				strCommand = strprintf("%s %s %lld","registaccounttx" ,address  , (INT64)REAL_MONEY(strtod(strFee,NULL)) );

				if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
				{
					int res = 0;
					if(theApp.m_bChinese)
						res = ::MessageBox(m_hWnd,theApp.m_bChinese ?  _T("地址激活失败,可能之前有地址已发送激活命令,请耐心等待1~2分钟") : _T("Address activation failed, may have been sent before the activation command, please be patient 1 to 2 minutes"),
						theApp.m_bChinese ? _T("提示") : _T("tip"), MB_OK );
					else
						res = ::MessageBoxEx(m_hWnd,theApp.m_bChinese ?  _T("地址激活失败,可能之前有地址已发送激活命令,请耐心等待1~2分钟") : _T("Address activation failed, may have been sent before the activation command, please be patient 1 to 2 minutes"),
						theApp.m_bChinese ? _T("提示") : _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

					if ( IDOK == res){
						EndDialog(IDOK);
					}
					
					TRACE("OnBnClickedButtonSend rpccmd registaccounttx error");
					return;
				}

				strShowData = root.toStyledString();
				string strData;
				int pos = strShowData.find("hash");

				if ( pos >=0 ) {
					//插入到数据库
					string strHash;
					strHash =root["hash"].asString();
					string strCond;
					strCond = strprintf(" hash = '%s' ", (LPSTR)(LPCTSTR)strHash.c_str());
					int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond) ;

					if ( 0 == nItem ) {

						CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
						postmsg.SetData(strHash);
						theApp.m_msgQueue.push(postmsg);

					}
				}

			}
			++nNum;
		}
	}
	int res2 = 0;

	if(nNum == 0)
	{
		if(theApp.m_bChinese)
			res2 = ::MessageBox(m_hWnd, theApp.m_bChinese ? _T("没有可激活的地址") : _T("There is no address that can be activated"), theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OK );
		else
			res2 = ::MessageBoxEx(m_hWnd, theApp.m_bChinese ? _T("没有可激活的地址") : _T("There is no address that can be activated"), theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

		if ( IDOK == res2 ){
			EndDialog(IDOK);
		}
	}
	else
	{	
		if(theApp.m_bChinese)
			res2 = ::MessageBox(m_hWnd, theApp.m_bChinese ? _T("激活交易发送成功，请等待1-2分钟确认激活交易") : _T("Activate the transaction to send successfully, please wait 1-2 minutes to confirm the activation of the transaction"), 
				theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OK );
		else
			res2 = ::MessageBoxEx(m_hWnd, theApp.m_bChinese ? _T("激活交易发送成功，请等待1-2分钟确认激活交易") : _T("Activate the transaction to send successfully, please wait 1-2 minutes to confirm the activation of the transaction"), 
				theApp.m_bChinese ? _T("提示") : _T("tip") , MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		if(IDOK == res2){
			EndDialog(IDOK);
		}
		
	}

}
void CSignAccountsDlg::SetShowAddr(vector<AMADDR> &refaddr)
{
	m_lstAddrAct.DeleteAllItems();
	string strShowData;

	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
	//GetDlgItem(IDC_EDIT_SA_ADDRESS)->SetWindowText(addr);
	for(int i = 0; i < refaddr.size(); i++)
	{
		m_lstAddrAct.InsertItem(i, refaddr[i].strAddr);
		//m_lstAddrAct.InsertItem(i, )
		strShowData = strprintf("%.2f" , refaddr[i].fMoney ) ;
		m_lstAddrAct.SetItemText(i , 1 , strShowData.c_str() ) ;
	}

}

BOOL CSignAccountsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_FEE)->SetWindowText("Fee:");
		GetDlgItem(IDC_BUTTON_SA_SEND)->SetWindowText("Active");
		SetWindowText("Active address");
	}

	CenterWindow(GetDlgItem(IDC_LIST_ADDRACT));

	/*int nWidht = GetSystemMetrics(SM_CXVSCROLL);*/
	CenterWindow();

	CRect rect;
	GetDlgItem(IDC_LIST_ADDRACT)->GetWindowRect(&rect);

	int i = rect.right - rect.left;

	m_fontGrid.CreatePointFont(100,_T("新宋体"));
	m_lstAddrAct.SetExtendedStyle( LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT );
	/*m_lstAddrAct.ShowScrollBar(SB_VERT, TRUE);
	m_lstAddrAct.GetHeaderCtrl()->EnableWindow(FALSE);*/

	m_lstAddrAct.InsertColumn(0, theApp.m_bChinese ? "激活地址" : "Active Address", LVCFMT_CENTER, 250);
	m_lstAddrAct.InsertColumn(1, theApp.m_bChinese ? "余额" : "Balance", LVCFMT_CENTER, 74);

	GetDlgItem(IDC_EDIT_FEE)->SetWindowText("0.0001");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString("klc");
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);



	SetShowAddr( theApp.m_verAMAddr);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CSignAccountsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SelectObject(&m_fontGrid);
	//	hbr = (HBRUSH)CreateSolidBrush(RGB(240,240,240));
	//}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}





void CSignAccountsDlg::OnItemchangedListAddract(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */ 
		) 
	{ 
		CKoalaDlg *pMainDlg = (CKoalaDlg*)AfxGetApp()->GetMainWnd();
		string strAddr = m_lstAddrAct.GetItemText(pNMLV->iItem, 0);
		CString strShow;
		uistruct::LISTADDR_t te =  pMainDlg->m_pAddrListDlg->m_MapAddrInfo[strAddr];
		if (te.fMoney <=0)
		{
			strShow.Format(theApp.m_bChinese ? _T("账户余额为零,不能激活!") : "The account balance is zero and can not be activated!");

			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
			else
				::MessageBoxEx(m_hWnd, strShow ,theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			m_lstAddrAct.SetCheck(pNMLV->iItem, FALSE);

			return;
		}
		if(te.bSign) 
		{
			strShow.Format(theApp.m_bChinese ? _T("账户已激活!") : _T("Account activated!"));
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, strShow, theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
			else
				::MessageBoxEx(m_hWnd, strShow, theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			m_lstAddrAct.SetCheck(pNMLV->iItem, FALSE);

			return;
		}
	} 
	else if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */ 
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */ 
		) 
	{ 
		//TRACE("Item %d is unchecked\n", pNMLV->iItem); 
	} 
	else 
	{ 
		//TRACE("Item %d does't change the check-status\n", pNMLV->iItem); 
	} 

	*pResult = 0;
}
