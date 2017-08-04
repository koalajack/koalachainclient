// SelectSendAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "SelectSendAddr.h"
#include "afxdialogex.h"


// CSelectSendAddr 对话框

IMPLEMENT_DYNAMIC(CSelectSendAddr, CDialogEx)

CSelectSendAddr::CSelectSendAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectSendAddr::IDD, pParent)
{

}

CSelectSendAddr::~CSelectSendAddr()
{
}

void CSelectSendAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectSendAddr, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SELECTSENDADDR, &CSelectSendAddr::OnBnClickedBtnSelectsendaddr)
END_MESSAGE_MAP()


// CSelectSendAddr 消息处理程序


void CSelectSendAddr::OnBnClickedBtnSelectsendaddr()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CSelectSendAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC333)->SetWindowText("This is the purse address you used to pay for. Before making a payment, please check the payment amount and payment address carefully.");
		GetDlgItem(IDC_BTN_SELECTSENDADDR)->SetWindowText("Select");
		SetWindowText("Select send address");
	}

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
