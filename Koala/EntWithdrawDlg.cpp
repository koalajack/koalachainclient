// EntWithdrawDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "EntWithdrawDlg.h"
#include "afxdialogex.h"
#include "CompanyInfoDlg.h"
#include "KoalaDlg.h"


// CEntWithdrawDlg 对话框

IMPLEMENT_DYNAMIC(CEntWithdrawDlg, CDialogEx)

CEntWithdrawDlg::CEntWithdrawDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEntWithdrawDlg::IDD, pParent)
	, m_strWithdrawAddr(_T(""))
	, m_fMoney(0.0f)
{

}

CEntWithdrawDlg::~CEntWithdrawDlg()
{
}

void CEntWithdrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_WITHDRAW_ADDR, m_strWithdrawAddr);
	DDX_Text(pDX, IDC_EDT_MONEY, m_fMoney);
}


BEGIN_MESSAGE_MAP(CEntWithdrawDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEntWithdrawDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEntWithdrawDlg 消息处理程序
#pragma pack(1)
typedef struct {
	unsigned char type;            //!<交易类型
	char  address[35];             //!<备注说明 字符串以\0结束，长度不足后补0
	ULONGLONG moneyM;   
}COMPANY_WITHDRAW;
#pragma pack()

void CEntWithdrawDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	DOUBLE dValue = 0;

	CCompanyInfoDlg* pCompanyInfoDlg = ((CKoalaDlg*)AfxGetMainWnd())->m_pCompanyInfoDlg;

	if(!pCompanyInfoDlg)
	{
		return;
	}

	if(m_strWithdrawAddr.IsEmpty())
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("提现地址不能为空"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The address can not be empty"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fMoney == 0)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("提现金额不能为0"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("The amount of cash can not be 0"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_strScriptId.length() == 0)
	{
		return;
	}

	string strEntAddr;
	if(!pCompanyInfoDlg->FindEntAddByScriptId(m_strScriptId, strEntAddr))
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("没有相应的企业地址"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("There is no corresponding business address"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	LIST_APP_DATA* app_data = NULL;

	BOOL ret = pCompanyInfoDlg->FindAppDataByScriptId(m_strScriptId, &app_data);
	if(ret)
	{
		if((app_data->fWithdrawByToday >= app_data->fQuotaByDay) && (app_data->fBalance > 0))
		{
			if(app_data->nHeight > 0)
			{
				int nMinutes = 1440 - theApp.m_nBlockTipHight % 1440;
				int nHours = nMinutes / 60 + (nMinutes % 60 > 0 ? 1 : 0);

				char szInfo[256] = {0};
				sprintf(szInfo, theApp.m_bChinese ? "当天的币已经全部提现，请等待大约%d小时后，再提取！" : "The day of the currency has been raised, please wait about %d hours later, and then extract", nHours); 
				if(theApp.m_bChinese)
					::MessageBox(m_hWnd, szInfo,theApp.m_bChinese ? _T("提示") : _T("tip"), MB_OK);
				else
					::MessageBoxEx(m_hWnd, szInfo,theApp.m_bChinese ? _T("提示") : _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

				CDialogEx::OnOK();
				return;
			}
		}
	}

	dValue = app_data->fQuotaByDay - app_data->fWithdrawByToday;
	if(dValue < 0)
	{
		dValue = 0;
	}

	if(m_fMoney > dValue)
	{
		CString strQuotaByDay;
		strQuotaByDay.Format(theApp.m_bChinese ? _T("当日可提现余额为%0.2lf") : _T("The current balance can be raised to %0.2lf"), dValue);
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, strQuotaByDay , theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, strQuotaByDay , theApp.m_bChinese ? _T("提示") : _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(m_fMoney > app_data->fBalance)
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("用户企业账户余额不够") , _T("提示") ,MB_OK );
		else
			::MessageBoxEx(m_hWnd, _T("The user's business account balance is not enough") , _T("tip") ,MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
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

	/*
	COMPANY_WITHDRAW company_withdraw;
	memset(&company_withdraw, 0, sizeof(COMPANY_WITHDRAW));
	company_withdraw.type = 0x03;

	char* pAddress = "dydzZu5EZj45aCiLCb94tZiU12RwaCx98z";
	memcpy(company_withdraw.address, pAddress, 34);

	company_withdraw.moneyM = 100000000;

	string addr = "dtQE7px7YhQQpAvAGyRGX2R7vPdgjvapEH";

	string strContract = CSoyPayHelp::getInstance()->GetHexData((const char*)&company_withdraw,sizeof(COMPANY_WITHDRAW));
	string strCommand = CSoyPayHelp::getInstance()->CreateContractTx("5-1", addr,strContract, 0,100000000,0);
	*/

	COMPANY_WITHDRAW company_withdraw;
	memset(&company_withdraw, 0, sizeof(COMPANY_WITHDRAW));
	company_withdraw.type = 0x03;

	memcpy(company_withdraw.address, m_strWithdrawAddr.GetBuffer(0), 34);

	company_withdraw.moneyM = REAL_MONEY(m_fMoney);
	

	string strContract = CSoyPayHelp::getInstance()->GetHexData((const char*)&company_withdraw,sizeof(COMPANY_WITHDRAW));
	string strCommand = CSoyPayHelp::getInstance()->CreateContractTx(m_strScriptId, strEntAddr,strContract, 0,theApp.m_BussCfg.WithdrawFee,0);
	string strShowData = _T("");
	CSoyPayHelp::getInstance()->SendContacrRpc(strCommand,strShowData);

	TRACE(strShowData.c_str());
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {

		//提现金额ｘｘｘｘ至账户ｘｘｘｘｘｘ请求已提交，请等待１～２分钟后，查看提币账户余额。
		char szInfo[512] = {0};
		string addr;
		pCompanyInfoDlg->FindEntAddByScriptId(m_strScriptId, addr);
		sprintf(szInfo, theApp.m_bChinese ? "提现金额%0.2f至账户%s,请求已提交，请等待１～２分钟后，查看提币账户余额。" : "Withdrawal Amount %0.2f to account %s, request has been submitted, please wait 1 to 2 minutes later, check the currency account balance.", m_fMoney, m_strWithdrawAddr.GetBuffer(0));
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, szInfo, _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, szInfo, _T("提示"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		if(theApp.m_bChinese)
			::MessageBox(m_hWnd, _T("提现失败"), _T("提示"), MB_OK);
		else
			::MessageBoxEx(m_hWnd, _T("Failure to withdraw"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}

	CDialogEx::OnOK();
}


BOOL CEntWithdrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STATIC22)->SetWindowText("Account address:");
		GetDlgItem(IDC_STATIC23)->SetWindowText("Amount:");
		GetDlgItem(IDOK)->SetWindowText("OK");
		GetDlgItem(IDCANCEL)->SetWindowText("Cancel");
		SetWindowText("Withdraw");
	}

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
