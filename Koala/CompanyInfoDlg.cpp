XConnPtContainer
// CompanyInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "CompanyInfoDlg.h"
#include "afxdialogex.h"
#include "EntRigeistDlg.h"
#include "EntCfgDlg.h"
#include "EntRechargeDlg.h"
#include "EntWithdrawDlg.h"
#include "CommonStruct.h"
#include "EntInfoDlg.h"

#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

bool comp(const LIST_DATA &a,const LIST_DATA &b)
{
	return a.scriptId >  b.scriptId;
}

// CCompanyInfoDlg 对话框

#define TIMER_FRESH 10003

IMPLEMENT_DYNAMIC(CCompanyInfoDlg, CDialogEx)

CCompanyInfoDlg::CCompanyInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompanyInfoDlg::IDD, pParent)
{
	m_nTotalPage = 0 ;
	m_nCurrentPage = 0;
	m_nSerialNum = 0;
	m_uSaveNum = 0;
}

CCompanyInfoDlg::~CCompanyInfoDlg()
{
}

void CCompanyInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstInfo);
	DDX_Control(pDX, IDC_MFCBTN_REGISTER, m_btnRegister);
	DDX_Control(pDX, IDC_MFCBTN_CONFIG, m_btnConfig);
	DDX_Control(pDX, IDC_MFCBTN_RECHARGE, m_btnRecharge);
	DDX_Control(pDX, IDC_INFO_PAGE, m_staPage);
	DDX_Control(pDX, IDC_MFCBTN_WITHDRAW, m_btnWithDraw);
}


BEGIN_MESSAGE_MAP(CCompanyInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBTN_REGISTER, &CCompanyInfoDlg::OnBnClickedMfcbtnRegister)
	ON_BN_CLICKED(IDC_MFCBTN_CONFIG, &CCompanyInfoDlg::OnBnClickedMfcbtnConfig)
	ON_BN_CLICKED(IDC_MFCBTN_RECHARGE, &CCompanyInfoDlg::OnBnClickedMfcbtnRecharge)
	ON_BN_CLICKED(IDC_MFCBTN_WITHDRAW, &CCompanyInfoDlg::OnBnClickedMfcbtnWithdraw)
	ON_BN_CLICKED(IDC_MFCBTN_INFO_UP, &CCompanyInfoDlg::OnBnClickedMfcbtnInfoUp)
	ON_BN_CLICKED(IDC_MFCBTN_INFO_NEXT, &CCompanyInfoDlg::OnBnClickedMfcbtnInfoNext)
	ON_MESSAGE(MSG_USER_UP_PROGRESS , &CCompanyInfoDlg::OnShowListCtrl )
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CCompanyInfoDlg::OnDblclkList1)
END_MESSAGE_MAP()


// CCompanyInfoDlg 消息处理程序

void Hex2String(string hexstring, string& outstring)
{
	vector<unsigned char> vChar = CSoyPayHelp::getInstance()->ParseHex(hexstring.c_str());
	outstring.assign(vChar.begin(),vChar.end());
}

void Hex2ULL(string hexstring, ULONGLONG& ull)
{
	vector<unsigned char> vChar = CSoyPayHelp::getInstance()->ParseHex(hexstring.c_str());
	string str;
	str.assign(vChar.begin(),vChar.end());
	memcpy(&ull, str.c_str(), 8);
}

void Hex2UL(string hexstring, ULONG& ul)
{
	vector<unsigned char> vChar = CSoyPayHelp::getInstance()->ParseHex(hexstring.c_str());
	string str;
	str.assign(vChar.begin(),vChar.end());
	memcpy(&ul, str.c_str(), 4);
}

LIST_APP_DATA CCompanyInfoDlg::GetAppValue(const Json::Value &root)
{
	LIST_APP_DATA ret;
	
	ret.scriptId = root["scriptId"].asString();
	//vector<unsigned char> vChar = CSoyPayHelp::getInstance()->ParseHex(root["description"].asString().c_str());
	//ret.entName.assign(vChar.begin(),vChar.end());

	Hex2String(root["description"].asString(), ret.entName);
	
	return ret;
}

