// VerifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "VerifyDlg.h"
#include "afxdialogex.h"


// CVerifyDlg 对话框

IMPLEMENT_DYNCREATE(CVerifyDlg, CDialogEx)

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVerifyDlg::IDD, pParent)
	, m_editSendAddr(_T(""))
	, m_richeditMessage(_T(""))
	, m_editSignature(_T(""))
	, m_ConfirmTip(_T(""))
{

}

CVerifyDlg::~CVerifyDlg()
{
}

void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_VERTIFYMESSAGE, m_redtConfirm);
	DDX_Text(pDX, IDC_EDIT_SENDADDR, m_editSendAddr);
	DDX_Text(pDX, IDC_RICHEDIT_VERTIFYMESSAGE, m_richeditMessage);
	DDX_Text(pDX, IDC_EDIT_SIGNATURE, m_editSignature);
	DDX_Text(pDX, IDC_VERTIFYMESSAGERESULT, m_ConfirmTip);
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SELECTSENDADDR, &CVerifyDlg::OnBnClickedBtnSelectsendaddr)
	ON_BN_CLICKED(IDC_BUTTON_M_CONFIRM, &CVerifyDlg::OnBnClickedButtonMConfirm)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CVerifyDlg::OnBnClickedButtonDelete)
	ON_STN_CLICKED(IDC_STATIC33, &CVerifyDlg::OnStnClickedStatic33)
END_MESSAGE_MAP()


// CVerifyDlg 消息处理程序


BOOL CVerifyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC33)->SetWindowText("Enter the signature address, message (please make sure that the newline, space character, tab, etc., etc.) are missing below and sign to verify the message. Please make sure that the signature information is accurate and that the middleman is attacked.");
		GetDlgItem(IDC_BTN_SELECTSENDADDR)->SetWindowText("Select");
		GetDlgItem(IDC_BUTTON_M_CONFIRM)->SetWindowText("Verify sign");
		GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText("Clear");
	}

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CVerifyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pWnd = GetFocus();
		if(pWnd != NULL && (pWnd->m_hWnd == m_redtConfirm.GetSafeHwnd()))
		{
			;
		}
		else
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CVerifyDlg::OnBnClickedBtnSelectsendaddr()
{
	// TODO: 在此添加控件通知处理程序代码

	CPayeeAddrBookDlg addrbook;
	if(IDOK == addrbook.DoModal())
	{
		uistruct::ADDRBOOK_t addr;
		addrbook.GetAddrbook(addr);

		//m_strAddrTo = addr.address.c_str();
		//m_strLabel = addr.label.c_str();
		UpdateData(FALSE);
	}

}


void CVerifyDlg::OnBnClickedButtonMConfirm()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	string strCommand;
	strCommand = strprintf("%s %s %s %s","verifymessage", m_editSendAddr, m_editSignature,m_richeditMessage);

	string strShowData;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData))
	{
		TRACE("AcceptRePacketCommtRecord rpccmd gettxdetail error");
		return;
	}

	if(strShowData.find("true") != string::npos)
	{
		//MessageBox(strShowData.c_str(), "提示");
	}
	else
	{
		m_ConfirmTip = theApp.m_bChinese ? "消息验证失败！" : "Message validation failed! ";
		UpdateData(FALSE);
		return;
	}
	
	m_ConfirmTip = theApp.m_bChinese ? "消息验证成功！" : "Message validation successful! ";

	UpdateData(FALSE);
}


void CVerifyDlg::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码

	m_editSendAddr = "";
	m_richeditMessage = "";
	m_editSignature = "";
	m_ConfirmTip = "";

	UpdateData(FALSE);
}


void CVerifyDlg::OnStnClickedStatic33()
{
	// TODO: 在此添加控件通知处理程序代码
}
