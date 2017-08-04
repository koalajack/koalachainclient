// SetAdminDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "SetAdminDlg.h"
#include "afxdialogex.h"
#include "AddrBookDlg.h"
#include "EntAddrBookDlg.h"
#include "OverviewDlg.h"
#include "KoalaDlg.h"
#include "CompanyInfoDlg.h"

// CSetAdminDlg 对话框

IMPLEMENT_DYNAMIC(CSetAdminDlg, CDialogEx)

CSetAdminDlg::CSetAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetAdminDlg::IDD, pParent)
	, m_strAdmin(_T(""))
{
	m_bShowAdmin = TRUE;
}

CSetAdminDlg::~CSetAdminDlg()
{
}

void CSetAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADMINADDR, m_edtAdmin);
	DDX_Text(pDX, IDC_ADMINADDR, m_strAdmin);
}


BEGIN_MESSAGE_MAP(CSetAdminDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADMIN_SELECT, &CSetAdminDlg::OnBnClickedAdminSelect)
	ON_BN_CLICKED(IDOK, &CSetAdminDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetAdminDlg 消息处理程序


void CSetAdminDlg::OnBnClickedAdminSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(m_bShowAdmin)
	{
		CAddrBookDlg dlg;
		if(IDOK == dlg.DoModal())
		{
			//theApp.m_strCurEntAddress = dlg.m_strCurSelectAddr;
			m_edtAdmin.SetWindowText(dlg.m_strCurSelectAddr.c_str());
			m_strShortAddr = dlg.m_strCurSelectShortAddr;
		}
	}
	else
	{
		CAddrBookDlg dlg;
		if(IDOK == dlg.DoModal())
		{
			m_edtAdmin.SetWindowText(dlg.m_strCurSelectAddr.c_str());
			theApp.m_strCurEntAddress = dlg.m_strCurSelectAddr;
		}
	}

}


void CSetAdminDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CKoalaDlg* pDlg = (CKoalaDlg*)AfxGetMainWnd();
	CCompanyInfoDlg* pCompanyInfoDlg = pDlg->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}
	if(m_bShowAdmin)
	{
		theApp.m_strCurAdminAddress = m_strAdmin.GetBuffer(0);
		theApp.m_strCurShortAddress = m_strShortAddr;
		/*
		((CKoalaDlg*)AfxGetMainWnd())->ShowCompanyInfoBtn();
		if(!pCompanyInfoDlg->IsWindowVisible())
			pCompanyInfoDlg->ShowWindow(SW_SHOW);
		theApp.m_nShowType = 0;
		*/
		pDlg->JudgeType();
		pCompanyInfoDlg->OnTimer(0);
	}
	else
	{
		theApp.m_strCurEntAddress = m_strAdmin.GetBuffer(0);

		COverviewDlg* pOverviewDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pOverviewDlg;

		if(!pOverviewDlg)
		{
			return;
		}

		pOverviewDlg->SetEntInfo(theApp.m_strCurEntAddress);

		/*
		BOOL bVisible = pCompanyInfoDlg->IsWindowVisible();
		if(!bVisible)
		{
			pDlg->ShowCompanyInfoBtn();
			pCompanyInfoDlg->ShowWithdrawBtn();
			pCompanyInfoDlg->ShowWindow(SW_SHOW);
		}
		if(theApp.m_strCurEntAddress.length() == 0)
			theApp.m_nShowType = 0;
		else
			theApp.m_nShowType = 1;
		*/
		pDlg->JudgeType();
		pCompanyInfoDlg->OnTimer(0);
	}
	pDlg->WriteConfig();

	CDialogEx::OnOK();
}


BOOL CSetAdminDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC555)->SetWindowText("Account address:");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Set up admin account");
	}

	// TODO:  在此添加额外的初始化
	CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;

	if(FALSE == m_bShowAdmin)
	{
		SetWindowText(theApp.m_bChinese ? "设置企业账号" : "Set up business account");

		if(pAddrListDlg->AddrIsExsit(theApp.m_strCurEntAddress))
		{
			m_edtAdmin.SetWindowText(theApp.m_strCurEntAddress.c_str());
		}
		else
		{
			theApp.m_strCurEntAddress = "";
		}

	}
	else
	{
		

		if(pAddrListDlg->AddrIsExsit(theApp.m_strCurAdminAddress))
		{
			m_edtAdmin.SetWindowText(theApp.m_strCurAdminAddress.c_str());
			m_strShortAddr = theApp.m_strCurShortAddress;
		}
		else
		{
			theApp.m_strCurAdminAddress = "";
			theApp.m_strCurShortAddress = "";
		}

	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
