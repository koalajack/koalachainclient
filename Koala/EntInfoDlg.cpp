// EntInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "EntInfoDlg.h"
#include "afxdialogex.h"


// CEntInfoDlg 对话框

IMPLEMENT_DYNAMIC(CEntInfoDlg, CDialogEx)

CEntInfoDlg::CEntInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntInfoDlg::IDD, pParent)
{
	m_strEntInfo = "";
}

CEntInfoDlg::~CEntInfoDlg()
{
}

void CEntInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEntInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEntInfoDlg 消息处理程序


BOOL CEntInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strEntInfo.c_str());

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		SetWindowText("Enterprise account information");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CEntInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
