
// KoalaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "KoalaDlg.h"
#include "afxdialogex.h"

#include "OpenUriDlg.h"
#include "MsgSigDlg.h"
#include "WalletEncDlg.h"
#include "ModifyPasswordDlg.h"
#include "ToolDlg.h"
#include "CmdHelpDlg.h"
#include "VersionDlg.h"
#include "SetAdminDlg.h"
#include <sstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TOP_BTN_HEIGHT 32


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKoalaDlg 对话框

CKoalaDlg::CKoalaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKoalaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
	m_pOverviewDlg = NULL;
	m_pSendDlg = NULL;
	m_pReceiveDlg = NULL;
	m_pTradeRecordDlg = NULL;
	m_pCompanyInfoDlg = NULL;
	//m_pEntRegistDlg = NULL;
	m_pAddrListDlg = NULL;
	m_pStatusDlg = NULL;
	m_bEntAcctChecked = FALSE;
	m_pBalloonTip = NULL;
	m_pOutGifDlg = NULL;
	m_bFirstQuit = TRUE;
	m_pCmdHelpDlg = NULL;
	m_pReceiveDlg = NULL;

	m_MsgSigDlg = NULL;
	m_pToolDlg = NULL;
}

CKoalaDlg::~CKoalaDlg()
{
	if(NULL != m_pToolDlg)
	{
		delete m_pToolDlg;
	}
	if(NULL != m_pCmdHelpDlg)
	{
		delete m_pCmdHelpDlg;
	}

	if(NULL != m_pOverviewDlg)
	{
		delete m_pOverviewDlg;
	}
	if(NULL != m_pSendDlg)
	{
		delete m_pSendDlg;
	}
	if(NULL != m_pReceiveDlg)
	{
		delete m_pReceiveDlg;
	}
	if(NULL != m_pTradeRecordDlg)
	{
		delete m_pTradeRecordDlg;
	}
	if(NULL != m_pCompanyInfoDlg)
	{
		delete m_pCompanyInfoDlg;
	}

}

void CKoalaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_RECE, m_btnRece);
	DDX_Control(pDX, IDC_BTN_SEND, m_btnSend);
	DDX_Control(pDX, IDC_BTN_PROFILE, m_btnOverview);
	DDX_Control(pDX, IDC_BTN_RECORD, m_btnRecord);
	DDX_Control(pDX, IDC_BTN_SELETEADDR, m_btnAddrSelect);
	DDX_Control(pDX, IDC_BTN_COMPANY_INFO, m_btnCompanyInfo);
}

BEGIN_MESSAGE_MAP(CKoalaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_INITMENUPOPUP()
	ON_BN_CLICKED(IDC_BTN_PROFILE, &CKoalaDlg::OnBnClickedBtnProfile)
	ON_BN_CLICKED(IDC_BTN_SEND, &CKoalaDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_RECE, &CKoalaDlg::OnBnClickedBtnRece)
	ON_BN_CLICKED(IDC_BTN_RECORD, &CKoalaDlg::OnBnClickedBtnRecord)
	ON_BN_CLICKED(IDC_BTN_COMPANY_INFO, &CKoalaDlg::OnBnClickedBtnCompanyInfo)
	ON_COMMAND(ID_MENU_OPENURI, &CKoalaDlg::OnMenuOpenuri)
	ON_COMMAND(ID_MENU_BACKUP, &CKoalaDlg::OnMenuBackup)
//	ON_COMMAND(ID_MENU_SIGN, &CKoalaDlg::OnMenuSign)
//	ON_COMMAND(ID_MENU_VERIFY, &CKoalaDlg::OnMenuVerify)
	ON_COMMAND(ID_MENU_EXPORT, &CKoalaDlg::OnMenuExport)
	ON_COMMAND(ID_MENU_IMPORT, &CKoalaDlg::OnMenuImport)
	ON_COMMAND(ID_MENU_EXIT, &CKoalaDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_SET_ACCOUNT, &CKoalaDlg::OnMenuSetAccount)
	ON_COMMAND(ID_MENU_CHANGE_ACCOUNT, &CKoalaDlg::OnMenuChangeAccount)
	ON_COMMAND(ID_MENU_ENCRYPT, &CKoalaDlg::OnMenuEncrypt)
	ON_COMMAND(ID_MENU_CHANGE_PASSWORD, &CKoalaDlg::OnMenuChangePassword)
	ON_COMMAND(ID_MENU_INFO, &CKoalaDlg::OnMenuInfo)
	ON_COMMAND(ID_MENU_PEERLIST, &CKoalaDlg::OnMenuPeerlist)
	ON_COMMAND(ID_MENU_CMD, &CKoalaDlg::OnMenuCmd)
	ON_COMMAND(ID_MENU_CMD_HELP, &CKoalaDlg::OnMenuCmdHelp)
	ON_COMMAND(ID_MENU_VERSION, &CKoalaDlg::OnMenuVersion)
	ON_UPDATE_COMMAND_UI(ID_MENU_CHANGE_ACCOUNT, &CKoalaDlg::OnUpdateMenuChangeAccount)
	ON_BN_CLICKED(IDC_BTN_SELETEADDR, &CKoalaDlg::OnBnClickedBtnSeleteaddr)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_MENU_LOCK, &CKoalaDlg::OnMenuLock)
	ON_COMMAND(ID_MENU_CHINESE, &CKoalaDlg::OnMenuChinese)
	ON_UPDATE_COMMAND_UI(ID_MENU_CHINESE, &CKoalaDlg::OnUpdateMenuChinese)
	ON_COMMAND(ID_MENU_ENGLISH, &CKoalaDlg::OnMenuEnglish)
	ON_UPDATE_COMMAND_UI(ID_MENU_ENGLISH, &CKoalaDlg::OnUpdateMenuEnglish)
END_MESSAGE_MAP()


// CKoalaDlg 消息处理程序