void CCompanyInfoDlg::ReadListApp(const Json::Value &root)
{
	Json::Value listapp = root["listregedscript"];
	if (listapp.isNull())
	{
		return;
	}
	for(unsigned int i=0; i<listapp.size();++i)
	{
		LIST_APP_DATA appdata = GetAppValue(listapp[i]);
		m_listapp[appdata.scriptId]=appdata;
	}
}

void CCompanyInfoDlg::GetHeight()
{
	string strCommand;
	strCommand = strprintf("%s",_T("getinfo"));
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return ;
	}

	TRACE(root.toStyledString().c_str());

	Json::Value blocks = root["blocks"];
	if (blocks.isNull())
	{
		return;
	}

	theApp.m_nBlockTipHight = root["blocks"].asInt();
}

void CCompanyInfoDlg::ShowWithdrawBtn(BOOL bShow )
{

	if(bShow)
	{
		m_btnWithDraw.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnWithDraw.ShowWindow(SW_HIDE);
	}
}

void CCompanyInfoDlg::ShowRechargeBtn(BOOL bShow )
{

	if(bShow)
	{
		m_btnRecharge.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnRecharge.ShowWindow(SW_HIDE);
	}
}


void CCompanyInfoDlg::HideAdminBtns(BOOL bHide)
{
	if(bHide)
	{
		m_btnConfig.ShowWindow(SW_HIDE);
		//m_btnRecharge.ShowWindow(SW_HIDE);
		m_btnRegister.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnConfig.ShowWindow(SW_SHOW);
		//m_btnRecharge.ShowWindow(SW_SHOW);
		m_btnRegister.ShowWindow(SW_SHOW);
	}
}

BOOL CCompanyInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		/*
		GetDlgItem(IDC_MFCBTN_INFO_UP)->SetWindowText("Previous Page");
		GetDlgItem(IDC_MFCBTN_INFO_NEXT)->SetWindowText("Next Page");
		GetDlgItem(IDC_MFCBTN_REGISTER)->SetWindowText("Register");
		GetDlgItem(IDC_MFCBTN_CONFIG)->SetWindowText("Config");
		GetDlgItem(IDC_MFCBTN_RECHARGE)->SetWindowText("Recharge");
		GetDlgItem(IDC_MFCBTN_WITHDRAW)->SetWindowText("Withdraw");
		*/
	}

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[6]  = {
		{theApp.m_bChinese ? "企业名称" : "Company name" ,      150},
		{theApp.m_bChinese ? "账号地址" : "Account address",       250},
		{theApp.m_bChinese ? "每次转出限额" : "Each limit" ,      100},
		{theApp.m_bChinese ? "每日转出限额" : "Daily limits" ,      100}, 
		{theApp.m_bChinese ? "今日余额" : "Today balance" ,      100},
		{theApp.m_bChinese ? "总余额" : "Total balance", 100}
	};
	m_lstInfo.SetBkColor(RGB(255,255,255));       
	m_lstInfo.SetRowHeigt(23);               
	m_lstInfo.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstInfo.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstInfo.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstInfo.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <6; i++  ) {
		m_lstInfo.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	if(!theApp.m_bIsAdmin)
	{
		m_btnConfig.ShowWindow(SW_HIDE);
		m_btnRecharge.ShowWindow(SW_HIDE);
		m_btnRegister.ShowWindow(SW_HIDE);
	}

	GetHeight();
	RefreshListCtrl();

	//GetEntInfo();
	GetAllInfo();

	for(int i = 0; i < NUM_IN_PAGE; i++)
	{
		if(i  < m_VecListData.size())
		{
			ShowListNode(&m_VecListData[i]);
		}
	}

	for (int i=0;i<m_VecListData.size();++i)
	{
		if (strlen(m_VecListData[i].strEntName.c_str()))
			++m_uSaveNum;
		else
			break;
	}

	if (m_uSaveNum%NUM_IN_PAGE)
		m_nTotalPage = m_uSaveNum / NUM_IN_PAGE+1;
	else
		m_nTotalPage = m_uSaveNum / NUM_IN_PAGE;

	//ShowPageContent();
	int s = m_listapp.size();

	string temp;
	temp =strprintf(theApp.m_bChinese ?  "%s%d页" : "%s %d pages",theApp.m_bChinese ? _T("共") : _T("total") ,m_nTotalPage );
	m_staPage.SetWindowText(temp.c_str());

	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_UP_PROGRESS ) ;
	SetTimer(TIMER_FRESH, 60000, NULL);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CCompanyInfoDlg::RefreshListCtrl()
{
	string strCommand;
	strCommand = strprintf("%s",_T("listapp false"));
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return TRUE;
	}

	TRACE(root.toStyledString().c_str());
	ReadListApp(root);

	GetEntInfo();
	return TRUE;
}

