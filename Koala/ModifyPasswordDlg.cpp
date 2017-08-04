// ModifyPasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ModifyPasswordDlg.h"
#include "afxdialogex.h"


// CModifyPasswordDlg 对话框

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


// CModifyPasswordDlg 消息处理程序


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
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CModifyPasswordDlg::OnBnClickedConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
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
			::MessageBox(m_hWnd, _T("输入新密码中不能有空格") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("There is no space in the input new password") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (inputOld == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("请输入旧密码") ,   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Please enter the old password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (confiedNew == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请输入新密码") ,   _T("提示") ,MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please enter a new password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (confiedPassWord == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("请输入确认新密码") ,   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("Please enter a confirmation new password") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("密码不能全是数字") ,   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The password can not be all numbers") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("密码个数最少六位"),   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd,_T("The number of passwords is at least six"),   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd,_T("确认密码和输入密码不一致"),   _T("提示") ,MB_OK );
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
			::MessageBox(m_hWnd, _T("输入旧密码不正确,请重新输入") ,   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Enter the old password incorrectly. Please re-enter it") ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}else{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("恭喜修改密码成功")  ,   _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Congratulations on changing the password successfully")  ,   _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	CDialogBase::OnOK();

}


void CModifyPasswordDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