void    CKoalaDlg::SyncAddrInfo()
{
	string strCommand;
	strCommand = strprintf("%s","listaddr");
	string strShowData ="";

	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("SyncAddrInfo rpccmd listaddr error");
		return;
	}

	map<string,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));

	/// 数据库中没有的地址要插入
	map<string,int> SListInfo;

	for(unsigned int i = 0; i < root.size(); ++i){
		//address
		uistruct::LISTADDR_t listaddr;
		string address = root[i]["addr"].asString();	
		SListInfo[address] = i;

		//memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address

		listaddr.address = root[i]["addr"].asString(); ;
		//RegID

		listaddr.RegID = root[i]["regid"].asString() ;
		//金额
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//是否支持冷挖矿
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//是否注册GetLength();
		if ( 3 <= listaddr.RegID.length() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}

		string strCond;
		strCond=strprintf(" address = '%s' ", listaddr.address.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			string strData;
			strData= strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , listaddr.address.c_str() ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label.c_str() ) ;
			if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strData ))
			{
				TRACE("INSERT t_wallet_address data failed!\n");
			}
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				string strSourceData,strWhere;
				strSourceData =strprintf("reg_id = '%s', money = %.8f ,cold_dig =%d, sign =%d" ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere=strprintf("address = '%s'" , listaddr.address.c_str()  ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE("UPDATE t_wallet_address data failed!\n");
				}
			}
		}
	}

	////// 剔除数据库中钱包没有的地址
	map<string,uistruct::LISTADDR_t>::const_iterator it;
	for (it= pListInfo.begin();it != pListInfo.end();it++)
	{
		if (SListInfo.count(it->first) <= 0)
		{
			string strCond;
			strCond = strprintf(" address='%s' ", it->first.c_str());
			int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_wallet_address"), strCond);
		}
	}

}

