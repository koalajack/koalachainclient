// WalletEncDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "WalletEncDlg.h"
#include "afxdialogex.h"
#include "KoalaDlg.h"


// CWalletEncDlg �Ի���

IMPLEMENT_DYNAMIC(CWalletEncDlg, CDialogEx)

CWalletEncDlg::CWalletEncDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CWalletEncDlg::IDD, pParent)
{

}

CWalletEncDlg::~CWalletEncDlg()
{
}

void CWalletEncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWalletEncDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ENC_OK, &CWalletEncDlg::OnBnClickedBtnEncOk)
//	ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BTN_ENC_CANCEL, &CWalletEncDlg::OnBnClickedBtnEncCancel)
END_MESSAGE_MAP()


// CWalletEncDlg ��Ϣ�������


BOOL CWalletEncDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_BTN_ENC_OK)->SetWindowText("OK");
		GetDlgItem(IDC_BTN_ENC_CANCEL)->SetWindowText("Cancel");
		GetDlgItem(IDC_STATIC_INPUTMIMA)->SetWindowText("New password");
		GetDlgItem(IDC_STATIC_CONFIREDMIMA)->SetWindowText("re-new password");
		SetWindowText("Encrypt wallet");
	}
*/
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWalletEncDlg::OnBnClickedBtnEncOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString inputE = _T("");
	CString confiedE = _T("");
	GetDlgItem(IDC_EDIT_NEWPS)->GetWindowText(inputE);
	GetDlgItem(IDC_EDIT_RENEWPASSWD)->GetWindowText(confiedE);

	int pos = inputE.Find(_T(" "));
	if (pos >=0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("���������в����пո�") , _T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("There can not be spaces in the input password") , _T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (inputE == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("��������"),_T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("enter password"),_T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (confiedE == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("������ȷ������") ,_T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Please enter a confirmation password") ,_T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (inputE.GetLength()<6)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("�������������λ") ,_T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The number of passwords is at least six") ,_T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (IsAllDigtal(inputE))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("���벻��ȫ������") , _T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The password can not be all numbers") , _T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (strcmp(inputE,confiedE) != 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("ȷ��������������벻һ��") ,_T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Make sure the password and the input password are inconsistent") ,_T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s",_T("encryptwallet"),inputE);
	string strShowData ="";
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE(" rpccmd encryptwallet error");
		return;
	}


	bool isClose = FALSE; 
	bool isEntryp = root["encrypt"].asBool();
	isClose = isEntryp;
	if (isEntryp)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("Ǯ�����ܳɹ�����������Ǯ��"),_T("��ʾ"),MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("If the wallet is encrypted, please restart the wallet"),_T("tip"),MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("Ǯ������ʧ��") , _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Wallet encryption failed") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	CDialogEx::OnOK();
	
	if (isClose)
	{
		((CKoalaDlg*)(this->GetParent()))->OnClose();
	}
}


//void CWalletEncDlg::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CDialogEx::OnClose();
//}


void CWalletEncDlg::OnBnClickedBtnEncCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
