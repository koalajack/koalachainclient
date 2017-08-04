// StatusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "StatusDlg.h"
#include "afxdialogex.h"
#include "KoalaDlg.h"
#include "BalloonTip.h"
#include <WinVer.h>


#pragma comment(lib, "Version.lib ")

// CStatusDlg 对话框

IMPLEMENT_DYNAMIC(CStatusDlg, CDialogBar)

CStatusDlg::CStatusDlg()
{
	m_pBmp = NULL ;
	m_bProgressType = FALSE;
	m_bIsShowProgress = FALSE;
	m_ProgressWnd = NULL ;
	m_nSigIndex = 0 ;
	m_nLockIndex = 0;
	m_bIsWalletUI = FALSE;
	m_strNetName = _T("");
	memset(m_rectSignal , 0 , sizeof(CRect));
	memset(m_rectLock , 0 , sizeof(CRect));
	m_progress.ShowPercent(FALSE);
	m_progress.ShowDefineText(TRUE);
	m_nConnectCount = 0;
}

CStatusDlg::~CStatusDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	if ( NULL != m_ProgressWnd ) {
		CGIFControl *gifcontrol= ((CGIFControl*)m_ProgressWnd);
		delete gifcontrol ;
		gifcontrol = NULL ;
	}
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_ST_NET_TB, m_strNeting);
	DDX_Control(pDX, IDC_ST_HEIGHT, m_strHeight);
	DDX_Control(pDX, IDC_ST_VERSION, m_strVersion);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_MESSAGE(MSG_USER_UP_PROGRESS , &CStatusDlg::OnShowProgressCtrl  )
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CStatusDlg 消息处理程序


void CStatusDlg::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}
void CStatusDlg::LoadGifing( BOOL bState )
{
	if( NULL != m_ProgressWnd ) {
		if( m_ProgressWnd->GetSafeHwnd() ) {
			if( TRUE == bState ) {
				if( TRUE == ((CGIFControl*)m_ProgressWnd)->Load((char*)theApp.m_strProgressGifFile.c_str()) ) {
					CRect rc ;
					GetClientRect( rc ) ;
					Invalidate() ;
					m_ProgressWnd->SetWindowPos( NULL , 842 - 30 , 8 , 0 , 0 , \
						SWP_SHOWWINDOW|SWP_NOSIZE ) ;
					((CGIFControl*)m_ProgressWnd)->Play();
				}
			}else{
				((CGIFControl*)m_ProgressWnd)->Stop() ;	
			}
		}
	}
}

BOOL CStatusDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return 1;
}

int CStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	OnIniLockParam();         //设置锁的变量
	return 0;
}