BOOL CKoalaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_BTN_PROFILE)->SetWindowText("Profile");
		GetDlgItem(IDC_BTN_SEND)->SetWindowText("Send");
		GetDlgItem(IDC_BTN_RECE)->SetWindowText("Receive");
		GetDlgItem(IDC_BTN_RECORD)->SetWindowText("Record");
		GetDlgItem(IDC_BTN_SELETEADDR)->SetWindowText("Address");
		GetDlgItem(IDC_BTN_COMPANY_INFO)->SetWindowText("Bussiness");
	}


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hAccelClose = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCEL_CLOSE));

	if(0 == theApp.m_bIsAdmin)
	{
		if(theApp.m_bChinese)
			SetWindowText("考拉链（企业用户版）");
		else 
			SetWindowText("Koala Chain（Enterprise Edition）");
	}
	else if(1 == theApp.m_bIsAdmin)
	{
		if(theApp.m_bChinese)
			SetWindowText("考拉链（管理员版）");
		else
			SetWindowText("Koala Chain（Administrator）");

	}else if(2 == theApp.m_bIsAdmin)
	{
		if(theApp.m_bChinese)
			SetWindowText("考拉链（个人版）");
		else
			SetWindowText("Koala Chain（Personal Edition）");
	}

	//SetWindowText("1111");

	InitialRpcCmd();
	SyncAddrInfo();

	if ( ((CKoalaApp*)AfxGetApp())->pSplashThread != NULL)
	{ 
		((CKoalaApp*)AfxGetApp())->pSplashThread->PostThreadMessage(WM_QUIT,NULL,NULL);
	}

	m_btnSend.SetFlat();
	m_btnSend.SetIcon(IDI_SEND);
	m_btnRece.SetFlat();
	m_btnRece.SetIcon(IDI_RECV);
	m_btnOverview.SetFlat();
	m_btnOverview.SetIcon(IDI_OVERVIEW);
	m_btnRecord.SetFlat();
	m_btnRecord.SetIcon(IDI_TRADERECORD);
	m_btnAddrSelect.SetFlat();
	m_btnAddrSelect.SetIcon(IDI_ADDRSELECT);
	m_btnCompanyInfo.SetFlat();
	m_btnCompanyInfo.SetIcon(IDI_CMPINFO);
	m_btnOverview.DrawFlatFocus(TRUE);

	

	m_btnOverview.SetFocusState(TRUE);
	if(0 == theApp.m_bIsAdmin)
	{
		GetMenu()->GetSubMenu(1)->RemoveMenu(1, MF_BYPOSITION);
		m_btnCompanyInfo.ShowWindow(SW_HIDE);
	}
	else if(2 == theApp.m_bIsAdmin)
	{
		GetMenu()->GetSubMenu(1)->RemoveMenu(0, MF_BYPOSITION);
		GetMenu()->GetSubMenu(1)->RemoveMenu(0, MF_BYPOSITION);
		m_btnCompanyInfo.ShowWindow(SW_HIDE);
	}
	else if(3 == theApp.m_bIsAdmin)
	{
		GetMenu()->GetSubMenu(1)->RemoveMenu(0, MF_BYPOSITION);
		m_btnCompanyInfo.ShowWindow(SW_HIDE);
	}

	GetClientRect(&m_rcMainDlg);

	if(NULL == m_pAddrListDlg)
	{
		m_pAddrListDlg = new CAddrSelectDlg;
		m_pAddrListDlg->Create(IDD_ADDR_SELECT_DIALOG, this);
		m_pAddrListDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
	}

	if(theApp.m_strCurAdminAddress.length() > 0
		&& m_pAddrListDlg->AddrIsExsit(theApp.m_strCurAdminAddress))
	{
		m_btnCompanyInfo.ShowWindow(SW_SHOW);
	}
	else
	{
		theApp.m_strCurAdminAddress = "";
	}

	if(theApp.m_strCurEntAddress.length() == 0
	  ||  !m_pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
	{
		theApp.m_strCurEntAddress = "";
	}


	if(NULL == m_pCompanyInfoDlg)
	{
		m_pCompanyInfoDlg = new CCompanyInfoDlg;
		m_pCompanyInfoDlg->Create(IDD_COMPANYINFO_DLG, this);
		m_pCompanyInfoDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
		/*
		if(theApp.m_strCurEntAddress == "" && theApp.m_strCurAdminAddress == "")
		{
			m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
			m_btnCompanyInfo.ShowWindow(SW_HIDE);
		}
		*/
	}

	JudgeType();

	if(NULL == m_pOverviewDlg)
	{
		m_pOverviewDlg = new COverviewDlg;
		m_pOverviewDlg->Create(IDD_OVERVIEW_DLG, this);
		m_pOverviewDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
	}


	if(NULL == m_pSendDlg)
	{
		m_pSendDlg = new CSendDlg;
		m_pSendDlg->Create(IDD_SEND_DLG, this);
		m_pSendDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
	}

	if(NULL == m_pReceiveDlg)
	{
		m_pReceiveDlg = new CReceiveDlg;
		m_pReceiveDlg->Create(IDD_RECEIVE_DLG, this);
		m_pReceiveDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
	}

	if(NULL == m_pTradeRecordDlg)
	{
		m_pTradeRecordDlg = new CTradeRecordDlg;
		m_pTradeRecordDlg->Create(IDD_TRADE_RECODE_DLG, this);
		m_pTradeRecordDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - 2 * TOP_BTN_HEIGHT);
	}


	/*
	if(NULL == m_pEntRegistDlg)
	{
		m_pEntRegistDlg = new CEntRigeistDlg;
		m_pEntRegistDlg->Create(IDD_ENTREGIST_DLG, this);
		m_pEntRegistDlg->MoveWindow(0, TOP_BTN_HEIGHT, m_rcMainDlg.Width(), m_rcMainDlg.Height() - TOP_BTN_HEIGHT);
	}
	*/

	ActiveDlg(IDD_OVERVIEW_DLG);

	if(NULL == m_pStatusDlg)
	{
		/*
		m_pStatusDlg = new CStatusDlg;
		m_pStatusDlg->Create(IDD_STATUS_DLG, this);
		m_pStatusDlg->MoveWindow(0, m_rcMainDlg.Height() - 32, m_rcMainDlg.Width(), 32);
		m_pStatusDlg->ShowWindow(SW_SHOW);
		*/

		m_pStatusDlg = new CStatusDlg ;
		m_pStatusDlg->Create(this, IDD_STATUS_DLG,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusDlg->SetWindowPos(NULL , 0 , m_rcMainDlg.Height() - 32  , m_rcMainDlg.Width() ,32 , SWP_SHOWWINDOW) ;
		m_pStatusDlg->ShowWindow(SW_SHOW);
	}

	if( NULL == m_pOutGifDlg ){
		m_pOutGifDlg = new COutGifDlg ;
		m_pOutGifDlg->Create(COutGifDlg::IDD , this) ;
		m_pOutGifDlg->ShowWindow(SW_HIDE) ;
	}



	theApp.m_bIsDlgCreateFinished = TRUE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

int CKoalaDlg::JudgeType()
{
	if(0 == theApp.m_bIsAdmin)
	{
		if(theApp.m_strCurEntAddress.length() != 0 && m_pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
		{
			ShowCompanyInfoBtn(TRUE);
			//m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->ShowWithdrawBtn(TRUE);
			m_pCompanyInfoDlg->ShowRechargeBtn(TRUE);
			m_pCompanyInfoDlg->HideAdminBtns(TRUE);
			theApp.m_nShowType = 2;				//设置
			return 2;
		}

		ShowCompanyInfoBtn(FALSE);
		m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
		theApp.m_nShowType = 1;					//没有设置
		return 1;

	}
	else if(1 == theApp.m_bIsAdmin)
	{
		BOOL bEntAddrExist = FALSE;
		BOOL bAdminAddrExist = FALSE;

		if(theApp.m_strCurEntAddress.length() != 0 && m_pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
		{
			bEntAddrExist = TRUE;
		}

		if(theApp.m_strCurAdminAddress.length() != 0 && m_pAddrListDlg->AddrIsExsit(theApp.m_strCurAdminAddress))
		{
			bAdminAddrExist = TRUE;
		}

		if(!bEntAddrExist && !bAdminAddrExist)
		{
			ShowCompanyInfoBtn(FALSE);
			m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
			theApp.m_nShowType = 3;
			return 3;
		}

		if(bEntAddrExist && bAdminAddrExist)
		{
			ShowCompanyInfoBtn(TRUE);
			//m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->ShowWithdrawBtn(TRUE);
			m_pCompanyInfoDlg->HideAdminBtns(FALSE);
			m_pCompanyInfoDlg->ShowRechargeBtn(TRUE);
			theApp.m_nShowType = 4;
			return 4;
		}

		if(bAdminAddrExist && !bEntAddrExist)
		{
			ShowCompanyInfoBtn(TRUE);
			//m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->ShowWithdrawBtn(FALSE);
			m_pCompanyInfoDlg->HideAdminBtns(FALSE);
			m_pCompanyInfoDlg->ShowRechargeBtn(TRUE);
			theApp.m_nShowType = 5;
			return 5;
		}

		if(!bAdminAddrExist && bEntAddrExist)
		{
			ShowCompanyInfoBtn(TRUE);
			//m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->ShowWithdrawBtn(TRUE);
			m_pCompanyInfoDlg->HideAdminBtns(TRUE);
			m_pCompanyInfoDlg->ShowRechargeBtn(TRUE);
			theApp.m_nShowType = 6;
			return 6;
		}

	}
	else if(2 == theApp.m_bIsAdmin)
	{
		ShowCompanyInfoBtn(FALSE);
		m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
		theApp.m_nShowType = 3;
		return 3;
	}
	else if(3 == theApp.m_bIsAdmin)
	{
		if(theApp.m_strCurAdminAddress.length() != 0 && m_pAddrListDlg->AddrIsExsit(theApp.m_strCurAdminAddress))
		{
			ShowCompanyInfoBtn(TRUE);
			//m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->ShowWithdrawBtn(FALSE);
			m_pCompanyInfoDlg->HideAdminBtns(FALSE);
			m_pCompanyInfoDlg->ShowRechargeBtn(TRUE);
			theApp.m_nShowType = 5;
			return 5; 
		}

		ShowCompanyInfoBtn(FALSE);
		m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
		theApp.m_nShowType = 3;
		return 3;
	}

	return 0;
}

void CKoalaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CKoalaDlg::InitialRpcCmd()
{
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severIp,theApp.m_sendPreHeadstr,theApp.m_sendEndHeadstr,theApp.m_rpcPort);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKoalaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKoalaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void  CKoalaDlg::ActiveDlg(UINT nDlgID)
{
	m_pOverviewDlg->ShowWindow(SW_HIDE);
	m_pSendDlg->ShowWindow(SW_HIDE);
	m_pReceiveDlg->ShowWindow(SW_HIDE);
	m_pTradeRecordDlg->ShowWindow(SW_HIDE);
	m_pAddrListDlg->ShowWindow(SW_HIDE);
	m_pCompanyInfoDlg->ShowWindow(SW_HIDE);
	//m_pEntRegistDlg->ShowWindow(SW_HIDE);
	

	switch(nDlgID)
	{
	case IDD_OVERVIEW_DLG:
		{
			m_btnOverview.DrawFlatFocus(TRUE);
			m_pOverviewDlg->ShowWindow(SW_SHOW);
			break;
		}
		
	case IDD_SEND_DLG:
		{
			m_btnSend.DrawFlatFocus(TRUE);
			m_pSendDlg->ShowWindow(SW_SHOW);
			break;
		}
	case IDD_RECEIVE_DLG:
		{
			m_btnRece.DrawFlatFocus(TRUE);
			m_pReceiveDlg->ShowWindow(SW_SHOW);
			break;
		}
	case IDD_TRADE_RECODE_DLG:
		{
			m_btnRecord.DrawFlatFocus(TRUE);
			m_pTradeRecordDlg->ShowWindow(SW_SHOW);
			break;
		}
		
	case IDD_ADDR_SELECT_DIALOG:
		{
			m_btnAddrSelect.DrawFlatFocus(TRUE);
			m_pAddrListDlg->ShowWindow(SW_SHOW);
			break;
		}
	case IDD_COMPANYINFO_DLG:
		{
			m_btnCompanyInfo.DrawFlatFocus(TRUE);
			m_pCompanyInfoDlg->ShowWindow(SW_SHOW);
			m_pCompanyInfoDlg->RefreshListCtrl();
			break;
		}
	/*
	case IDD_ENTREGIST_DLG:
		{
			m_btnCompanyRegister.DrawFlatFocus(TRUE);
			m_pEntRegistDlg->ShowWindow(SW_SHOW);
			break;
		}
		*/
	}
}




void CKoalaDlg::OnBnClickedBtnProfile()
{
	// TODO: 在此添加控件通知处理程序代码
	
	ActiveDlg(IDD_OVERVIEW_DLG);
	m_btnOverview.SetFocusState(TRUE);
	m_btnSend.SetFocusState(FALSE);
	m_btnRece.SetFocusState(FALSE);
	m_btnRecord.SetFocusState(FALSE);
	m_btnAddrSelect.SetFocusState(FALSE);
	m_btnCompanyInfo.SetFocusState(FALSE);
	
}


void CKoalaDlg::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_SEND_DLG);
	m_btnOverview.SetFocusState(FALSE);
	m_btnSend.SetFocusState(TRUE);
	m_btnRece.SetFocusState(FALSE);
	m_btnRecord.SetFocusState(FALSE);
	m_btnAddrSelect.SetFocusState(FALSE);
	m_btnCompanyInfo.SetFocusState(FALSE);
	
}


void CKoalaDlg::OnBnClickedBtnRece()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_RECEIVE_DLG);
	m_btnOverview.SetFocusState(FALSE);
	m_btnSend.SetFocusState(FALSE);
	m_btnRece.SetFocusState(TRUE);
	m_btnRecord.SetFocusState(FALSE);
	m_btnAddrSelect.SetFocusState(FALSE);
	m_btnCompanyInfo.SetFocusState(FALSE);

}


