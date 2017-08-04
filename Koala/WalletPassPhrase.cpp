// WalletPassPhrase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "WalletPassPhrase.h"
#include "afxdialogex.h"


// CWalletPassPhrase �Ի���

IMPLEMENT_DYNAMIC(CWalletPassPhrase, CDialogEx)

CWalletPassPhrase::CWalletPassPhrase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalletPassPhrase::IDD, pParent)
{

}

CWalletPassPhrase::~CWalletPassPhrase()
{
}

void CWalletPassPhrase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_TIMEOUT, m_timeout);	
	DDX_Control(pDX, IDC_MINUE, m_minue);	
}


BEGIN_MESSAGE_MAP(CWalletPassPhrase, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWalletPassPhrase::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWalletPassPhrase::OnBnClickedCancel)
END_MESSAGE_MAP()


// CWalletPassPhrase ��Ϣ�������


void CWalletPassPhrase::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString PassWord= _T("");
	CString passtime = _T("");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(PassWord);
	GetDlgItem(IDC_EDIT_TIEM)->GetWindowText(passtime);
	if (PassWord == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("����������")  , _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Please enter your password")  , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (passtime == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("������ʱ��") , _T("��ʾ")  ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please enter the time") , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("ʱ�����������")  , _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Time must be a number")  , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	
	string strCommand;
	strCommand = strprintf("%s %s %d","walletpassphrase",PassWord,(atoi(passtime)*60));
	string strShowData =_T("");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE(" rpccmd walletpassphrase error");
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("�������벻��ȷ,����������") ,_T("��ʾ")  ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enter the password incorrectly. Please re-enter it") ,_T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}


	bool isEntryp = root["passphrase"].asBool();
	if (!isEntryp)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("�������벻��ȷ,����������"), _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Enter the password incorrectly. Please re-enter it"), _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) );
		return;
	}else{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ϲǮ�������ɹ�") , _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Unlock the wallet successfully") , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	theApp.m_bIsPassLock = TRUE;
	CDialogEx::OnOK();
}


BOOL CWalletPassPhrase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_PASSWORD)->SetWindowText("Password");
		GetDlgItem(IDC_TIMEOUT)->SetWindowText("Unlock time");
		GetDlgItem(IDC_MINUE)->SetWindowText("Minutes");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Unlock wallet");
	}


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	theApp.m_bIsPassLock = FALSE;
	GetDlgItem(IDC_EDIT_TIEM)->SetWindowText(_T("60"));
	GetDlgItem(IDC_PASSWORD)->SetWindowText(theApp.m_bChinese ? _T("����:") : _T("Password"));
	GetDlgItem(IDC_TIMEOUT)->SetWindowText(theApp.m_bChinese ? _T("����ʱ��:") : _T("Unlock Time"));
	GetDlgItem(IDC_MINUE)->SetWindowText(theApp.m_bChinese ? _T("��") : _T("Minutes"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWalletPassPhrase::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_bIsPassLock = TRUE;
	CDialogEx::OnCancel();
}
