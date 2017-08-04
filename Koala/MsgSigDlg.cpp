// MsgSigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "MsgSigDlg.h"
#include "afxdialogex.h"


// CMsgSigDlg �Ի���

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


// CMsgSigDlg ��Ϣ�������


BOOL CMsgSigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		SetWindowText("Sign/Verify Message");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rc;
	GetClientRect(&rc);
	//m_wndTab.Create (CMFCTabCtrl::STYLE_3D_ROUNDED, rc, this, 1, CMFCTabCtrl::LOCATION_TOP);
	m_wndTab.Create(TCS_UP|WS_CHILD|WS_VISIBLE,rc, this, 1);
	
	//m_dlgSign.Create(IDD_M_SIGN_DLG, &m_wndTab);
	//m_dlgVerify.Create(IDD_M_CONFIRM_DLG, &m_wndTab);

	//m_wndTab.AddTab (&m_dlgSign, _T("ǩ����Ϣ"));
	//m_wndTab.AddTab (&m_dlgVerify, _T("��֤��Ϣ"));
	m_wndTab.AddPage(RUNTIME_CLASS(CSignDlg),IDD_M_SIGN_DLG, theApp.m_bChinese ? "ǩ����Ϣ" : "Signature message");
	m_wndTab.AddPage(RUNTIME_CLASS(CVerifyDlg),IDD_M_CONFIRM_DLG, theApp.m_bChinese ? "��֤��Ϣ" : "Verify the message");

	CenterWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



BOOL CMsgSigDlg::PreTranslateMessage(MSG* pMsg)
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
