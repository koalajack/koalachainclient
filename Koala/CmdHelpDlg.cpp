// CmdHelpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "CmdHelpDlg.h"
#include "afxdialogex.h"


// CCmdHelpDlg 对话框

IMPLEMENT_DYNAMIC(CCmdHelpDlg, CDialogEx)

CCmdHelpDlg::CCmdHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCmdHelpDlg::IDD, pParent)
{
	
}

CCmdHelpDlg::~CCmdHelpDlg()
{
}

void CCmdHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EXPLORER1, m_WebView);
}


BEGIN_MESSAGE_MAP(CCmdHelpDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCmdHelpDlg 消息处理程序


BOOL CCmdHelpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		SetWindowText("Command line help document");
	}

	CString strText;
	TCHAR exepath[MAX_PATH]={0};
	::GetModuleFileName(NULL,exepath,MAX_PATH);
	strText = exepath;
	int nPos = strText.ReverseFind( _T('\\') );   
	strText=strText.Left(nPos+1); 


	strText.Format(strText + "help\\CmdHelp.html");	

	CRect rc;
	GetClientRect(rc);
	m_WebView.MoveWindow(0, 0, rc.Width(), rc.Height());
	m_WebView.Navigate(strText, NULL, NULL, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}






void CCmdHelpDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(::IsWindow(m_WebView.m_hWnd))
	{
		m_WebView.MoveWindow(0, 0, cx, cy);
	}
}
