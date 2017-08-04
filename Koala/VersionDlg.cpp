// VersionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "VersionDlg.h"
#include "afxdialogex.h"


// CVersionDlg 对话框

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVersionDlg::IDD, pParent)
{

}

CVersionDlg::~CVersionDlg()
{
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_VERSION, m_richeditVer);
}


BEGIN_MESSAGE_MAP(CVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVersionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CVersionDlg 消息处理程序


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		SetWindowText("Version of Wallet");
	}

	// TODO:  在此添加额外的初始化

	m_richeditVer.SetBackgroundColor(false,GetSysColor(COLOR_BTNFACE));
	/*
	CStdioFile file;

	//打开文件
	CString strText;
	TCHAR exepath[MAX_PATH]={0};
	::GetModuleFileName(NULL,exepath,MAX_PATH);
	strText = exepath;
	int nPos = strText.ReverseFind( _T('\\') );   
	strText=strText.Left(nPos+1); 


	strText.Format(strText + "help/version.txt");	

	if(!file.Open(strText, CFile::modeReadWrite))
	{
		MessageBox( _T("文档为空"),_T("提示"));
		return FALSE;
	}

	CString txt;

	CString tmpStr;

	//从文件中读取数据

	while(file.ReadString(tmpStr))
	{
		txt += tmpStr + "\n";
	}

	file.Close();
	*/

	//显示文本

	string strVer = GetUIVersion();

	CString txt;
	txt.Format(theApp.m_bChinese ? "版本：v%s\n\nCopyright (C) 2017 " : "Version：%s\n\nCopyright (C) 2017 ", strVer.c_str());

	GetDlgItem(IDC_RICHEDIT_VERSION)->SetWindowText(txt);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

string CVersionDlg::GetUIVersion()
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


void CVersionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
