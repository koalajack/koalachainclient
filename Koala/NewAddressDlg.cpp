// NewAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "NewAddressDlg.h"
#include "afxdialogex.h"


// CNewAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CNewAddressDlg, CDialogEx)

CNewAddressDlg::CNewAddressDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CNewAddressDlg::IDD, pParent)
	, m_strNum(_T(""))
{
	m_strNum = "1";
}

CNewAddressDlg::~CNewAddressDlg()
{
}

void CNewAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SCDZ, m_rBtnNewAdd);
	DDX_Control(pDX, IDC_LABEL_NEW, m_label);
	//DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Text(pDX, IDC_EDIT_NUM, m_strNum);
}


BEGIN_MESSAGE_MAP(CNewAddressDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SCDZ, &CNewAddressDlg::OnBnClickedButtonScdz)
END_MESSAGE_MAP()


// CNewAddressDlg ��Ϣ�������
BOOL CNewAddressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_headText.SetWindowText(_T("�½���ַ"));
	//m_headText.SetFont(100, _T("΢���ź�"));
	//m_headText.SetTextColor(RGB(255,255,255));	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_BUTTON_SCDZ)->SetWindowText("New");
		GetDlgItem(IDC_LABEL_NEW)->SetWindowText("Label");
		GetDlgItem(IDC_STATIC_NUM)->SetWindowText("Number");
		SetWindowText("New an address");
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNewAddressDlg::OnBnClickedButtonScdz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (theApp.IsLockWallet())
	{
		return;
	}

	CString strNum;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strNum);

	int nNum = _ttoi(strNum);

	if (!IsAllDigtal(strNum))
	{
		if(theApp.m_bChinese)
			::MessageBox( m_hWnd,  _T("��Ŀ����������"), _T("��ʾ"), MB_OK );
		else
			::MessageBoxEx( m_hWnd,  _T("The number must be a number"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	if(nNum < 1 || nNum > 2000)
	{
		if(theApp.m_bChinese)
			::MessageBox( m_hWnd,  _T("��ĿҪ��1~2000֮��"), _T("��ʾ"), MB_OK );
		else
			::MessageBoxEx( m_hWnd,  _T("The number should be between 1 and 2000"), _T("tip"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
		return;
	}

	string strCommand;
	string strAddrList;
	strAddrList = strprintf("%s:\n",theApp.m_bChinese ? _T("��ϲ�����µ�ַ") : _T("Congratulations on generating new addresses"));
	for(int i = 0; i < nNum; i++)
	{
		
		int nCold = 0;
		strCommand = strprintf("%s",_T("getnewaddress true"));
		nCold = 1;

		string strShowData ;
		Json::Value root; 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("OnBnClickedButtonScdz rpccmd getnewaddress error");
			return;
		}

		
		string addr = root["addr"].asString();

		CString Lable;
		GetDlgItem(IDC_EDIT_LABEL)->GetWindowText(Lable);

		uistruct::LISTADDR_t newaddr; 
		newaddr.address = addr;
		newaddr.nColdDig = nCold;
		newaddr.Label=strprintf("%s",Lable);

		string strSourceData;
		double money = 0.0;
		strSourceData =strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , addr.c_str() ,"",money ,nCold ,0,Lable) ;
		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strcutjson = newaddr.ToJson();
		pDatabase.strTabName =  _T("t_wallet_address");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string  strTemp = pDatabase.ToJson();
		postmsg.SetData(strTemp.c_str());
		theApp.m_msgQueue.push(postmsg);

		//strAddrList = strprintf("%s\n%s\n",strAddrList, addr);
	}
	strAddrList = strprintf("%s\n%s:\n",strAddrList, theApp.m_bChinese ? _T("���±���Ǯ���������µ���˽Կ") : _T("Re-backup the wallet or re-export the private key"));

	int res = 0;
	if(theApp.m_bChinese)
		res = ::MessageBox( m_hWnd, strAddrList.c_str() , theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_YESNO );
	else
		res = ::MessageBoxEx( m_hWnd, strAddrList.c_str() , theApp.m_bChinese ? _T("��ʾ") : _T("tip"), MB_YESNO, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));

	if(IDYES == res)
	{
		EndDialog(IDOK);
	}
}