void CKoalaDlg::OnBnClickedBtnRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_TRADE_RECODE_DLG);
	m_btnOverview.SetFocusState(FALSE);
	m_btnSend.SetFocusState(FALSE);
	m_btnRece.SetFocusState(FALSE);
	m_btnRecord.SetFocusState(TRUE);
	m_btnAddrSelect.SetFocusState(FALSE);
	m_btnCompanyInfo.SetFocusState(FALSE);
	
}


void CKoalaDlg::OnBnClickedBtnCompanyInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_COMPANYINFO_DLG);
	m_btnOverview.SetFocusState(FALSE);
	m_btnSend.SetFocusState(FALSE);
	m_btnRece.SetFocusState(FALSE);
	m_btnRecord.SetFocusState(FALSE);
	m_btnAddrSelect.SetFocusState(FALSE);
	m_btnCompanyInfo.SetFocusState(TRUE);
	
}


void CKoalaDlg::OnBnClickedBtnSeleteaddr()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_ADDR_SELECT_DIALOG);
	m_btnOverview.SetFocusState(FALSE);
	m_btnSend.SetFocusState(FALSE);
	m_btnRece.SetFocusState(FALSE);
	m_btnRecord.SetFocusState(FALSE);
	m_btnAddrSelect.SetFocusState(TRUE);
	m_btnCompanyInfo.SetFocusState(FALSE);
}


void CKoalaDlg::OnBnClickedBtnAddrlist()
{
	// TODO: 在此添加控件通知处理程序代码
	ActiveDlg(IDD_ADDR_SELECT_DIALOG);
	
}

void CKoalaDlg::ShowCompanyInfoBtn(BOOL bShow)
{
	if(bShow)
		m_btnCompanyInfo.ShowWindow(SW_SHOW);
	else
		m_btnCompanyInfo.ShowWindow(SW_HIDE);
}


void CKoalaDlg::OnMenuOpenuri()
{
	// TODO: 在此添加命令处理程序代码
	COpenUriDlg dlg;

	if(dlg.DoModal()==IDOK)
	{
		m_pSendDlg->m_strAddrTo = dlg.m_strAddr.c_str();
		//m_pSendDlg->m_curAddrMoney = dlg.m_account.strtoul();
		m_pSendDlg->m_strLabel = dlg.m_strLabel.c_str();
		// = dlg.m_account.;
		stringstream strValue;
		strValue << dlg.m_account;

		strValue >>  m_pSendDlg->m_fMoney;


		m_pSendDlg->m_strMessage = theApp.m_bChinese ? _T("消息：") : _T("Message：");
		m_pSendDlg->m_strMessage +=	dlg.m_strMsg.c_str();

		m_pSendDlg->UpdateData(FALSE);
	}

	OnBnClickedBtnSend();

}