BOOL CStatusDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);

		m_strNeting.SetFont(90, _T("宋体"));				//设置显示字体和大小
		//m_strNeting.SetTextColor(RGB(255,255,255));			    //字体颜色
		if (theApp.m_nNetWorkType == MAIN)
		{
			m_strNetName = theApp.m_bChinese ? "主" : "Main"; 
		}else if (theApp.m_nNetWorkType == REGTEST)
		{
			m_strNetName = theApp.m_bChinese ? "局域" : "Regtest" ; 
		}else if (theApp.m_nNetWorkType == TESTNET)
		{
			m_strNetName = theApp.m_bChinese ? "测试" : "Test"; 
		}
		string strTemp ="";
		strTemp =strprintf("%s%s%s", theApp.m_bChinese ? "获取" : "Getting ", m_strNetName, theApp.m_bChinese ? "网络连接" : "net connection");
		m_strNeting.SetWindowText(strTemp.c_str()) ;


		m_strHeight.SetFont(90, _T("宋体"));				//设置显示字体和大小
		//m_strHeight.SetTextColor(RGB(255,255,255));			    //字体颜色
		m_strHeight.SetWindowText(theApp.m_bChinese ? "高度:" : "Height") ;
		m_strHeight.ShowWindow(SW_HIDE) ;

		m_strVersion.SetFont(90, _T("宋体"));				//设置显示字体和大小
		//m_strVersion.SetTextColor(RGB(255,255,255));	    //字体颜色

		string ver = strprintf("%s:v%s RC", theApp.m_bChinese ? "版本" : "Version", GetUIVersion());
		m_strVersion.SetWindowText(ver.c_str()) ;
		//m_strVersion.SetWindowText(_T("版本:v1.0.2.0 RC")) ;

		if ( NULL == m_ProgressWnd ) {
			m_ProgressWnd = new CGIFControl ;
			m_ProgressWnd->Create(_T("") , WS_CHILD | SS_OWNERDRAW | WS_VISIBLE | SS_NOTIFY , \
				CRect(20,20,36,36) , this, 111 ) ;
		}

		m_SignalBmpArray[0].LoadBitmap(IDB_BITMAP_SIGNAL0);  
		m_SignalBmpArray[1].LoadBitmap(IDB_BITMAP_SIGNAL1);  
		m_SignalBmpArray[2].LoadBitmap(IDB_BITMAP_SIGNAL2);  
		m_SignalBmpArray[3].LoadBitmap(IDB_BITMAP_SIGNAL3);  

		m_LockBmpArray[0].LoadBitmap(IDB_BITMAP_LOCK); 
		m_LockBmpArray[1].LoadBitmap(IDB_BITMAP_UNLOCK); 

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_UP_PROGRESS ) ;

		//m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//背景色
		//m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//前景色

		m_progress.SetTextColor(RGB(0, 0, 0));
		m_progress.SetBarBkColor(RGB(255, 255, 255));
		m_progress.SetBarColor(RGB(255, 0, 0));

		LoadGifing(TRUE);
	}

	return bRes;
}


