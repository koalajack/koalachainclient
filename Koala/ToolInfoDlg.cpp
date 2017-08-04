// ToolInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ToolInfoDlg.h"
#include "afxdialogex.h"

#define TIMER_INFO 1001



// CToolInfoDlg 对话框

IMPLEMENT_DYNCREATE(CToolInfoDlg, CDialogEx)

CToolInfoDlg::CToolInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolInfoDlg::IDD, pParent)
{
	m_vecMainNodes.empty();
}

CToolInfoDlg::~CToolInfoDlg()
{
}

void CToolInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CToolInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CToolInfoDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CToolInfoDlg 消息处理程序


BOOL CToolInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC900)->SetWindowText("Common");
		GetDlgItem(IDC_STATIC901)->SetWindowText("Client name:");
		GetDlgItem(IDC_STATIC902)->SetWindowText("Client Version:");
		GetDlgItem(IDC_STATIC903)->SetWindowText("Create time:");
		GetDlgItem(IDC_STATIC904)->SetWindowText("Launch time:");
		GetDlgItem(IDC_STATIC905)->SetWindowText("Network:");
		GetDlgItem(IDC_STATIC906)->SetWindowText("Name:");
		GetDlgItem(IDC_STATIC907)->SetWindowText("Connection Count:");
		GetDlgItem(IDC_STATIC908)->SetWindowText("Main Nodes:");
		GetDlgItem(IDC_STATIC909)->SetWindowText("Data link");
		GetDlgItem(IDC_STATIC910)->SetWindowText("Blocks:");
		GetDlgItem(IDC_STATIC911)->SetWindowText("Previous block time:");
		GetDlgItem(IDC_STATIC912)->SetWindowText("Log");
		GetDlgItem(IDC_BUTTON1)->SetWindowText("Open");
	}

	// TODO:  在此添加额外的初始化
	GetMainNode();				//获取所有主节点的信息

	SYSTEMTIME st;
	CString strDate;
	CString strTime;
	GetLocalTime(&st);
	strDate.Format("%04d-%02d-%02d",st.wYear,st.wMonth,st.wDay);
	strTime.Format("%02d:%02d:%02d",st.wHour,st.wMinute,st.wSecond);
	
	m_strDateTime = strDate + " " + strTime;

	string strcom =strprintf("%s","getinfo");
	string strSendData,strretshow;
	RPCCommandToJson(strcom,strSendData);
	CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);
	//ShowJson(strcom,strretshow);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strretshow, root)) 
		return FALSE;
	GetInfo();
	ShowInfo();


	SetTimer(TIMER_INFO, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CToolInfoDlg::GetMainNode()
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
		int pos = strFile.find("listMainNode");
		if(pos >= 0)
		{
			Json::Value addrValue = root["listMainNode"];
			for(int i = 0; i < (int)addrValue.size(); i++)
			{
				Json::Value obj = addrValue[i];
				m_vecMainNodes.push_back(obj["MainNode"].asString());
			}
		}
	}

}

void CToolInfoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//::ShellExecute(NULL, _T("open"), _T("d:\\debug.log"), NULL, NULL, SW_SHOWNORMAL);
	//TCHAR *path = new TCHAR[MAX_PATH];
	//GetModuleFileName(NULL,path,MAX_PATH);

	CString strAppName;
	::GetModuleFileName(NULL, strAppName.GetBuffer(_MAX_PATH),_MAX_PATH);
	strAppName.ReleaseBuffer();

	int nPos = strAppName.ReverseFind('\\');
	strAppName = strAppName.Left(nPos+1);

	if (theApp.m_nNetWorkType == MAIN)
	{
		strAppName.Format(strAppName + "main\\INFO.log");
	}else if (theApp.m_nNetWorkType == REGTEST)
	{
		strAppName.Format(strAppName + "regtest\\INFO.log");
	}else if (theApp.m_nNetWorkType == TESTNET)
	{
		strAppName.Format(strAppName + "testnet\\INFO.log");
	}
	

	::WinExec("notepad.exe " + strAppName,SW_SHOW);
}

