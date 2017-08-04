// CmdHelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "CmdHelpDlg.h"
#include "afxdialogex.h"


// CCmdHelpDlg �Ի���

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


// CCmdHelpDlg ��Ϣ�������


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
	// �쳣: OCX ����ҳӦ���� FALSE
}






void CCmdHelpDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(::IsWindow(m_WebView.m_hWnd))
	{
		m_WebView.MoveWindow(0, 0, cx, cy);
	}
}