void CCompanyInfoDlg::GetAllInfo()
{
	string strCommand;
	Json::Value root; 
	map<string,LIST_APP_DATA>::iterator it;
	int i = 0;


	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{

		string str = it->second.entName.c_str();

		string strEntName = it->second.entName;
		int pos = strEntName.find("@");
		if(pos == string::npos)
		{
			continue;
		}


		string strCompanyName = strEntName.substr(0, pos);


		string strValue;
		string tmp;

		strCommand = strprintf("%s %s",_T("getaccountinfo"), it->second.scriptId.c_str()); 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("InsertTransaction rpccmd registerapptx error");

			continue;
		}

		ULONGLONG balance;
		balance = root["Balance"].asUInt64();

		it->second.fBalance = balance *1.0/COIN;

		CString strBalance;
		strBalance.Format("%0.2f", (it->second.fBalance));
		//m_lstInfo.SetItemText(i, 5, strBalance);

		TRACE(root.toStyledString().c_str());

		strCommand = strprintf("%s %s %s",_T("getscriptdata"), it->second.scriptId.c_str(), "636f6e666967");
		Json::Value root; 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("InsertTransaction rpccmd registerapptx error");
			continue;
		}

		TRACE(root.toStyledString().c_str());

		strValue = root["value"].asString();
		tmp = strValue.substr(0, 16);

		ULONGLONG ullByTime;
		Hex2ULL(tmp, ullByTime);
		it->second.fQuotaByTime = ullByTime *1.0/COIN;

		CString strByTime;
		strByTime.Format("%0.2f", (it->second.fQuotaByTime));
		//m_lstInfo.SetItemText(i, 2, strByTime);


		ULONGLONG ullByDay;
		tmp = strValue.substr(16, 16);
		Hex2ULL(tmp, ullByDay);
		it->second.fQuotaByDay = ullByDay *1.0/COIN;

		CString strByDay;
		strByDay.Format("%0.2f", (it->second.fQuotaByDay));
		//m_lstInfo.SetItemText(i, 3, strByDay);

		string strAddr;
		tmp = strValue.substr(32, 68);
		Hex2String(tmp, strAddr);
		it->second.entAddr = strAddr;
		//m_lstInfo.SetItemText(i, 1, strAddr.c_str());

		strCommand = strprintf("%s %s %s",_T("getscriptdata"), it->second.scriptId.c_str(), "746F74616C6461796D6F6E6579");
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			it->second.bWithdrawByTodayValid = false;
			TRACE("InsertTransaction rpccmd registerapptx error");
			//m_lstInfo.SetItemText(i, 4, strByDay);

			continue;
		}
		it->second.bWithdrawByTodayValid = true;

		TRACE(root.toStyledString().c_str());
		strValue = root["value"].asString();
		ULONG ulHeight = 0;
		tmp = strValue.substr(0, 8);
		Hex2UL(tmp, ulHeight);
		it->second.nHeight = (int)ulHeight;
		ULONGLONG ullWithdrawDay;
		tmp = strValue.substr(8, 16);
		Hex2ULL(tmp, ullWithdrawDay);

		if(theApp.m_nBlockTipHight - ulHeight < 1440)
			it->second.fWithdrawByToday = ullWithdrawDay *1.0/COIN;
		else
			it->second.fWithdrawByToday = 0;

		//add by lbf 验证
		CString strLeftByDay;
		if(ullByDay < ullWithdrawDay)
		{
			strLeftByDay.Format("%0.2f", 0.00);
		}
		else
		{
			strLeftByDay.Format("%0.2f", ((ullByDay - ullWithdrawDay)*1.0/COIN));
		}

		//m_lstInfo.SetItemText(i, 4, strLeftByDay); 

	}
}

BOOL CCompanyInfoDlg::GetEntAddr(string strScriptId, string& strEntAddr)
{
	string strCommand = strprintf("%s %s %s",_T("getscriptdata"), strScriptId.c_str(), "636f6e666967");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return FALSE;
	}

	TRACE(root.toStyledString().c_str());

	string strValue = root["value"].asString();

	string strAddr;
	string tmp = strValue.substr(32, 68);
	Hex2String(tmp, strAddr);

	strEntAddr = strAddr;
	return TRUE;
}