string GetUIVersion()
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
			strRet = strprintf("v%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strRet;
}

void CToolInfoDlg::ShowInfo()
{
	GetDlgItem(IDC_CLIENTNAME)->SetWindowText(theApp.m_bChinese ? "考拉链" : "Koala Chain");
	GetDlgItem(IDC_CON_NUM)->SetWindowText(m_info.strConCount.c_str());
	GetDlgItem(IDC_CURBLOCK_NUM)->SetWindowText(m_info.strCurblockNum.c_str());
	string strVer = GetUIVersion();
	GetDlgItem(IDC_CLIENT_VER)->SetWindowText(strVer.c_str());
	GetDlgItem(IDC_START_TIME)->SetWindowText(m_strDateTime);
	GetDlgItem(IDC_CREATE_TIME)->SetWindowText(m_info.strInitTime.c_str());
	GetDlgItem(IDC_LASTBLOCK_TIME)->SetWindowText(m_info.strLastBlockTime.c_str());
	GetDlgItem(IDC_MAINNODE)->SetWindowText(m_info.strMNodes.c_str());
	GetDlgItem(IDC_NAME)->SetWindowText("main");



}

void CToolInfoDlg::long2time(ULONGLONG nTime, CString& strTime)
{ 
	CTime objTime(nTime);    
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),
		objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond());
}

void CToolInfoDlg::GetInfo()
{
	string strcom =strprintf("%s","getinfo");
	string strSendData,strretshow;
	RPCCommandToJson(strcom,strSendData);
	CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);
	//ShowJson(strcom,strretshow);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strretshow, root)) 
		return ;

	//m_info.strClientName;
	m_info.strClientVer = root["version"].asString();
	m_info.strConCount = root["connections"].asString();
	m_info.strCurblockNum = root["blocks"].asString();

	m_info.strInitTime = root["fullversion"].asString();
	{
		int iFind = m_info.strInitTime.find_first_of('(');
		int iEnd = m_info.strInitTime.find_last_of(' ');
		if (iFind == string::npos)
		{
			m_info.strInitTime = "";
		}
		if(iEnd == string::npos)
		{
			m_info.strInitTime = "";
		}
		m_info.strInitTime = m_info.strInitTime.substr(iFind + 1, 19);

	}

	//m_info.StartTime = root[];
 
	//m_info.strName;
	
	{
		CString strCommand;
		strCommand.Format("getblock %d", root["blocks"].asInt() - 1);
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand.GetBuffer(),strretshow))
		{
			TRACE("InsertTransaction rpccmd registerapptx error");
			return ;
		}
		Json::Value root; 
		if (!reader.parse(strretshow, root)) 
			return ;

		CString str;
		
		long2time(root["time"].asUInt64(), str);

		m_info.strLastBlockTime = str;
		
	}

	{
		int MNodes = 0;
		string strcom =strprintf("%s","getpeerinfo");
		string strSendData,strretshow;
		RPCCommandToJson(strcom,strSendData);
		CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);

		TRACE("=======%s\n", strretshow);
		//ShowJson(strcom,strretshow);

		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strretshow, root)) 
			return ;

		int size = root.size();

		for ( int index =0; index < size; ++index )
		{

			Json::Value arr = root[index];
			string str = arr["addr"].asString();

			int iFind = str.find(':');
			if(iFind == string::npos)
			{
				return;
			}

			str = str.substr(0, iFind);

			vector<string>::iterator it;

			for(it  = m_vecMainNodes.begin(); it != m_vecMainNodes.end(); it++)
			{
				if(!strcmp(str.c_str(), (*it).c_str()))
				{
					++MNodes;
				}
			}
		}

		m_info.strMNodes = strprintf("%d", MNodes);

	}
	
}

void CToolInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetInfo();
	ShowInfo();
	CDialogEx::OnTimer(nIDEvent);
}


void CToolInfoDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CToolInfoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(TIMER_INFO);
	// TODO: 在此处添加消息处理程序代码
}
