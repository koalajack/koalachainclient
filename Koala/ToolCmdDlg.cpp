// ToolCmdDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ToolCmdDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"


// CToolCmdDlg 对话框

IMPLEMENT_DYNCREATE(CToolCmdDlg, CDialogEx)

CToolCmdDlg::CToolCmdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolCmdDlg::IDD, pParent)
{

}

CToolCmdDlg::~CToolCmdDlg()
{
}

void CToolCmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RPC, m_rpccommand);
	DDX_Control(pDX, IDC_CMDCLEAR_BTN, m_rpcBtnClear);
	DDX_Control(pDX, IDC_COMBO_INPUT, m_comboxinput);
	//  DDX_Control(pDX, IDC_HEAD, m_headText);


}


BEGIN_MESSAGE_MAP(CToolCmdDlg, CDialogEx)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_MESSAGE(WM_SHOW_SEND_DATA,OnShowSendData)
	ON_BN_CLICKED(IDC_CMDCLEAR_BTN, &CToolCmdDlg::OnBnClickedCmdclearBtn)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CToolCmdDlg 消息处理程序


BOOL CToolCmdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC666)->SetWindowText("RPC command");
		GetDlgItem(IDC_CMDCLEAR_BTN)->SetWindowText("Clear");
		SetWindowText("Set up admin account");
	}

	// TODO:  在此添加额外的初始化

	CRect rcWindow;
	theApp.m_pMainWnd->GetWindowRect(&rcWindow);
	CRect rc;
	GetWindowRect(&rc);
	MoveWindow(rcWindow.right,rcWindow.top-20,rc.Width(),rc.Height());
	// TODO:  在此添加额外的初始化

	//GetDlgItem(IDC_CMDARROW)->SetWindowText(UiFun::UI_LoadString("RPC" , "RPC_INPUT" ,1));
	CRect rcClient;
	GetClientRect(&rcClient);

	AutoCombo.Init(&m_comboxinput);

	CStringA configpath = "";
	configpath.AppendFormat("%s",theApp.m_strInsPath.c_str());
	configpath.AppendFormat("\\%s","koalachainclient.conf");

	if (PathFileExistsA(configpath))
	{
		CJsonConfigHelp::getInstance()->AddItemString(configpath,AutoCombo);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CToolCmdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		//if (pMsg->hwnd == GetDlgItem(IDC_COMBO_INPUT)->GetSafeHwnd())
		if(m_comboxinput.IsChild(GetFocus()))
		{
			CString strCommand;
			GetDlgItem(IDC_COMBO_INPUT)->GetWindowText(strCommand);
			GetDlgItem(IDC_COMBO_INPUT)->SetWindowText(_T(""));

			string strcom =strprintf("%s",strCommand);
			string strSendData,strretshow;
			RPCCommandToJson(strcom,strSendData);
			CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);
			ShowJson(strcom,strretshow);
			return TRUE;
		}
		else
		{
			switch(pMsg->wParam) 
			{  

			case VK_RETURN: //回车   
				return TRUE;  

			}  
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CToolCmdDlg::OnBnClickedCmdclearBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowText(_T(""));
	m_rpccommand.DeleteAllItems();
}

LRESULT CToolCmdDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("RecvData:"));
	return TRUE;
}
LRESULT CToolCmdDlg::OnShowSendData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("SendData:"));
	return TRUE;
}
void CToolCmdDlg::UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord)
{
	/*CString strShow;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	pEdit->GetWindowText(strShow);
	if(strShow.GetLength()> 1024*10)
	strShow.Empty();
	strShow += _T("\r\n");
	strShow += strKeyWord;
	strShow += _T("\r\n");
	strShow += CString(strNewData.GetString());
	pEdit->SetWindowText(strShow);

	int nLines = pEdit->GetLineCount();
	pEdit->LineScroll(nLines);*/
}


void CToolCmdDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		//const int div = 100 ;
		//CRect rc ;
		//GetClientRect( rc ) ;
		//CWnd *pst = GetDlgItem( IDC_TREE_RPC ) ;
		//if ( NULL != pst ) {
		//	pst->SetWindowPos( NULL , 0 , 0 , rc.Width(), 90*rc.Height()/div ,SWP_SHOWWINDOW ) ;
		//}
		//pst = GetDlgItem( IDC_STATIC_INPUT ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL , 2 , 92*rc.Height()/div , rect.Width(), rect.Height() ,SWP_SHOWWINDOW ) ;
		//}
		//pst = GetDlgItem( IDC_COMBO_INPUT ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL , (14*rc.Width()/div)+5  , 92*rc.Height()/div - 2 , 71*rc.Width()/div, 65*rc.Width()/div ,SWP_SHOWWINDOW ) ;
		//}
		//pst = GetDlgItem( IDC_BTN_CLEAR ) ;
		//if ( NULL != pst ) {
		//	CRect rect ;
		//	pst->GetClientRect( rect ) ;
		//	pst->SetWindowPos( NULL , 88*rc.Width()/div  , 92*rc.Height()/div - 5 , rect.Width(), rect.Height() ,SWP_SHOWWINDOW ) ;
		//}
	}
}

void CToolCmdDlg::ShowJson(string rpcname,string jsomstr){
	if (jsomstr == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(jsomstr, root)) 
		return ;
	m_rpccommand.Expand(m_rpccommand.GetRootItem(),TVE_COLLAPSE);
	HTREEITEM hItemRoot = m_rpccommand.InsertItem(rpcname.c_str(),TVI_ROOT);
	CSoyPayHelp::getInstance()->Jiexi(root,hItemRoot,m_rpccommand);
	m_rpccommand.Expand(hItemRoot,TVE_EXPAND);
}