void CCompanyInfoDlg::GetEntInfo()
{
	//m_lstInfo.DeleteAllItems();
	m_VecListData.clear();
	string strCommand;
	Json::Value root; 
	map<string,LIST_APP_DATA>::iterator it;
	int i = 0;

	int nShowType = ((CKoalaDlg*)AfxGetMainWnd())->JudgeType();
	if(0 == nShowType || 1 == nShowType || 3 == nShowType)
		return;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		LIST_DATA lstData;
		string str = it->second.entName.c_str();
		//m_lstInfo.InsertItem(i, it->second.entName.c_str());
		//m_lstInfo.SetItemData(i, (DWORD_PTR)(&it->second));
		
		string strEntName = it->second.entName;
		int pos = strEntName.find("@");
		if(pos == string::npos)
		{
			continue;
		}

		if(4 == nShowType || 5 == nShowType)
		{
			string strAdminAddr = strEntName.substr(pos + 1, strEntName.length() - pos - 1);
			if(strAdminAddr != theApp.m_strCurAdminAddress)
			{
				if(4 == nShowType)
				{
					string strEntAddr;
					BOOL bRet = GetEntAddr(it->second.scriptId, strEntAddr);
					if(!bRet)
					{
						continue;
					}

					if(strEntAddr != theApp.m_strCurEntAddress)
						continue;
				}
				else
				{
					continue;
				}

			}
		}


		string strCompanyName = strEntName.substr(0, pos);
		lstData.strEntName = strCompanyName;
		lstData.scriptId = it->second.scriptId;

		string strValue;
		string tmp;

		strCommand = strprintf("%s %s",_T("getaccountinfo"), it->second.scriptId.c_str()); 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("InsertTransaction rpccmd registerapptx error");
			if(4 == nShowType || 5 == nShowType)
			{
				m_VecListData.push_back(lstData);
			}
			continue;
		}

		ULONGLONG balance;
		balance = root["Balance"].asUInt64();

		it->second.fBalance = balance *1.0/COIN;

		CString strBalance;
		strBalance.Format("%0.2f", (it->second.fBalance));
		//m_lstInfo.SetItemText(i, 5, strBalance);
		lstData.strSumBalance = strBalance;

		TRACE(root.toStyledString().c_str());

		strCommand = strprintf("%s %s %s",_T("getscriptdata"), it->second.scriptId.c_str(), "636f6e666967");
		Json::Value root; 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("InsertTransaction rpccmd registerapptx error");
			if(4 == nShowType || 5 == nShowType)
			{
				m_VecListData.push_back(lstData);
			}
			continue;
		}

		TRACE(root.toStyledString().c_str());

		strValue = root["value"].asString();
		tmp = strValue.substr(0, 16);

		ULONGLONG ullByTime;
		Hex2ULL(tmp, ullByTime);
		it->second.fQuotaByTime = ullByTime *1.0/COIN;

		CString strByTime;
		strByTime.Format("%0.2f", (it->second.fQuotaByTime));
		//m_lstInfo.SetItemText(i, 2, strByTime);
		lstData.strOutQuotaByTime = strByTime;
		

		ULONGLONG ullByDay;
		tmp = strValue.substr(16, 16);
		Hex2ULL(tmp, ullByDay);
		it->second.fQuotaByDay = ullByDay *1.0/COIN;

		CString strByDay;
		strByDay.Format("%0.2f", (it->second.fQuotaByDay));
		//m_lstInfo.SetItemText(i, 3, strByDay);
		lstData.strOutQuotaByDay = strByDay;

		string strAddr;
		tmp = strValue.substr(32, 68);
		Hex2String(tmp, strAddr);
		it->second.entAddr = strAddr;
		//m_lstInfo.SetItemText(i, 1, strAddr.c_str());
		lstData.strEntAddr = strAddr;

		if(2 == nShowType || 6 == nShowType)
		{
			if(strAddr != theApp.m_strCurEntAddress)
				continue;
		}

		strCommand = strprintf("%s %s %s",_T("getscriptdata"), it->second.scriptId.c_str(), "746F74616C6461796D6F6E6579");
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			it->second.bWithdrawByTodayValid = false;
			TRACE("InsertTransaction rpccmd registerapptx error");
			//m_lstInfo.SetItemText(i, 4, strByDay);
			lstData.strBalance = strByDay; 
			m_VecListData.push_back(lstData);
			continue;
		}
		it->second.bWithdrawByTodayValid = true;

		TRACE(root.toStyledString().c_str());
		strValue = root["value"].asString();
		ULONG ulHeight = 0;
		tmp = strValue.substr(0, 8);
		Hex2UL(tmp, ulHeight);
		it->second.nHeight = (int)ulHeight;
		ULONGLONG ullWithdrawDay;
		tmp = strValue.substr(8, 16);
		Hex2ULL(tmp, ullWithdrawDay);

		int nDay1 = theApp.m_nBlockTipHight / 1440;
		int nDay2 = ulHeight / 1440;

		if(nDay1 == nDay2)
		{
			it->second.fWithdrawByToday = ullWithdrawDay *1.0/COIN;
		}
		else
		{
			it->second.fWithdrawByToday = 0;
			ullWithdrawDay = 0;
		}

		//add by lbf 验证
		CString strLeftByDay;
		if(ullByDay < ullWithdrawDay)
		{
			strLeftByDay.Format("%0.2f", 0.00);
		}
		else
		{
			strLeftByDay.Format("%0.2f", ((ullByDay - ullWithdrawDay)*1.0/COIN));
		}


		
		//m_lstInfo.SetItemText(i, 4, strLeftByDay);
		lstData.strBalance = strLeftByDay; 

		m_VecListData.push_back(lstData);
	}

	sort(m_VecListData.begin(), m_VecListData.end(), comp);
}