int CStatusDlg::ShowProgressCtrl(){
	CPostMsg postmsg;
	if (!theApp.m_msgProcessBarQueue.pop(postmsg))
	{
		return 1;
	}

	uistruct::BLOCKCHANGED_t pBlockchanged; 
	string strTemp = postmsg.GetData();
	pBlockchanged.JsonToStruct(strTemp.c_str());
	LogPrint("CProgStatusBar", "MSG_USER_UP_PROGRESS WM_CONNECTNET 更新进度条消息:%s\n",strTemp.c_str());
	if (pBlockchanged.tips <= 0)
	{
		return 1;
	}

	//// blocktip高度
	theApp.m_nBlockTipHight = pBlockchanged.tips ;
	if (!m_bProgressType)
	{
		string strTemp = "";
		strTemp = strprintf("%s%s",m_strNetName , theApp.m_bChinese ? "网络同步中..." : "Network synchronization...");
		m_strNeting.SetWindowText(strTemp.c_str());
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.SetRange32( 0 , 100); 
		int  setpos =(int)((pBlockchanged.high*1.0/pBlockchanged.tips)*100) ;
		setpos = setpos>100?100:setpos;
		//设置进度条的值
		m_progress.SetPos(setpos);
		CString strText;
		strText.AppendFormat("%s ~%d", theApp.m_bChinese ? "剩余" : "remaining", pBlockchanged.tips-pBlockchanged.high);
		strText.AppendFormat(" %s",theApp.m_bChinese ? "块没有同步到本地" : "blocks not synchronized");
		m_progress.SetDefinedStr(strText);
		m_bProgressType = TRUE;
		//m_nSigIndex =pBlockchanged.connections>3?3:pBlockchanged.connections;

		if(m_nConnectCount < pBlockchanged.connections)
			m_nConnectCount = pBlockchanged.connections;

		m_nSigIndex =m_nConnectCount>3?3:m_nConnectCount;

		if (pBlockchanged.tips==pBlockchanged.high)
		{
			//theApp.IsSyncAppTx = TRUE;             /// 同步app交易
		}

		if ((pBlockchanged.tips-pBlockchanged.high)<10 && !m_bIsWalletUI)
		{
			TRACE("ok:%s\r\n","OnShowProgressCtrl");
			//// 发送钱包同步完毕
			CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
			theApp.m_msgQueue.pushFront(postblockmsg); 
			LoadGifing(false);
			m_bIsWalletUI = true;
			theApp.m_bIsSyncBlock = true;
		}
		Invalidate(); 
		//InvalidateRect(m_bmpsig);
		//		return 1;
	}

	//m_nSigIndex = pBlockchanged.connections>3?3:pBlockchanged.connections;
	m_nSigIndex =m_nConnectCount>3?3:m_nConnectCount;
	int  setpos =(int) ((pBlockchanged.high*1.0/pBlockchanged.tips)*100) ;
	setpos = setpos>100?100:setpos;
	//设置进度条的值
	m_progress.SetPos(setpos);
	CString strText;
	strText.AppendFormat("%s ~%d", theApp.m_bChinese ? "剩余" : "remaining", pBlockchanged.tips-pBlockchanged.high);
	strText.AppendFormat(" %s",theApp.m_bChinese ? "块没有同步到本地" : "blocks not synchronized");
	m_progress.SetDefinedStr(strText);
	m_progress.Invalidate();

	if (pBlockchanged.tips==pBlockchanged.high)
	{
		//theApp.IsSyncAppTx = TRUE;             /// 同步app交易
	}
	if ((pBlockchanged.tips-pBlockchanged.high)<10&& !m_bIsWalletUI)
	{
		TRACE("ok:%s\r\n","OnShowProgressCtrl");
		//// 发送钱包同步完毕
		CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
		theApp.m_msgQueue.pushFront(postblockmsg); 
		LoadGifing(false);
		m_bIsWalletUI = true;
		theApp.m_bIsSyncBlock = true;
	}
	if ( m_bIsWalletUI && !m_bIsShowProgress) {
		string strTemp = "";
		strTemp =strprintf("%s%s",m_strNetName , theApp.m_bChinese ? "网络同步中..." : "Network synchronization...");
		m_strNeting.SetWindowText(strTemp.c_str()) ;
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.ShowWindow(SW_HIDE);
		if ( NULL != m_ProgressWnd ) {
			m_ProgressWnd->ShowWindow(SW_HIDE) ;
		}
		if (m_bIsShowProgress == false)
		{
			m_bIsShowProgress =true;
		}else{
			m_bIsShowProgress =false;
		}
	}

	if (m_bIsWalletUI && m_bIsShowProgress)
	{
		string strTemp;
		strTemp = strprintf(theApp.m_bChinese ? "%s网络" : "%s net",m_strNetName);
		m_strNeting.SetWindowText(strTemp.c_str());
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		string strTips;
		strTips = strprintf("%s:%d" ,theApp.m_bChinese ? "当前高度" : "Height" ,pBlockchanged.tips );
		m_strHeight.SetWindowText(strTips.c_str()) ;
		m_strHeight.ShowWindow(SW_HIDE);
		m_strHeight.ShowWindow(SW_SHOW);
	}
	InvalidateRect(m_rectSignal);

	SetAppStepfee(pBlockchanged.fuelrate);
	return 1;
}

void CStatusDlg::ShowLockCtrl()
{
	CPostMsg postmsg;
	if (!theApp.m_msgLockStatusQueue.pop(postmsg))
	{
		return ;
	}
	string strTemp = postmsg.GetStrType();
	if (!strcmp(strTemp.c_str(),"Lock"))
	{
		theApp.m_bIsHaveLocked = TRUE;
		m_nLockIndex = 0;
	}else if(!strcmp(strTemp.c_str(),"UnLock")){
		theApp.m_bIsHaveLocked = TRUE;
		m_nLockIndex = 1;
	}
	InvalidateRect(m_rectLock);
}