void CKoalaDlg::OnMenuBackup()
{
	// TODO: 在此添加命令处理程序代码
	/*CString strAppName;
	::GetModuleFileName(NULL, strAppName.GetBuffer(_MAX_PATH),_MAX_PATH);
	strAppName.ReleaseBuffer();

	int nPos = strAppName.ReverseFind('\\');
	strAppName = strAppName.Left(nPos+1);
	strAppName = "";

	LPCTSTR szFilter = "钱包文件(*.dat)|*.dat||";

	CFileDialog openFileDlg(FALSE, NULL, strAppName , OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT, szFilter, NULL);

	if(openFileDlg.DoModal() == IDOK)
	{
	CString strFile = openFileDlg.GetPathName();
	}*/

	CFileDialog dlg(FALSE,"dat","wallet",OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.dat||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".dat")))
		{
			return;
		}

		//strPath.AppendFormat(_T(".dat"));
		string strCommand;
		strCommand = strprintf("%s %s",_T("backupwallet"),strPath);
		string strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);

		CString strShowData;
		strShowData.Format(_T("%s:%s"),theApp.m_bChinese ? _T("钱包备份成功") : _T("Wallet backup successful"),strPath);
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, strShowData, theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, strShowData, theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}


//void CKoalaDlg::OnMenuSign()
//{
//	// TODO: 在此添加命令处理程序代码
//	/*CMsgSigDlg dlg;
//	dlg.SetActiveIndex(0);
//	dlg.DoModal();*/
//}


//void CKoalaDlg::OnMenuVerify()
//{
//	// TODO: 在此添加命令处理程序代码
//	/*CMsgSigDlg dlg;
//	dlg.SetActiveIndex(1);
//	dlg.DoModal();
//	*/
//	
//}


void CKoalaDlg::OnMenuExport()
{
	// TODO: 在此添加命令处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.klc||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".klc")))
		{
			return;
		}
		//strPath.AppendFormat(_T(".smc"));
		string strCommand;
		strCommand = strprintf("%s %s","dumpwallet",strPath);
		string strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		CString strShowData;
		strShowData.Format(_T("%s:%s"),UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORT" ,1),strPath);
		AddImportWalletAndBookAddr(strPath);

		//MessageBox(strShowData);
		//UiFun::MessageBoxEx(strShowData , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) ,MFB_OK|MFB_TIP );
	}
}

bool CKoalaDlg::GetFileName(CString &fileName,CString strexe )
{
	int spcace = fileName.Find(" ");
	if (spcace >=0)
	{
		//UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_PATHNOTNULL" ,1) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) ,MFB_OK|MFB_TIP );
		return false;
	}
	int pos = fileName.Find(".",0);
	if (pos <0)
	{
		fileName.AppendFormat(strexe);
	}
	pos = fileName.Find(".",0);
	fileName = fileName.Left(pos);
	fileName.AppendFormat(strexe);
	if(PathFileExistsA(fileName)){
		CString strDisplay;
		strDisplay=UiFun::UI_LoadString("DACRSU" , "DACRSU_INSTEAD" ,1) ;
		/*if (IDOK == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) , MFB_OKCANCEL|MFB_TIP ) )
		{
		return TRUE;
		}*/
	}

	return TRUE;
}

void CKoalaDlg::WriteExportWalletAndBookAddr(CString fileName)
{
	string fiename =strprintf("%s",fileName);
	string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fiename);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile, root)) 
		return;

	int pos = strFile.find("walletaddr");
	if ( pos>=0)
	{
		Json::Value addrValue = root["walletaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];

			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			CString regid = obj["reg_id"].asCString();
			double money = obj["money"].asDouble();
			int cold = obj["cold_dig"].asInt();
			int sig = obj["sign"].asInt();

			string conditon = _T("");
			conditon = strprintf("address = '%s'",addr);
			uistruct::LISTADDR_t pAddr;
			theApp.m_SqliteDeal.GetWalletAddressItem(conditon,&pAddr);
			if (pAddr.address.length() == 0)
			{
				string strSourceData= "";
				strSourceData = strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , addr ,regid ,money ,cold ,sig,label ) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				string strSourceData,strWhere;
				strSourceData = strprintf("label = '%s'" ,label) ;
				strWhere= strprintf("address = '%s'" , addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}
	pos = strFile.find("wbookaddr");
	if (pos >=0)
	{
		Json::Value addrValue = root["bookaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];
			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			string conditon = _T("");
			conditon = strprintf("address = '%s'",addr);
			uistruct::ADDRBOOK_t  pAddr;
			theApp.m_SqliteDeal.GetAddressBookItem(conditon,&pAddr);
			if (pAddr.address.length() == 0)
			{
				string strSourceData= "";
				strSourceData= strprintf("'%s' , '%s' "  ,label, addr) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				string strSourceData,strWhere;
				strSourceData=strprintf("Label = '%s'",label) ;
				strWhere= strprintf("address = '%s'", addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_address_book") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}
}

void CKoalaDlg::AddImportWalletAndBookAddr(CString fileName)
{
	string fiename =strprintf("%s",fileName);
	string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fiename);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile, root)) 
		return;
	/// 自己钱包地址保存  walletaddr
	Json::Value walletaddr; 
	map<string,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));
	if (pListInfo.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<string,uistruct::LISTADDR_t>::iterator item = pListInfo.begin();
		for (;item != pListInfo.end();item++)
		{
			if (item->second.Label.length() != 0)
			{
				itemValue["addr"]=item->second.address;
				itemValue["label"] = item->second.Label;
				itemValue["reg_id"] = item->second.RegID;
				itemValue["money"] = item->second.fMoney;
				itemValue["cold_dig"] = item->second.nColdDig;
				itemValue["sign"] = item->second.bSign;
				Array.append(itemValue);
			}
		}
		root["walletaddr"] = Array;
	}

	map<string,uistruct::ADDRBOOK_t> pAddrBookMap;
	theApp.m_SqliteDeal.GetAddressBookList(_T(" 1=1 "),(&pAddrBookMap));

	if (pAddrBookMap.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<string,uistruct::ADDRBOOK_t>::iterator item = pAddrBookMap.begin();
		for (;item != pAddrBookMap.end();item++)
		{
			//if (item->second.label != _T(""))
			{
				itemValue["addr"]=item->second.address;
				itemValue["label"] = item->second.label;
				Array.append(itemValue);
			}
		}
		root["wbookaddr"] = Array;
	}

	/// 保存到文件
	CStdioFile  File;
	File.Open(fileName,CFile::modeWrite | CFile::modeCreate); 
	string strfile = root.toStyledString();
	File.WriteString(strfile.c_str());
	File.Close();

}