BOOL CCompanyInfoDlg::FindEnt(string strEntName)
{
	map<string,LIST_APP_DATA>::iterator it;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		if(strEntName == it->second.entName)
			return TRUE;
	}

	return FALSE;
}

void CCompanyInfoDlg::InsertEntName(string strEntName)
{
	int count = m_lstInfo.GetItemCount();
	m_lstInfo.InsertItem(count, strEntName.c_str());
	m_lstInfo.SetItemText(count, 5, "0");
}

BOOL CCompanyInfoDlg::FindEntAddr(string strEntAddr)
{
	map<string,LIST_APP_DATA>::iterator it;
	if(strEntAddr.empty())
		return FALSE;
	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		if(strEntAddr == it->second.entAddr)
			return TRUE;
	}

	return FALSE;
}

BOOL CCompanyInfoDlg::FindEntDataByAddr(string strEntAddr, LIST_APP_DATA& appData)
{
	map<string,LIST_APP_DATA>::iterator it;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		if(strEntAddr == it->second.entAddr)
		{
			appData.entAddr = it->second.entAddr;
			appData.entName = it->second.entName;
			appData.scriptId = it->second.scriptId;
			appData.fBalance = it->second.fBalance;
			appData.fQuotaByDay = it->second.fQuotaByDay;
			appData.fQuotaByTime = it->second.fQuotaByTime;
			appData.fWithdrawByToday = it->second.fWithdrawByToday;
			appData.nHeight = it->second.nHeight;
			appData.bWithdrawByTodayValid = it->second.bWithdrawByTodayValid;

			return TRUE;
		}
	}

	return FALSE;
}

void CCompanyInfoDlg::GetAllEntAddr(vector<string>& vecAddr)
{
	map<string,LIST_APP_DATA>::iterator it;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		vecAddr.push_back(it->second.entAddr);
	}
}

