// MsgSigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "MsgSigDlg.h"
#include "afxdialogex.h"


// CMsgSigDlg 对话框

IMPLEMENT_DYNAMIC(CMsgSigDlg, CDialogEx)

CMsgSigDlg::CMsgSigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMsgSigDlg::IDD, pParent)
{

}

CMsgSigDlg::~CMsgSigDlg()
{
}

void CMsgSigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgSigDlg, CDialogEx)
END_MESSAGE_MAP()


// CMsgSigDlg 消息处理程序


BOOL CMsgSigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		SetWindowText("Sign/Verify Message");
	}

	// TODO:  在此添加额外的初始化
	CRect rc;
	GetClientRect(&rc);
	//m_wndTab.Create (CMFCTabCtrl::STYLE_3D_ROUNDED, rc, this, 1, CMFCTabCtrl::LOCATION_TOP);
	m_wndTab.Create(TCS_UP|WS_CHILD|WS_VISIBLE,rc, this, 1);
	
	//m_dlgSign.Create(IDD_M_SIGN_DLG, &m_wndTab);
	//m_dlgVerify.Create(IDD_M_CONFIRM_DLG, &m_wndTab);

	//m_wndTab.AddTab (&m_dlgSign, _T("签名消息"));
	//m_wndTab.AddTab (&m_dlgVerify, _T("验证消息"));
	m_wndTab.AddPage(RUNTIME_CLASS(CSignDlg),IDD_M_SIGN_DLG, theApp.m_bChinese ? "签名消息" : "Signature message");
	m_wndTab.AddPage(RUNTIME_CLASS(CVerifyDlg),IDD_M_CONFIRM_DLG, theApp.m_bChinese ? "验证消息" : "Verify the message");

	CenterWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



BOOL CMsgSigDlg::PreTranslateMessage(MSG* pMsg)
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
