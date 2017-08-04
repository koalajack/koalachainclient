// VersionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "VersionDlg.h"
#include "afxdialogex.h"


// CVersionDlg �Ի���

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


// CVersionDlg ��Ϣ�������


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		SetWindowText("Version of Wallet");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_richeditVer.SetBackgroundColor(false,GetSysColor(COLOR_BTNFACE));
	/*
	CStdioFile file;

	//���ļ�
	CString strText;
	TCHAR exepath[MAX_PATH]={0};
	::GetModuleFileName(NULL,exepath,MAX_PATH);
	strText = exepath;
	int nPos = strText.ReverseFind( _T('\\') );   
	strText=strText.Left(nPos+1); 


	strText.Format(strText + "help/version.txt");	

	if(!file.Open(strText, CFile::modeReadWrite))
	{
		MessageBox( _T("�ĵ�Ϊ��"),_T("��ʾ"));
		return FALSE;
	}

	CString txt;

	CString tmpStr;

	//���ļ��ж�ȡ����

	while(file.ReadString(tmpStr))
	{
		txt += tmpStr + "\n";
	}

	file.Close();
	*/

	//��ʾ�ı�

	string strVer = GetUIVersion();

	CString txt;
	txt.Format(theApp.m_bChinese ? "�汾��v%s\n\nCopyright (C) 2017 " : "Version��%s\n\nCopyright (C) 2017 ", strVer.c_str());

	GetDlgItem(IDC_RICHEDIT_VERSION)->SetWindowText(txt);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

string CVersionDlg::GetUIVersion()
{
	string strRet = "";
	char szAppFullPath[_MAX_PATH] = {0};

	GetModuleFileName(NULL,szAppFullPath,MAX_PATH);//�õ�����ģ�����ƣ�ȫ·��

	//��ȡ��ǰ�ļ��İ汾��Ϣ
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
			//��ȡ�汾��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
