// EntCfgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "EntCfgDlg.h"
#include "afxdialogex.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"


// CEntCfgDlg 对话框

IMPLEMENT_DYNAMIC(CEntCfgDlg, CDialogEx)

CEntCfgDlg::CEntCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntCfgDlg::IDD, pParent)
	, m_strAccountAddr(_T(""))
	, m_fQuotaByTime(0.0f)
	, m_fQuotaByDay(0.0f)
{
	m_bHadConfig = FALSE;
}

CEntCfgDlg::~CEntCfgDlg()
{
}

void CEntCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_ADDR, m_strAccountAddr);
	DDX_Text(pDX, IDC_EDT_EACH_QUOTA, m_fQuotaByTime);
	DDX_Text(pDX, IDC_EDT_DAY_QUOTA, m_fQuotaByDay);
	DDX_Control(pDX, IDC_EDT_ADDR, m_edtAddr);
}


BEGIN_MESSAGE_MAP(CEntCfgDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntCfgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEntCfgDlg 消息处理程序
#pragma pack(1)
struct APP_CFG
{
	unsigned char type;
	ULONGLONG ullQuotaByTime;
	ULONGLONG ullQuotaByDay;
	char szEntAddr[35];
};

typedef struct {
	unsigned char type;            //!<交易类型
	ULONGLONG ullQuotaByTime;       //!<每次限额
	ULONGLONG ullQuotaByDay;        //!<每日限额
}APP_MODIFY;  //!<修改企业配置信息
#pragma pack()

#define TX_CONFIG     0x01     //企业配置信息
#define TX_RECHARGE   0x02     // 企业批发币，充值
#define TX_WITHDRAW   0x03     //企业提币

BOOL CEntCfgDlg::IsActived(string strAddr)
{
	Json::Value root;
	string strValue;
	string tmp;

	string strCommand = strprintf("%s %s",_T("getaccountinfo"), strAddr.c_str()); 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		return FALSE;
	}

	string str = root.toStyledString();
	int pos = str.find("RegID");
	if(pos >= 0)
	{
		string strRegId = root["RegID"].asString();
		if(strRegId.length() <= 0)
			return FALSE;

		if(strRegId[0] == ' ')
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

void CEntCfgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}

	if(m_strAccountAddr.IsEmpty())
	{
		return;
	}

	if(m_strScriptId.length() == 0)
	{
		return;
	}

	if(theApp.m_strCurAdminAddress.length() == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("请先选择一个管理员账号"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Please select an administrator account first"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	
	if(m_fQuotaByTime <= 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("每次限额不能小于或等于0"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Each limit can not be less than or equal to 0"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fQuotaByDay <= 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("每日限额不能小于或等于0"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The daily limit can not be less than or equal to zero"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}
	
	if(FALSE == IsActived(m_strAccountAddr.GetBuffer(0)))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("配置失败，企业账户必须先激活"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Configuration failed, business account must be activated first"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (!theApp.m_bIsSyncBlock )
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("同步未完成,不能发送交易") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("Synchronization is not completed and can not be sent") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if (theApp.IsLockWallet())
	{
		return ;
	}
	
	string strContract;
	if(FALSE == m_bHadConfig)
	{
		if(pCompanyInfoDlg->FindEntAddr(m_strAccountAddr.GetBuffer(0)))
		{
			if(theApp.m_bChinese)
				::MessageBox(m_hWnd, _T("该账号地址已经配置过"), _T("提示"), MB_OK);
			else
				::MessageBoxEx(m_hWnd, _T("The account address has been configured"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
			return;
		}

		APP_CFG app_cfg;
		memset(&app_cfg, 0, sizeof(APP_CFG));
		app_cfg.type = TX_CONFIG;
		memcpy(app_cfg.szEntAddr, m_strAccountAddr.GetBuffer(0), 34);
		app_cfg.ullQuotaByTime = REAL_MONEY(m_fQuotaByTime);
		app_cfg.ullQuotaByDay = REAL_MONEY(m_fQuotaByDay);

		int size = sizeof(APP_CFG);

		strContract = CSoyPayHelp::getInstance()->GetHexData((const char*)&app_cfg,sizeof(APP_CFG));
	}
	else
	{
		APP_MODIFY app_modify;
		memset(&app_modify, 0, sizeof(APP_MODIFY));
		app_modify.type = TX_CONFIG;
		app_modify.ullQuotaByTime = REAL_MONEY(m_fQuotaByTime);
		app_modify.ullQuotaByDay = REAL_MONEY(m_fQuotaByDay);

		int size = sizeof(APP_MODIFY);

		strContract = CSoyPayHelp::getInstance()->GetHexData((const char*)&app_modify,sizeof(APP_MODIFY));
	}


	/*
	string strCommand;
	strCommand = strprintf("%s %s %s %s %s %lld %s",_T("createcontracttx") , theApp.m_strCurShortAddress.c_str(), m_strScriptId, "0", strContract.c_str()  ,theApp.m_BussCfg.ModifyFee, "0");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd registerapptx error");
		::MessageBox(m_hWnd, _T("企业配置失败"), _T("提示"), MB_OK);
		return;
	}

	TRACE(root.toStyledString().c_str());
	*/

	string strCommand = CSoyPayHelp::getInstance()->CreateContractTx(m_strScriptId, theApp.m_strCurShortAddress,strContract, 0,theApp.m_BussCfg.ModifyFee,0);
	string strShowData = _T("");
	CSoyPayHelp::getInstance()->SendContacrRpc(strCommand,strShowData);

	TRACE(strShowData.c_str());
	int pos = strShowData.find("hash");
	if ( pos >=0 ) {
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("企业配置信息已成功提交，请耐性等待１～２分钟生效。"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enterprise configuration information has been successfully submitted, please tolerate wait 1 to 2 minutes to take effect."), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("企业配置失败"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Enterprise configuration failed"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}


	CDialogEx::OnOK();
}


BOOL CEntCfgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
/*
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC_ADDR5)->SetWindowText("Address:");
		GetDlgItem(IDC_STATIC_EACH5)->SetWindowText("Each quota:");
		GetDlgItem(IDC_STATIC_DAY5)->SetWindowText("Day quota:");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Config information");
	}
*/
	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);

	if(m_bHadConfig)
	{
		m_edtAddr.SetReadOnly();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
