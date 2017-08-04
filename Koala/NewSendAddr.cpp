// NewSendAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "NewSendAddr.h"
#include "afxdialogex.h"


// CNewSendAddr �Ի���

IMPLEMENT_DYNAMIC(CNewSendAddr, CDialogEx)

CNewSendAddr::CNewSendAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewSendAddr::IDD, pParent)
{

}

CNewSendAddr::~CNewSendAddr()
{
}

void CNewSendAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CNewSendAddr, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewSendAddr::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CNewSendAddr::OnBnClickedClose)
END_MESSAGE_MAP()


// CNewSendAddr ��Ϣ�������


void CNewSendAddr::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString label =_T("");
	CString addr  = _T("");
	GetDlgItem(IDC_EDIT_Lebel)->GetWindowText(label);
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ַ����Ϊ��") , _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The address can not be empty") , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	uistruct::ADDRBOOK_t sAddrBookItem;
	string strCond;
	strCond = strprintf(" address='%s' ", addr);
	
	int  item = theApp.m_SqliteDeal.GetAddressBookItem(strCond, &sAddrBookItem);
	if (sAddrBookItem.address != _T(""))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ַ�����Ѵ��ڴ˵�ַ") , _T("��ʾ")  ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("This address already exists in the address book") , _T("tip")  ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	m_selectAddr.label = label;
	m_selectAddr.address = addr;
	CDialogEx::OnOK();
}
void CNewSendAddr::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}
BOOL CNewSendAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		GetDlgItem(IDC_LABEL5)->SetWindowText("Label");
		GetDlgItem(IDC_ADDR5)->SetWindowText("Address");
		SetWindowText("New send address");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CNewSendAddr::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
