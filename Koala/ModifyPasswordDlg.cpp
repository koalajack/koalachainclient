// ModifyPasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "ModifyPasswordDlg.h"
#include "afxdialogex.h"


// CModifyPasswordDlg �Ի���

IMPLEMENT_DYNAMIC(CModifyPasswordDlg, CDialogEx)

CModifyPasswordDlg::CModifyPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CModifyPasswordDlg::IDD, pParent)
{

}

CModifyPasswordDlg::~CModifyPasswordDlg()
{
}

void CModifyPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CModifyPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CONFIRM, &CModifyPasswordDlg::OnBnClickedConfirm)
	ON_BN_CLICKED(IDC_CANCEL, &CModifyPasswordDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifyPasswordDlg ��Ϣ�������


BOOL CModifyPasswordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC66)->SetWindowText("Old password");
		GetDlgItem(IDC_STATIC67)->SetWindowText("New password");
		GetDlgItem(IDC_STATIC68)->SetWindowText("Confirm new password");
		GetDlgItem(IDC_CONFIRM)->SetWindowText("OK");
		GetDlgItem(IDC_CANCEL)->SetWindowText("Cancel");
		SetWindowText("Modify Password");
	}
*/
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CModifyPasswordDlg::OnBnClickedConfirm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputOld = _T("");
	CString confiedNew= _T("");
	CString confiedPassWord = _T("");
	GetDlgItem(IDC_EDIT_OLDPS)->GetWindowText(inputOld);
	GetDlgItem(IDC_EDIT_NEWPS)->GetWindowText(confiedNew);
	GetDlgItem(IDC_EDIT_CONFIREDNEWPS2)->GetWindowText(confiedPassWord);
	int pos = confiedNew.Find(_T(" "));
	if (pos >=0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("�����������в����пո�") , _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("There is no space in the input new password") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (inputOld == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("�����������") ,   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Please enter the old password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (confiedNew == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("������������") ,   _T("��ʾ") ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please enter a new password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (confiedPassWord == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("������ȷ��������") ,   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Please enter a confirmation new password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("���벻��ȫ������") ,   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The password can not be all numbers") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("�������������λ"),   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The number of passwords is at least six"),   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("ȷ��������������벻һ��"),   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Make sure the password and the input password are inconsistent"),   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	string strCommand;
	strCommand = strprintf("%s %s %s",_T("walletpassphrasechange"),inputOld,confiedNew);

	Json::Value root; 
	if (!CSoyPayHelp::getInstance()->SendRpc(strCommand.c_str(),root))
	{
		TRACE("rpccmd walletpassphrasechange error");
		return;
	}

	bool isEntryp = root["chgpwd"].asBool();
	if (!isEntryp)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��������벻��ȷ,����������") ,   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Enter the old password incorrectly. Please re-enter it") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}else{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ϲ�޸�����ɹ�")  ,   _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Congratulations on changing the password successfully")  ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	CDialogBase::OnOK();

}


void CModifyPasswordDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