BOOL CCompanyInfoDlg::FindEntAddByScriptId(string strScriptId, string& entAddr)
{
	map<string,LIST_APP_DATA>::iterator it;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		if(strScriptId == it->second.scriptId)
		{
			entAddr = it->second.entAddr;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCompanyInfoDlg::FindAppDataByScriptId(string strScriptId, LIST_APP_DATA** app_data)
{
	map<string,LIST_APP_DATA>::iterator it;

	for(it = m_listapp.begin();it != m_listapp.end();++it)
	{
		if(strScriptId == it->second.scriptId)
		{
			*app_data = &(it->second);
			return TRUE;
		}
	}

	return FALSE;
}

void CCompanyInfoDlg::InsertEntCfgInfo(string strScriptId, string strAddr, double ullByTime, double ullByDay)
{
	map<string,LIST_APP_DATA>::iterator it = m_listapp.find(strScriptId);
	if(it == m_listapp.end()) {
		return;
	}

	m_listapp[strScriptId].entAddr = strAddr;
	m_listapp[strScriptId].fQuotaByTime = ullByTime;
	m_listapp[strScriptId].fQuotaByDay = ullByDay;

	LIST_APP_DATA* pData = NULL;
	POSITION pos = m_lstInfo.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstInfo.GetNextSelectedItem(pos) ;

		m_lstInfo.SetItemText(nRow, 1, strAddr.c_str());
		CString strByTime;
		strByTime.Format("%0.2f", ullByTime);
		
		m_lstInfo.SetItemText(nRow, 2, strByTime);

		CString strByDay;
		strByDay.Format("%0.2f", ullByDay);
		m_lstInfo.SetItemText(nRow, 3, strByDay);
	}
}

void CCompanyInfoDlg::OnBnClickedMfcbtnRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CEntRigeistDlg dlg;
	dlg.DoModal();
}

//HkuT5qqyohRLKe4cefow21CFA7utG7Q3qG
void CCompanyInfoDlg::OnBnClickedMfcbtnConfig()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelCount = m_lstInfo.GetSelectedCount();
	if(nSelCount <= 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请选择一个企业"),  _T("提示"), MB_OK);
		else 
			::MessageBoxEx(m_hWnd, _T("Please choose a business"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	BOOL bHaveConfig = FALSE;

	LIST_APP_DATA* pData = NULL;
	POSITION pos = m_lstInfo.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstInfo.GetNextSelectedItem(pos) ;
		pData = (LIST_APP_DATA*)m_lstInfo.GetItemData(nRow);
		if(pData == NULL)
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该企业登记还未生效"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The business registration has not yet entered into force"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}
		/*
		if(pData->fQuotaByTime != 0)
		{
			bHaveConfig = TRUE;
		}
		*/
		if(FindEntAddr(pData->entAddr.c_str()))
		{
			bHaveConfig = TRUE;
		}
	}

	CEntCfgDlg dlg;
	dlg.m_bHadConfig = bHaveConfig;
	if(bHaveConfig)
	{
		dlg.m_strScriptId = pData->scriptId;
		dlg.m_strAccountAddr = pData->entAddr.c_str();
		dlg.m_fQuotaByTime = pData->fQuotaByTime ;
		dlg.m_fQuotaByDay = pData->fQuotaByDay;
	}
	else
	{
		dlg.m_strScriptId = pData->scriptId;
	}

	dlg.DoModal();
}


void CCompanyInfoDlg::OnBnClickedMfcbtnRecharge()
{
	// TODO: 在此添加控件通知处理程序代码

	int nSelCount = m_lstInfo.GetSelectedCount();
	if(nSelCount <= 0)
	{
		CEntRechargeDlg dlg;
		dlg.m_strScriptId = "";
		dlg.m_strVerifyEntName = "";
		dlg.DoModal();
		return;
	}

	CString strEntName;

	LIST_APP_DATA* pData = NULL;
	POSITION pos = m_lstInfo.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstInfo.GetNextSelectedItem(pos) ;
		pData = (LIST_APP_DATA*)m_lstInfo.GetItemData(nRow);
		if(pData == NULL)
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该企业登记还未生效"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The business registration has not yet entered into force"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}
		/*
		if(pData->fQuotaByTime == 0)
		{
			::MessageBox(m_hWnd, _T("该企业配置还没生效，请先配置后再充值"),  _T("提示"), MB_OK);
			return;
		}
		*/
		if(FALSE == FindEntAddr(pData->entAddr.c_str()))
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该企业配置还没生效，请先配置后再充值"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The enterprise configuration has not yet entered into force, please configure and then recharge"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}

		strEntName = m_lstInfo.GetItemText(nRow, 0);
		if(strEntName.IsEmpty())
			return;
	}

	CEntRechargeDlg dlg;
	dlg.m_strInputScriptId = pData->scriptId.c_str();
	//dlg.m_strEntName = strEntName;

	/*m_strInputScriptId = m_strScriptId.c_str();
	m_strVerifyEntName = m_strEntName;*/
	dlg.DoModal();
}


