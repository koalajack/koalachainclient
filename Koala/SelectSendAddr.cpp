// SelectSendAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "SelectSendAddr.h"
#include "afxdialogex.h"


// CSelectSendAddr �Ի���

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


// CSelectSendAddr ��Ϣ�������


void CSelectSendAddr::OnBnClickedBtnSelectsendaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL CSelectSendAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC333)->SetWindowText("This is the purse address you used to pay for. Before making a payment, please check the payment amount and payment address carefully.");
		GetDlgItem(IDC_BTN_SELECTSENDADDR)->SetWindowText("Select");
		SetWindowText("Select send address");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