void CStatusDlg::ShowNetCount()
{
	CPostMsg postmsg;

	CSingleLock s1(&mutexObj);
	s1.Lock();

	int size = theApp.m_msgConnCountQueue.size();
	while(size > 1) {
		theApp.m_msgConnCountQueue.pop(postmsg);
		size--;
	}

	if (!theApp.m_msgConnCountQueue.pop(postmsg))
	{
		s1.Unlock();
		return ;
	}

	s1.Unlock();
	string strTemp = postmsg.GetStrType();
	int pos = strTemp.find("connections") ;
	if (pos >=0)
	{
		pos = strTemp.find("=") ;
		if (pos >=0)
		{
			strTemp = strTemp.substr(pos+1,strTemp.length());
			strTemp =  UiFun::trimleft(strTemp);
			strTemp =  UiFun::trimright(strTemp);
			int netCount = atoi(strTemp.c_str());
			m_nSigIndex = netCount>3?3:netCount;
			InvalidateRect(m_rectSignal);

			m_nConnectCount = netCount;
			TRACE("cc: %d\n", m_nConnectCount);
			if (netCount == 0 )
			{
				string strTemp ="";
				strTemp =strprintf("%s%s%s",theApp.m_bChinese ? "获取" : "Getting",m_strNetName, theApp.m_bChinese ? "网络连接" : "net connection");
				m_strNeting.SetWindowText(strTemp.c_str()) ;
				Invalidate(); 
			}
		}	
	}
}
LRESULT CStatusDlg::OnShowProgressCtrl( WPARAM wParam, LPARAM lParam ) 
{
	//	TRACE("OnShowProgressCtrl:%s\r\n","OnShowProgressCtrl");
	if (wParam == WM_LOCKSTATE)
	{
		ShowLockCtrl();
	}else if (wParam == WM_CONNECTNET){
		ShowNetCount();
	}else{
		ShowProgressCtrl();
	}
	return 1;
}
//Invalidate(); 
void CStatusDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogBar::OnPaint()
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CRect rc;  
	GetClientRect(&rc);  

	BITMAP bm1;
	m_SignalBmpArray[m_nSigIndex].GetObject(sizeof(BITMAP),&bm1);

	int width = bm1.bmWidth; //图片的宽度（逻辑单位）。 
	int height=	bm1.bmHeight; ////图片的高度（逻辑单位）。

	HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_SignalBmpArray[m_nSigIndex]); 
	dc.BitBlt(rc.Width()-60, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	CRect rc1(rc.Width()-60, rc.Height()-height, rc.Width()-60+width, rc.Height());
	m_rectSignal = rc1;
	memDC.SelectObject(hOldbmp);  
	//	memDC.DeleteDC();  
	if (theApp.m_bIsHaveLocked)
	{
		HBITMAP hlockbmp = (HBITMAP)memDC.SelectObject(m_LockBmpArray[m_nLockIndex]); 
		dc.BitBlt(rc.Width()-60-30, 3, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
		CRect rc2(rc.Width()-60-30, 3, rc.Width(), rc.Height());
		m_rectLock = rc2;
		memDC.SelectObject(hlockbmp);  
	}
	memDC.DeleteDC();  
}
void CStatusDlg::OnIniLockParam()
{
	string strCommand;
	strCommand = strprintf("%s",_T("islocked"));
	string strShowData ="";
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("OnIniLockParam rpccmd islocked error");
		return;
	}

	int state = root["islock"].asInt();
	if (state == 0)     /// 没有锁
	{
		theApp.m_bIsHaveLocked = FALSE;
		theApp.m_bIsWalletLocked =FALSE;
	}else if(state == 1){   /// 解锁
		theApp.m_bIsWalletLocked =FALSE;
		m_nLockIndex = 1;
		theApp.m_bIsHaveLocked = TRUE;
	}else if (state == 2)    /// 锁定状态
	{
		theApp.m_bIsWalletLocked =TRUE;
		m_nLockIndex = 0;
		theApp.m_bIsHaveLocked = TRUE;
	}

	theApp.m_nLockStatus = state; 
	//// 更新block
	CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
	theApp.m_msgQueue.push(postblockmsg);  
}

