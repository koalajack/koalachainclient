// EntRigeistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "EntRigeistDlg.h"
#include "afxdialogex.h"


// CEntRigeistDlg �Ի���

IMPLEMENT_DYNAMIC(CEntRigeistDlg, CDialogEx)

CEntRigeistDlg::CEntRigeistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntRigeistDlg::IDD, pParent)
	, m_strCompanyName(_T(""))
{
	
}

CEntRigeistDlg::~CEntRigeistDlg()
{
}

void CEntRigeistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDT_COMPANY_NAME, m_edtCompanyName);
	DDX_Text(pDX, IDC_EDT_COMPANY_NAME, m_strCompanyName);
}


BEGIN_MESSAGE_MAP(CEntRigeistDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntRigeistDlg::OnBnClickedMfcbtnOk)
END_MESSAGE_MAP()


// CEntRigeistDlg ��Ϣ�������


BOOL CEntRigeistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC12)->SetWindowText("Bussiness name:");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Register bussiness");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CEntRigeistDlg::OnBnClickedMfcbtnOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	/*
	if(m_strCompanyName.IsEmpty())
	{
		return;
	}

	string strBinPath;
	strBinPath += theApp.m_strInsPath;
	strBinPath += "\\kaola.bin";

	string strCommand;
	strCommand = strprintf("%s %s %s %s %s",_T("registerapptx") , "0-3", strBinPath.c_str(), "110000000 0",  (LPCTSTR)m_strCompanyName);
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return;
	}

	TRACE(root.toStyledString().c_str());
	*/

	if(m_strCompanyName.IsEmpty())
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("����д��ҵ��"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please fill in the business name"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(IsAllDigtal(m_strCompanyName.GetBuffer(0)))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("����ҵ������ȫΪ����"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The name of the enterprise can not be all numbers"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}

	if(pCompanyInfoDlg->FindEnt(m_strCompanyName.GetBuffer(0)))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("����ҵ�Ѿ��Ǽǹ�"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The business has been registered"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(theApp.m_strCurAdminAddress.length() == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("����ѡ��һ������Ա�˺�"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please select an administrator account first"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (!theApp.m_bIsSyncBlock )
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Synchronization is not completed and can not be sent") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (theApp.IsLockWallet())
	{
		return ;
	}

	string strBinPath;
	strBinPath += theApp.m_strInsPath;
	strBinPath += "\\kaola.bin";

	CString strExtraInfo;
	strExtraInfo += m_strCompanyName;
	strExtraInfo += "@";
	strExtraInfo += theApp.m_strCurAdminAddress.c_str();

	string strCommand;
	strCommand = strprintf("%s %s %s %s %s",_T("registerapptx") , theApp.m_strCurShortAddress.c_str(), strBinPath.c_str(), "110000000 0",  (LPCTSTR)strExtraInfo);
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ҵ�Ǽ�ʧ��"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Business registration failure"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	pCompanyInfoDlg->InsertEntName(m_strCompanyName.GetBuffer(0));

	if(theApp.m_bChinese)
		::MessageBox(m_hWnd, _T("��ҵ�Ǽ���Ϣ�ѳɹ��ύ�������Եȴ�������������Ч��"), _T("��ʾ"), MB_OK);
	else
		::MessageBoxEx(m_hWnd, _T("Business registration information has been successfully submitted, please tolerate wait 1 to 2 minutes to take effect."), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	TRACE(root.toStyledString().c_str());

	CDialogEx::OnOK();
}
