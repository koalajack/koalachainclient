// SignDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "SignDlg.h"
#include "afxdialogex.h"


// CSignDlg �Ի���

IMPLEMENT_DYNCREATE(CSignDlg, CDialogEx)

CSignDlg::CSignDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignDlg::IDD, pParent)
	, m_strRecvAddr(_T(""))
	, m_strSignMessage(_T(""))
	, m_SignTip(_T(""))
{

}

CSignDlg::~CSignDlg()
{
}

void CSignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_SIGNMESSAGE, m_redtSign);
	//  DDX_Control(pDX, IDC_EDIT_SIGNADDR, m_editRecvAddr);
	DDX_Text(pDX, IDC_EDIT_SIGNADDR, m_strRecvAddr);
	DDX_Text(pDX, IDC_RICHEDIT_SIGNMESSAGE, m_strSignMessage);
	DDX_Text(pDX, IDC_EDIT_SIGNATURE, m_strSignature);
	//  DDX_Text(pDX, IDC_EDIT_SIGNATURE, m_editSignature);
	//  DDX_Control(pDX, IDC_EDIT_SIGNATURE, m_pEditSignature);
	DDX_Control(pDX, IDC_EDIT_SIGNADDR, m_editAddr);
	DDX_Control(pDX, IDC_EDIT_SIGNATURE, m_editSignature);
	//  DDX_Control(pDX, IDC_BTN_SELECTADDR, m_btnSelectAddr);
	DDX_Text(pDX, IDC_SIGNMESSAGERESULT, m_SignTip);
}


BEGIN_MESSAGE_MAP(CSignDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SELECTADDR, &CSignDlg::OnBnClickedBtnSelectaddr)
	ON_BN_CLICKED(IDC_BUTTON_M_SIGN, &CSignDlg::OnBnClickedButtonMSign)
	ON_BN_CLICKED(IDC_BUTTON_BOARDCOPY, &CSignDlg::OnBnClickedButtonBoardcopy)
	ON_BN_CLICKED(IDC_BUTTON_BOARDPASTE, &CSignDlg::OnBnClickedButtonBoardpaste)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CSignDlg::OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CSignDlg ��Ϣ�������


BOOL CSignDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC44)->SetWindowText("You can sign the message with your address to prove that you are the owner of the address. Be careful not to sign ambiguous messages to avoid phishing attacks. Please make sure that the content of the message accurately expresses your true wishes.");
		GetDlgItem(IDC_BTN_SELECTADDR)->SetWindowText("Select");
		GetDlgItem(IDC_BUTTON_BOARDPASTE)->SetWindowText("Paste");
		GetDlgItem(IDC_STATIC45)->SetWindowText("Sign");
		GetDlgItem(IDC_BUTTON_BOARDCOPY)->SetWindowText("Copy");
		GetDlgItem(IDC_BUTTON_M_SIGN)->SetWindowText("Message Sign");
		GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText("Clear");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CSignDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pWnd = GetFocus();
		if(pWnd != NULL && (pWnd->m_hWnd == m_redtSign.GetSafeHwnd()))
		{
			
		}
		else
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSignDlg::OnBnClickedBtnSelectaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSelectRecvAddr dlg;
	
	if(dlg.DoModal()==IDOK)    
	{    
		m_strRecvAddr = dlg.m_SelectRecvAddr;
	}    

	UpdateData(FALSE);

	
}


void CSignDlg::OnBnClickedButtonMSign()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (theApp.IsLockWallet())
	{
		return ;
	}

	
	string strCommand;
	strCommand = strprintf("%s %s %s","signmessage", m_strRecvAddr, m_strSignMessage);
	string strShowData;
	
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData))
	{
		TRACE("AcceptRePacketCommtRecord rpccmd gettxdetail error");
		return;
	}
	
	if(string::npos != strShowData.find("code"))
	{
		if(theApp.m_bChinese)
			MessageBox(strShowData.c_str(), theApp.m_bChinese ? "��ʾ" : "tip");
		else
			::MessageBoxEx(m_hWnd, strShowData.c_str(), theApp.m_bChinese ? "��ʾ" : "tip", MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	m_strSignature = strShowData.c_str();
	

	//SetDlgItemText(IDC_SIGNMESSAGERESULT, "��Ϣ��ǩ����");
	m_SignTip = theApp.m_bChinese ? "��Ϣ��ǩ����" : "The message is signed! ";

	UpdateData(FALSE);

}


void CSignDlg::OnBnClickedButtonBoardcopy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString strSignature;
	CString strShow;

	GetDlgItemText(IDC_EDIT_SIGNATURE, strSignature);
	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strSignature.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strSignature));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
		strShow = theApp.m_bChinese ? _T("��ַ�Ѹ��Ƶ�������") : _T("The address has been copied to the clipboard");

		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, strShow , theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, strShow , theApp.m_bChinese ? _T("��ʾ") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}


void CSignDlg::OnBnClickedButtonBoardpaste()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(OpenClipboard())     
	{   
		char * buffer = NULL;
		HANDLE   hData   =   GetClipboardData(   CF_TEXT   );   
		buffer   =   (char*)GlobalLock(   hData   );


		GetDlgItem(IDC_EDIT_SIGNADDR)->SetWindowText(_T("")); 

		m_strRecvAddr = buffer;

		GlobalUnlock(   hData   );   
		CloseClipboard();   
	}
	UpdateData(FALSE);
}


void CSignDlg::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strRecvAddr = "";
	m_strSignMessage = "";
	m_strSignature = "";
	m_SignTip = "";

	UpdateData(FALSE);

}
