// EntInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "EntInfoDlg.h"
#include "afxdialogex.h"


// CEntInfoDlg �Ի���

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


// CEntInfoDlg ��Ϣ�������


BOOL CEntInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strEntInfo.c_str());

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		SetWindowText("Enterprise account information");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CEntInfoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
