// ToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "ToolDlg.h"
#include "afxdialogex.h"

// CToolDlg �Ի���

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


// CToolDlg ��Ϣ�������
BOOL CToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		SetWindowText("Tools");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ToolTab.InsertItem(0, theApp.m_bChinese ? "��Ϣ" : "Information");
	m_ToolTab.InsertItem(1, theApp.m_bChinese ? "�ڵ�" : "Peerlist");
	m_ToolTab.InsertItem(2, theApp.m_bChinese ? "�����д���": "Command line");

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
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CToolDlg::OnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN) 
	{  

		switch(pMsg->wParam) 
		{  

		case VK_RETURN: //�س�   
			return TRUE;  

		case VK_ESCAPE: //ESC  
			return TRUE;  

		}  

	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}
