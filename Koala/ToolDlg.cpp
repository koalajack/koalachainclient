// ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ToolDlg.h"
#include "afxdialogex.h"

// CToolDlg 对话框

IMPLEMENT_DYNAMIC(CToolDlg, CDialogEx)

CToolDlg::CToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolDlg::IDD, pParent)
{
	m_pActiveWnd = NULL;
	m_nActiveIndex = 0;
}

CToolDlg::~CToolDlg()
{

}

void CToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_TOOL, m_ToolTab);
}


BEGIN_MESSAGE_MAP(CToolDlg, CDialogEx)

	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TOOL, &CToolDlg::OnSelchangeTabTool)
END_MESSAGE_MAP()


// CToolDlg 消息处理程序
BOOL CToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		SetWindowText("Tools");
	}

	// TODO:  在此添加额外的初始化
	m_ToolTab.InsertItem(0, theApp.m_bChinese ? "信息" : "Information");
	m_ToolTab.InsertItem(1, theApp.m_bChinese ? "节点" : "Peerlist");
	m_ToolTab.InsertItem(2, theApp.m_bChinese ? "命令行窗口": "Command line");

	m_ToolTab.SetCurSel(m_nActiveIndex);

	if(0 == m_ToolTab.GetCurSel())
	{
		ShowToolInfoDlg();
	}
	if(1 == m_ToolTab.GetCurSel())
	{
		ShowToolPeerlistDlg();
	}
	if(2 == m_ToolTab.GetCurSel())
	{
		ShowToolCmd();
	}

	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CToolDlg::SetActiveIndex(int nIndex)
{
	m_nActiveIndex = nIndex;
}

void CToolDlg::ShowToolCmd()
{
	if(NULL == m_ToolCmd)
	{
		m_ToolCmd.Create(IDD_TOOLCMD_DLG, &m_ToolTab);
		m_ToolCmd.ModifyStyle(DS_CONTROL, 0); 
	}

	if(m_pActiveWnd)
	{
		m_pActiveWnd->ShowWindow(SW_HIDE);
	}
	m_pActiveWnd = &m_ToolCmd;	

	ShowActiveWnd();
}

void CToolDlg::ShowToolInfoDlg()
{
	if(NULL == m_ToolInfo)
	{
		m_ToolInfo.Create(IDD_TOOLINFO_DLG, &m_ToolTab);
		m_ToolInfo.ModifyStyle(DS_CONTROL, 0); 
	}

	if(m_pActiveWnd)
	{
		m_pActiveWnd->ShowWindow(SW_HIDE);
	}
	m_pActiveWnd = &m_ToolInfo;	

	ShowActiveWnd();
}

void CToolDlg::ShowActiveWnd()
{
	if(m_pActiveWnd)
	{
		RECT rcClient;
		m_ToolTab.GetClientRect(&rcClient);
		rcClient.top += 30;
		rcClient.bottom -= 2;
		rcClient.left += 2;
		rcClient.right -= 2;
		m_pActiveWnd->ShowWindow(SW_SHOW);
		m_pActiveWnd->MoveWindow(&rcClient);
	}
}

void CToolDlg::ShowToolPeerlistDlg()
{
	if(NULL == m_ToolPeerlist)
	{
		m_ToolPeerlist.Create(IDD_TOOLPEERLIST_DLG, &m_ToolTab);
		m_ToolPeerlist.ModifyStyle(DS_CONTROL, 0); 
	}

	if(m_pActiveWnd)
	{
		m_pActiveWnd->ShowWindow(SW_HIDE);
	}
	m_pActiveWnd = &m_ToolPeerlist;	

	ShowActiveWnd();
}

void CToolDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


void CToolDlg::OnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int iSel = m_ToolTab.GetCurSel();
	if(0 == iSel)
	{
		ShowToolInfoDlg();	
	}
	if(1 == iSel)
	{
		ShowToolPeerlistDlg();
	}
	if(2 == iSel)
	{
		ShowToolCmd();
	}
	*pResult = 0;
}


BOOL CToolDlg::PreTranslateMessage(MSG* pMsg)
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