void  CKoalaDlg::ClosWallet()
{
	if (theApp.m_reminder == 0 || theApp.m_reminder == 2)
	{
		//CReminderdlg remindgl(this,theApp.m_reminder);
		//remindgl.DoModal();
	}else if (theApp.m_reminder == 1)  ///最小化
	{
		//ToTray();                              /// 最小化
	}else if (theApp.m_reminder == 3){        /// 关闭程序
		OnCloseWriteAppFee();
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		//::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
	}
	//COut outdlg;
	//if ( IDOK == outdlg.DoModal()){
	//	LogPrint("INFO","Close app start \n");
	//	BeginWaitCursor();
	//	if ( NULL != m_pOutGifDlg ) {
	//		CRect rc;
	//		GetWindowRect(&rc);	
	//		m_pOutGifDlg->LoadGifing(TRUE);
	//		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
	//		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	//	}
	//	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
	//	SetTimer( 0x10 , 2000 , NULL ) ; 
	//}else{
	//	;
	//}
}

void CKoalaDlg::OnCloseWriteAppFee()
{
	if (PathFileExistsA(theApp.m_strInsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.m_strInsPath);
		configpath+= strprintf("\\%s","koalachainclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		int pos =strFile.find("p2pbet");
		if (pos>=0)
		{
			Json::Value setbet = root["p2pbet"];
			ASSERT(!setbet.isNull());
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}else{
			Json::Value setbet ;
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}

		pos =strFile.find("redpacket");
		if (pos>=0)
		{
			Json::Value setred = root["redpacket"];
			ASSERT(!setred.isNull());
			setred["sendredcommFee"]= theApp.m_RedPacketCfg.SendRedPacketCommFee;
			setred["acceptredcommFee"]= theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
			setred["sendredspecalFee"]= theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
			setred["acceptredspecalFee"]= theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
			root["redpacket"]=setred;
		}else{
			Json::Value setred ;
			setred["sendredcommFee"]= theApp.m_RedPacketCfg.SendRedPacketCommFee;
			setred["acceptredcommFee"]= theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
			setred["sendredspecalFee"]= theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
			setred["acceptredspecalFee"]= theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
			root["redpacket"]=setred;
		}

		CStdioFile  File;
		string strpath = theApp.m_strInsPath;
		strpath+="\\koalachainclient.conf";
		if (File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate))
		{
			string strfile = root.toStyledString();
			File.WriteString(strfile.c_str());
			File.Close();
		} 

	}
}

void CKoalaDlg::OnMenuImport()
{
	// TODO: 在此添加命令处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}
	OPENFILENAME ofn;
	char szFile[MAX_PATH];
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0'); 
	ofn.nMaxFile = sizeof(szFile); 
	//CString strFormat;
	//strFormat.Format("%s(*.smc)\0*.smc\0",UiFun::UI_LoadString("DACRSU" , "DACRSU_FILE" ,1));
	//strFormat.AppendFormat("%s(*.*)\0*.*\0\0",UiFun::UI_LoadString("DACRSU" , "DACRSU_ALLFILE" ,1));

	//ofn.lpstrFilter =  "文本文件(*.smc)\0*.smc\0所有文件(*.*)\0*.*\0\0"; 
	ofn.lpstrFilter =  "All Files(*.klc)\0*.klc\0All Files(*.*)\0*.*\0\0"; 
	ofn.nFilterIndex = 1; 
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = 0; 
	ofn.lpstrInitialDir = NULL;	ofn.hwndOwner = m_hWnd; 
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (::GetOpenFileName(&ofn))
	{
		CString strPath = ofn.lpstrFile;
		string strCommand;
		strCommand = strprintf("%s %s",_T("importwallet"),strPath);
		Json::Value root;

		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("UpdateAddressData rpccmd listaddr error");
			return;
		}

		int size = root["imorpt key size"].asInt();
		if (size > 0)
		{
			WriteExportWalletAndBookAddr(strPath);
			//UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORTTIP" ,1) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) ,MFB_OK|MFB_TIP );
			//ClosWallet();
			//((CKoalaDlg*)(this->GetParent()))->Close();
			//ClosWalletWind();
			OnClose();
		}else{
			//UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_EXPORTREEOR" ,1) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) ,MFB_OK|MFB_TIP );
		}
	}
}


void CKoalaDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	if(m_bFirstQuit)
	{
		m_bFirstQuit = FALSE;
		ClosWalletWind();
		return;
	}
	CDialogEx::OnClose();
}


void CKoalaDlg::OnMenuChangeAccount()
{
	// TODO: 在此添加命令处理程序代码
	/*
	m_bEntAcctChecked = !m_bEntAcctChecked;
	//如果不是企业账号,则不显示企业信息
	if(m_bEntAcctChecked)
	{
		m_btnCompanyInfo.ShowWindow(SW_SHOW);
		m_btnCompanyInfo.SetFocusState(TRUE);
		ActiveDlg(IDD_COMPANYINFO_DLG);
		
	}
	//如果是企业账号的话，显示企业信息
	if(!m_bEntAcctChecked)
	{
		m_btnCompanyInfo.ShowWindow(SW_HIDE);
	}

	Invalidate();
	*/

	CSetAdminDlg dlg;
	dlg.m_bShowAdmin = FALSE;
	dlg.DoModal();
}

void CKoalaDlg::OnMenuSetAccount()
{
	// TODO: 在此添加命令处理程序代码
	CSetAdminDlg dlg;
	dlg.DoModal();
}

void CKoalaDlg::OnMenuEncrypt()
{
	// TODO: 在此添加命令处理程序代码
	if (theApp.m_bIsHaveLocked)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("已经加过密")  , _T("提示")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Has been encrypted")  , _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	CWalletEncDlg dlg;
	dlg.DoModal();
}




void CKoalaDlg::OnMenuChangePassword()
{
	// TODO: 在此添加命令处理程序代码
	CModifyPasswordDlg dlg;
	dlg.DoModal(); 
}


void CKoalaDlg::OnMenuInfo()
{
	// TODO: 在此添加命令处理程序代码
	CToolDlg dlg;
	dlg.SetActiveIndex(0);
	dlg.DoModal();

}


void CKoalaDlg::OnMenuPeerlist()
{
	// TODO: 在此添加命令处理程序代码
	CToolDlg dlg;
	dlg.SetActiveIndex(1);
	dlg.DoModal();
	

}