void CCompanyInfoDlg::OnBnClickedMfcbtnWithdraw()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSelCount = m_lstInfo.GetSelectedCount();
	if(nSelCount <= 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请选择一个企业"),  _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please choose a business"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	LIST_APP_DATA* pData = NULL;
	POSITION pos = m_lstInfo.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_lstInfo.GetNextSelectedItem(pos) ;
		pData = (LIST_APP_DATA*)m_lstInfo.GetItemData(nRow);
		if(pData == NULL)
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该企业登记还未生效"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The business registration has not yet entered into force"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}
		/*
		if(pData->fQuotaByTime == 0)
		{
			::MessageBox(m_hWnd, _T("该企业配置还没生效，请先配置后再充值"),  _T("提示"), MB_OK);
			return;
		}
		*/
		if(FALSE == FindEntAddr(pData->entAddr.c_str()))
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该企业配置还没生效，请先配置后再充值"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The enterprise configuration has not yet entered into force, please configure and then recharge"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}

		if(pData->fBalance == 0)
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("余额为0，请先充值"),  _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("Balance is 0, please recharge first"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}
	}

	CEntWithdrawDlg dlg;
	dlg.m_strScriptId = pData->scriptId;
	dlg.DoModal();
}


void CCompanyInfoDlg::OnBnClickedMfcbtnInfoUp()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_nCurrentPage==0) 
	{
		if(theApp.m_bChinese)
			MessageBox("已是第一页","提示");
		else
			::MessageBoxEx(m_hWnd, _T("It's the first page"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	int i = 0;
	int num = 0;

	//保存选中的项
	int nCount = m_lstInfo.GetItemCount();

	--m_nCurrentPage;
	m_nSerialNum -= (nCount + NUM_IN_PAGE);

	//删除List中显示的所有项
	m_lstInfo.DeleteAllItems();

	//在List中插入新的项
	num = m_nCurrentPage*NUM_IN_PAGE;
	for (i=num; i<num+NUM_IN_PAGE; ++i)
	{
		if(i  < m_VecListData.size())
		{
		ShowListNode(&m_VecListData[i]);
		}
	}

	//显示之前是否已经选中过的项
}

void CCompanyInfoDlg::ShowListNode(LIST_DATA *node)
{

	int		itemno;
	itemno = m_lstInfo.InsertItem(m_nSerialNum,node->strEntName.c_str() );

	LIST_APP_DATA* app_data = NULL;

	BOOL ret = FindAppDataByScriptId(node->scriptId, &app_data);
	if(ret)
	{
		m_lstInfo.SetItemData(itemno, (DWORD_PTR)app_data);
	}
	//m_lstInfo.SetItemData();
	//m_lstInfo.SetItemText(itemno, 1, node->strEntName.c_str());

	m_lstInfo.SetItemText(itemno, 1, node->strEntAddr.c_str());
	m_lstInfo.SetItemText(itemno, 2, node->strOutQuotaByTime.c_str());
	m_lstInfo.SetItemText(itemno, 3, node->strOutQuotaByDay.c_str());
	m_lstInfo.SetItemText(itemno, 4, node->strBalance.c_str());
	m_lstInfo.SetItemText(itemno, 5, node->strSumBalance.c_str());
	++m_nSerialNum;				  
}



void CCompanyInfoDlg::OnBnClickedMfcbtnInfoNext()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( (m_nCurrentPage+1)>=m_nTotalPage) 
	{
		if(theApp.m_bChinese)
			MessageBox("达到最后一页","提示");
		else
			::MessageBoxEx(m_hWnd, _T("Reach the last page"),  _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	int i = 0;
	int num = 0;
	//保存选中的项
	int nCount = m_lstInfo.GetItemCount();

	++m_nCurrentPage;

	//删除List中显示的所有项
	m_lstInfo.DeleteAllItems();

	//在List中插入新的项
	num = m_nCurrentPage*NUM_IN_PAGE;
	for (i=num; i<(num + NUM_IN_PAGE) ; ++i)
	{
		if(i == m_uSaveNum)
		{
			return;
		}
		if(i  < m_VecListData.size())
		{
		ShowListNode(&m_VecListData[i]);
		}
		
	}

	//显示 以前已经选中过的项
	nCount = m_lstInfo.GetItemCount();
	
}

/*
void CCompanyInfoDlg::AddListNode(LIST_DATA *node)
{
	//转到最后一页
	int i = 0;

	//重新获取总页面数
	if (m_uSaveNum%NUM_IN_PAGE)
		m_nTotalPage = m_uSaveNum/NUM_IN_PAGE+1;
	else
		m_nTotalPage = m_uSaveNum/NUM_IN_PAGE;

	m_nCurrentPage = m_nTotalPage - 1;

	//如果最后一页是满的，就重新增加新的一页
	m_lstInfo.DeleteAllItems();
	if( (m_uSaveNum-m_nCurrentPage*NUM_IN_PAGE) >= NUM_IN_PAGE)
	{
		ShowListNode(node);
		++m_nTotalPage;
		++m_nCurrentPage;
	}
	else
	{
		m_nSerialNum = m_nCurrentPage*NUM_IN_PAGE;//重置序号
		for(i=m_nCurrentPage*NUM_IN_PAGE; i<m_uSaveNum+1; ++i )
		{
			ShowListNode(&m_VecListData[i]);
		}

	}


}
*/

void CCompanyInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RefreshListCtrl();
	//sort(m_VecListData.begin(), m_VecListData.end());
	
	m_lstInfo.DeleteAllItems();
	for(int i = 0; i < NUM_IN_PAGE; i++)
	{
		if(i  < m_VecListData.size())
		{
		ShowListNode(&m_VecListData[i]);
		}
	}

	m_uSaveNum = 0;
	m_nCurrentPage = 0;
	for (int i=0;i<m_VecListData.size();++i)
	{
		if (strlen(m_VecListData[i].strEntName.c_str()))
			++m_uSaveNum;
		else
			break;
	}

	if (m_uSaveNum%NUM_IN_PAGE)
		m_nTotalPage = m_uSaveNum / NUM_IN_PAGE+1;
	else
		m_nTotalPage = m_uSaveNum / NUM_IN_PAGE;

	//ShowPageContent();
	int s = m_listapp.size();

	string temp;
	temp =strprintf(theApp.m_bChinese ? "%s%d页" : "%s %d pages",theApp.m_bChinese ? _T("共") : _T("total") ,m_nTotalPage );
	m_staPage.SetWindowText(temp.c_str());

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CCompanyInfoDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CCompanyInfoDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	if(theApp.m_bIsSyncBlock)
	{
		OnTimer(0);

		COverviewDlg* pDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pOverviewDlg;
		if(pDlg)
		{
			pDlg->SetEntInfoEx();
		}
	}
	return 0;
}


void CCompanyInfoDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(-1 != pNMItemActivate->iItem) 
	{  
		int nRow = pNMItemActivate->iItem;

		
		LIST_APP_DATA* pData = NULL;
		pData = (LIST_APP_DATA*)m_lstInfo.GetItemData(nRow);

		CEntInfoDlg dlg;

		dlg.m_strEntInfo += strprintf("%s: %s\r\n\r\n",theApp.m_bChinese ? _T("企业名称") : _T("Company Name"),m_lstInfo.GetItemText(nRow, 0));
		dlg.m_strEntInfo += strprintf("%s: %s\r\n\r\n",theApp.m_bChinese ? _T("账户地址") : _T("Account address"),pData->entAddr);
		dlg.m_strEntInfo += strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("企业账号ID") : _T("Enterprise account ID"),pData->scriptId);
		dlg.m_strEntInfo += strprintf("%s:   %0.2lf\r\n\r\n",theApp.m_bChinese ? _T("每次转出限额") : _T("Each out of the limit"),pData->fQuotaByTime);
		dlg.m_strEntInfo += strprintf("%s:   %0.2lf\r\n\r\n",theApp.m_bChinese ? _T("每日转出限额") : _T("Daily out of limits"),pData->fQuotaByDay);

		dlg.m_strEntInfo += strprintf("%s:   %0.2lf\r\n\r\n",theApp.m_bChinese ? _T("今日余额") : _T("Balance today"),m_lstInfo.GetItemText(nRow, 4));
		dlg.m_strEntInfo += strprintf("%s:   %0.2lf\r\n\r\n",theApp.m_bChinese ? _T("总余额") : _T("Total balance"),pData->fBalance);
		
		dlg.DoModal();

		int i = 0;
	}  
	*pResult = 0;
}
