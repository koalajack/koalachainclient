// EntRechargeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "EntRechargeDlg.h"
#include "afxdialogex.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"
#include "AddrBookDlg.h"

// CEntRechargeDlg �Ի���

IMPLEMENT_DYNAMIC(CEntRechargeDlg, CDialogEx)

CEntRechargeDlg::CEntRechargeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntRechargeDlg::IDD, pParent)
	, m_fRecharge(0.0f)
	, m_strEntName(_T(""))
	, m_strRechargeSrc(_T(""))
	, m_strInputScriptId(_T(""))
{

}

CEntRechargeDlg::~CEntRechargeDlg()
{
}

void CEntRechargeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_RECHARGE, m_fRecharge);
	DDX_Text(pDX, IDC_EDT_RECHARGE_ENTNAME, m_strEntName);
	DDX_Text(pDX, IDC_EDT_RECHARGE_SRC, m_strRechargeSrc);
	DDX_Text(pDX, IDC_EDIT_SCRIPT_ID, m_strInputScriptId);
}


BEGIN_MESSAGE_MAP(CEntRechargeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntRechargeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MFCBTN_REC_FROM, &CEntRechargeDlg::OnBnClickedMfcbtnRecFrom)
END_MESSAGE_MAP()


// CEntRechargeDlg ��Ϣ�������
#pragma pack(1)
typedef struct {
	unsigned char type;            //!<��������

}COMPANY_RECHARGE;   
#pragma pack()

void CEntRechargeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	LIST_APP_DATA *pData = NULL;

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;
	if(!pCompanyInfoDlg)
	{
		return;
	}
	if(!pCompanyInfoDlg->FindAppDataByScriptId(m_strInputScriptId.GetBuffer(), &pData))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ҵ�˻�ID����"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enterprise account ID is incorrect��"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	
	int iPos = pData->entName.find('@');
	if(iPos == string::npos)
	{
		return;
	}
	string strEntName = pData->entName.substr(0, iPos);

	if(strEntName.compare(m_strEntName))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ҵ���ƺ���ҵ�˻�ID��һ��"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enterprise name and business account ID are inconsistent"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_strInputScriptId.GetLength() == 0)
	{
		return;
	}

	/*if(m_strEntName != m_strVerifyEntName)
	{
	::MessageBox(m_hWnd, _T("�������ҵ��������ѡ�����ҵ���Ʋ�һ��"), _T("��ʾ"), MB_OK);
	return;
	}*/
	/*
	if(theApp.m_strCurAdminAddress.length() == 0)
	{
		::MessageBox(m_hWnd, _T("����ѡ��һ������Ա�˺�"), _T("��ʾ"), MB_OK);
		return;
	}
	*/
	if(0 == m_strRechargeSrc.GetLength() || m_strRechargeSrc == "")
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ѡ��һ����ֵ��Դ"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please choose a recharge source"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;

	if(FALSE == pAddrListDlg->AddrIsExsit(m_strRechargeSrc.GetBuffer(0)))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��Դ��ַ�����ڻ�û����"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The source address does not exist or is not active"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fRecharge == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ֵ����Ϊ0"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The recharge amount can not be zero"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
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

	COMPANY_RECHARGE company_recharge;
	memset(&company_recharge, 0, sizeof(COMPANY_RECHARGE));
	company_recharge.type = 02;

	string strContract = CSoyPayHelp::getInstance()->GetHexData((const char*)&company_recharge,sizeof(COMPANY_RECHARGE));
	//string strMoney =CSoyPayHelp::getInstance()->GetHexData((const char*)&m_lluRecharge,sizeof(ULONGLONG));
	/*
	string strCommand;
	strCommand = strprintf("%s %s %s %s %s %s %s",_T("createcontracttx") , "0-3", "2474-1", "200000000", strContract.c_str()  ,"100000000", "0");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return;
	}
	*/
	
	string strCommand = CSoyPayHelp::getInstance()->CreateContractTx(m_strInputScriptId.GetBuffer(), /*theApp.m_strCurShortAddress.c_str()*/m_strRechargeSrc.GetBuffer(0), strContract, 0,theApp.m_BussCfg.RechargeFee,REAL_MONEY(m_fRecharge));
	string strShowData = _T("");
	CSoyPayHelp::getInstance()->SendContacrRpc(strCommand,strShowData);

	TRACE(strShowData.c_str());
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {

		//��ֵ�������������˻��������������������ύ����ȴ����������Ӻ󣬵ȴ���ֵ���ˡ�
		char szInfo[512] = {0};
		string addr;
		pCompanyInfoDlg->FindEntAddByScriptId(m_strScriptId, addr);
		sprintf(szInfo, theApp.m_bChinese ? "��ֵ���%0.2f���˻�%s,�������ύ����ȴ����������Ӻ󣬵ȴ���ֵ���ˡ�" : _T("Recharge the amount of %0.2f to account %s,request has been submitted, please wait 1 ~ 2 minutes, waiting for recharge to account"), m_fRecharge, addr.c_str());

		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, szInfo, theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, szInfo, theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("��ҵ��ֵʧ��"), _T("��ʾ"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enterprise recharge failed"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	

	CDialogEx::OnOK();
}


void CEntRechargeDlg::OnBnClickedMfcbtnRecFrom()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CAddrBookDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		m_strRechargeSrc.Format("%s", dlg.m_strCurSelectAddr.c_str());
		GetDlgItem(IDC_EDT_RECHARGE_SRC)->SetWindowText(m_strRechargeSrc);
	}

}


BOOL CEntRechargeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC6)->SetWindowText("From:");
		GetDlgItem(IDC_STATIC7)->SetWindowText("Account ID:");
		GetDlgItem(IDC_STATIC8)->SetWindowText("Bussiness name:");
		GetDlgItem(IDC_STATIC9)->SetWindowText("Charge Amount:");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Recharge");
	}
*/
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(1 == theApp.m_bIsAdmin || 3 == theApp.m_bIsAdmin)
	{
		CAddrSelectDlg* pAddrListDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pAddrListDlg;
		if(theApp.m_strCurAdminAddress.length() != 0 && pAddrListDlg->AddrIsExsit(theApp.m_strCurAdminAddress))
		{
			GetDlgItem(IDC_EDT_RECHARGE_SRC)->SetWindowText(theApp.m_strCurAdminAddress.c_str());
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