void CStatusDlg::SetAppStepfee(int fuelrate)
{
	if (fuelrate == 0)
	{
		return;
	}

	theApp.m_BussCfg.ConfigFee = (theApp.m_bussStep.ConfigStep/100+1)*fuelrate + 10000;
	theApp.m_BussCfg.ModifyFee = (theApp.m_bussStep.ModifyStep/100+1)*fuelrate+ 10000;
	theApp.m_BussCfg.RechargeFee = (theApp.m_bussStep.RechargeStep/100+1)*fuelrate+ 10000;
	theApp.m_BussCfg.WithdrawFee = (theApp.m_bussStep.WithdrawStep/100+1)*fuelrate+ 10000;
}

string CStatusDlg::GetUIVersion()
{
	string strRet = "";
	char szAppFullPath[_MAX_PATH] = {0};

	GetModuleFileName(NULL,szAppFullPath,MAX_PATH);//得到程序模块名称，全路径

	//获取当前文件的版本信息
	DWORD dwLen = GetFileVersionInfoSize(szAppFullPath,NULL); 
	char *pszAppVersion = new char[dwLen+1];
	if(pszAppVersion)
	{
		memset(pszAppVersion,0,sizeof(char)*(dwLen+1));
		GetFileVersionInfo(szAppFullPath,NULL,dwLen,pszAppVersion);
		CString strVersion;
		UINT nLen(0);
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		VerQueryValue(pszAppVersion,"\\",(LPVOID*)&pFileInfo,&nLen);
		if(pFileInfo)
		{
			//获取版本号
			strRet = strprintf("%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strRet;
}

void CStatusDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect temp = m_rectSignal;
	//ClientToScreen(&temp);
	CKoalaDlg *pDlg = (CKoalaDlg*)(theApp.m_pMainWnd);
	if (temp.PtInRect(point))
	{
		RECT ret;
		GetWindowRect(&ret);


		LOGFONT lf;
		::ZeroMemory (&lf, sizeof (lf));
		lf.lfHeight = 11;
		lf.lfWeight = FW_BOLD;
		lf.lfUnderline = FALSE;
		strcpy((char*)lf.lfFaceName, "宋体");

		string strShow = strprintf("%s:%d",theApp.m_bChinese ? "当前网络连接数" : "active connections", m_nConnectCount);
		if (IsWindowVisible()&& pDlg->m_pBalloonTip->nBalloonInstances !=1)
		{
			pDlg->m_pBalloonTip=CBalloonTip::Show(
				CPoint(ret.right -50, ret.bottom),         // Point on the screen where the tip will be shown
				CSize(85, 80),          // Size of the total rectangle encompassing the balloon 
				_T(strShow.c_str()), // Message to be shown in the balloon
				lf,                               // LOGFONT structure for font properties 
				5,                 // Time in seconds to show the balloon
				FALSE ,             // TRUE  == Balloon is up(Balloon Tip is down) 
				FALSE // ==  Balloon is down(Balloon Tip is up)
				);
		}
	}else if (pDlg->m_pBalloonTip != NULL && pDlg->m_pBalloonTip->nBalloonInstances ==1){
		int pos = pDlg->m_pBalloonTip->m_strMessage.Find(theApp.m_bChinese ? "网络连接" : "net connection");
		if (pos >=0)
		{
			CBalloonTip::Hide(pDlg->m_pBalloonTip);
		}
	}

	CDialogBar::OnMouseMove(nFlags, point);
}


BOOL CStatusDlg::PreTranslateMessage(MSG* pMsg)
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
	return CDialogBar::PreTranslateMessage(pMsg);
}