void CKoalaDlg::OnMenuCmd()
{
	// TODO: 在此添加命令处理程序代码
	CToolDlg dlg;
	dlg.SetActiveIndex(2);
	dlg.DoModal();	

}


void CKoalaDlg::OnMenuCmdHelp()
{
	// TODO: 在此添加命令处理程序代码
	/*m_pCmdHelpDlg = new CCmdHelpDlg;
	m_pCmdHelpDlg->Create(IDD_CMDHELP_DLG, this);
	m_pCmdHelpDlg->ShowWindow(SW_SHOW);

	m_vecCmdHelp.push_back(m_pCmdHelpDlg);*/
}


void CKoalaDlg::OnMenuVersion()
{
	// TODO: 在此添加命令处理程序代码
	CVersionDlg dlg;
	dlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!theApp.m_bChinese) {
		SetWindowText("About Koala Chain");
		GetDlgItem(IDC_STATIC_ABOUT)->SetWindowText("Koala Chain，v1.0");
		GetDlgItem(IDOK)->SetWindowText("OK");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CKoalaDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child Windows dont have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup cant be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}

}



void CKoalaDlg::OnUpdateMenuChangeAccount(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//pCmdUI->SetCheck(m_bEntAcctChecked);
}


void  CKoalaDlg::ClosWalletWind()
{
	OnCloseWriteAppFee();
	BeginWaitCursor();
	
	if ( NULL != m_pOutGifDlg ) {
		CRect rc;
		GetWindowRect(&rc);	
		m_pOutGifDlg->LoadGifing(TRUE);
		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	}

	SetTimer(0x10, 2000, NULL);

}

void CKoalaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( 0x10 == nIDEvent ) {
		KillTimer(0x10);
		StopSever();

		LogPrint("INFO","OnTimer start \n");
		if ( NULL != m_pOutGifDlg ) {
			m_pOutGifDlg->ShowWindow(SW_HIDE) ;
			m_pOutGifDlg->LoadGifing(FALSE);
			delete m_pOutGifDlg ;
			m_pOutGifDlg = NULL ;
		}
		theApp.m_bIsCloseAppFlag = TRUE ;
		CloseThread();
		DestroyDlg();

		PostMessage( WM_CLOSE ); 	
	}
	CDialogEx::OnTimer(nIDEvent);
}

void  CKoalaDlg::StopSever()
{
	string strCommand;
	strCommand = strprintf("%s",_T("stop"));
	string strSendData;
	string strret = _T("Koala server stopping");

	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	int RecivetxTimestart =0;
	RecivetxTimestart=(int) UiFun::SystemTimeToTimet(curTime);
	bool nRecStopCmd = false;
	while(TRUE){
		if(!nRecStopCmd) 
		{
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		}
		if (strSendData.find(strret) >=0)
		{
			nRecStopCmd = true;
		}
		GetLocalTime( &curTime ) ;
		int RecivetxTimeLast =0;
		RecivetxTimeLast= (int)UiFun::SystemTimeToTimet(curTime);
		if ((RecivetxTimeLast - RecivetxTimestart) > 5)
		{
			return;
		}
		Sleep(5);
	}

}

void CKoalaDlg::DestroyDlg()
{
	if ( NULL != m_pOverviewDlg ) 
	{
		delete m_pOverviewDlg ;
		m_pOverviewDlg = NULL ;
	}
	if ( NULL != m_pSendDlg ) 
	{
		delete m_pSendDlg ;
		m_pSendDlg = NULL ;
	}
	if ( NULL != m_pTradeRecordDlg ) 
	{
		delete m_pTradeRecordDlg ;
		m_pTradeRecordDlg = NULL ;
	}
	if ( NULL != m_pReceiveDlg ) 
	{
		delete m_pReceiveDlg ;
		m_pReceiveDlg = NULL ;
	}

	if( NULL != m_pCompanyInfoDlg)
	{
		delete m_pCompanyInfoDlg;
		m_pCompanyInfoDlg = NULL;
	}

	if( NULL != m_pAddrListDlg)
	{
		delete m_pAddrListDlg;
		m_pAddrListDlg = NULL;
	}

	if ( NULL != m_pStatusDlg ) 
	{
		delete m_pStatusDlg ;
		m_pStatusDlg = NULL ;
	}

}


void CKoalaDlg::CloseThread()
{
	DWORD exc;
	theApp.m_bIsProcessMsgRunSwitch = 0;
	closesocket(theApp.m_uiConnSocket);

	while( ::GetExitCodeThread( theApp.m_hReceiveThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hReceiveThread );
	while( ::GetExitCodeThread( theApp.m_hPraseJsonToMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hPraseJsonToMsgThread );
	while( ::GetExitCodeThread( theApp.m_hProcessMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hProcessMsgThread);

	while( ::GetExitCodeThread( theApp.m_hCheckUpdateThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hCheckUpdateThread);
}

void  CKoalaDlg::WriteConfig()
{
	if (PathFileExistsA(theApp.m_strInsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.m_strInsPath);
		configpath+= strprintf("\\%s","koalachainclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;
		int pos = strFile.find("paramconf");
		if (pos>=0)
		{
			Json::Value paramcfg = root["paramconf"];
			ASSERT(!paramcfg.isNull());
			paramcfg["admin"]= theApp.m_bIsAdmin;
			paramcfg["admin_address"] = theApp.m_strCurAdminAddress;
			paramcfg["admin_short_address"] = theApp.m_strCurShortAddress;
			paramcfg["ent_address"] = theApp.m_strCurEntAddress;
			root["paramconf"]=paramcfg;
		}else{
			Json::Value obj;
			obj["admin"]= theApp.m_bIsAdmin;
			obj["admin_address"] = theApp.m_strCurAdminAddress;
			obj["admin_short_address"] = theApp.m_strCurShortAddress;
			obj["ent_address"] = theApp.m_strCurEntAddress;
			root["paramconf"]=obj;
		}
		CStdioFile  File;
		string strpathe=theApp.m_strInsPath;
		strpathe +="\\koalachainclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpathe.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}
}

void CKoalaDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bFirstQuit)
	{
		m_bFirstQuit = FALSE;
		if (!theApp.m_bIsPassLock)
		{
			EndDialog(IDD_DIALOG_SETWALLETENTRP);
		}
		WriteConfig();
		ClosWalletWind();
		return;
	}
	CDialogEx::OnClose();
}


int CKoalaDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if(theApp.m_bChinese) {
		m_Menu.LoadMenu(IDR_MENU);
	} else {
		m_Menu.LoadMenu(IDR_MENU_ENGLISH);	
	}

	SetMenu(&m_Menu);

	return 0;
}


LRESULT CKoalaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	switch(message) 
	{
	case WM_COMMAND:
		{/*
			if ( ID_MENU_INFO == LOWORD(wParam) || ID_MENU_PEERLIST == LOWORD(wParam) || ID_MENU_CMD == LOWORD(wParam) ) 
			{
				CRect rcWindow;
				GetWindowRect(&rcWindow);
				if(NULL == m_pToolDlg)
				{
					m_pToolDlg = new CToolDlg;
										
					m_pToolDlg->Create(IDD_TOOL_DLG, this);
					
				}
				
				if(ID_MENU_INFO == LOWORD(wParam))
				{
					m_pToolDlg->m_wndTab.SetActivePage(0);
					
				}
				if(ID_MENU_PEERLIST == LOWORD(wParam))
				{
					m_pToolDlg->m_wndTab.SetActivePage(1);
				}
				if(ID_MENU_CMD == LOWORD(wParam))
				{
					m_pToolDlg->m_wndTab.SetActivePage(2);
				}

				if ( NULL != m_pToolDlg )
				{
					//m_pToolDlg->MoveWindow(rcWindow.right,rcWindow.top,800,rcWindow.Height()-8);
					m_pToolDlg->ShowWindow(SW_SHOW);
				}
				m_pToolDlg->m_wndTab.UpdateWindow();
			}
			}*/

			if(ID_MENU_CMD_HELP == LOWORD(wParam))
			{
				if(NULL == m_pCmdHelpDlg)
				{
					m_pCmdHelpDlg = new CCmdHelpDlg;

					m_pCmdHelpDlg->Create(IDD_CMDHELP_DLG, this);

				}

				if ( NULL != m_pCmdHelpDlg )
				{
					//m_pToolDlg->MoveWindow(rcWindow.right,rcWindow.top,800,rcWindow.Height()-8);
					m_pCmdHelpDlg->ShowWindow(SW_SHOW);
				}
			}

			if(ID_MENU_SIGN == LOWORD(wParam) || ID_MENU_VERIFY == LOWORD(wParam))
			{
				
				if(NULL == m_MsgSigDlg)
				{
					m_MsgSigDlg = new CMsgSigDlg;

					m_MsgSigDlg->Create(IDD_MESSAGE_SIG_DLG, this);
				}

				if(ID_MENU_SIGN == LOWORD(wParam))
				{
					m_MsgSigDlg->m_wndTab.SetActivePage(0);

				}
				if(ID_MENU_VERIFY == LOWORD(wParam))
				{
					m_MsgSigDlg->m_wndTab.SetActivePage(1);
				}

				if ( NULL != m_MsgSigDlg )
				{
					m_MsgSigDlg->ShowWindow(SW_SHOW);
				}

				m_MsgSigDlg->m_wndTab.UpdateWindow();

			}
		}
		break;

	default:
		break ;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CKoalaDlg::OnMenuLock()
{
	// TODO: 在此添加命令处理程序代码
	if (!theApp.m_bIsHaveLocked)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("钱包没有加锁") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The wallet is not locked") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	string strCommand;
	strCommand = strprintf("%s",_T("walletlock"));

	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}


	bool isEntryp = root["walletlock"].asBool();
	if (!isEntryp)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("钱包锁定失败")  , _T("提示") ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Wallet lock failed")  , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
}


BOOL CKoalaDlg::PreTranslateMessage(MSG* pMsg)
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

	if (::TranslateAccelerator(GetSafeHwnd(), m_hAccelClose, pMsg))
	{
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void  CKoalaDlg::WriteLanguageConfig(bool bChinese)
{
	if (PathFileExistsA(theApp.m_strInsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.m_strInsPath);
		configpath+= strprintf("\\%s","koalachainclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;
		int pos = strFile.find("language");
		if (pos>=0)
		{
			Json::Value paramcfg = root["language"];
			ASSERT(!paramcfg.isNull());
			paramcfg["name"]= bChinese ? "chinese" : "english";
			root["language"]=paramcfg;
		}else{
			Json::Value obj;
			obj["name"]= bChinese ? "chinese" : "english";
			root["language"]=obj;
		}
		CStdioFile  File;
		string strpathe=theApp.m_strInsPath;
		strpathe +="\\koalachainclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpathe.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}
}


BOOL CKoalaDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style &= ~WS_MAXIMIZEBOX;

	return CDialogEx::PreCreateWindow(cs);
}





void CKoalaDlg::OnMenuChinese()
{
	// TODO: 在此添加命令处理程序代码
	if(theApp.m_bChinese) {
		return;
	}

	int res = 0;
	if(theApp.m_bChinese)
		res = ::MessageBox( m_hWnd, _T("修改语言坏境请手动重新启动钱包使其生效！") , _T("提示"), MB_YESNO );
	else
		res = ::MessageBoxEx( m_hWnd, _T("Change the language environment, please restart manually to make it effective！") , _T("tip"), MB_YESNO, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	if(IDYES == res)
	{
		WriteLanguageConfig();
	}
}


void CKoalaDlg::OnUpdateMenuChinese(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(theApp.m_bChinese);
	
}


void CKoalaDlg::OnMenuEnglish()
{
	// TODO: 在此添加命令处理程序代码
	if(!theApp.m_bChinese) {
		return;
	}

	int res = 0;
	if(theApp.m_bChinese)
		res = ::MessageBox( m_hWnd, _T("修改语言坏境请手动重新启动钱包使其生效！") , _T("提示"), MB_YESNO );
	else
		res = ::MessageBoxEx( m_hWnd, _T("Change the language environment, please restart manually to make it effective！") , _T("tip"), MB_YESNO, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	if(IDYES == res)
	{
		WriteLanguageConfig(false);
	}
}


void CKoalaDlg::OnUpdateMenuEnglish(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(!theApp.m_bChinese);
}
