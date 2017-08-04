// TxDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg �Ի���

IMPLEMENT_DYNAMIC(CTxDetailDlg, CDialogEx)

CTxDetailDlg::CTxDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxDetailDlg::IDD, pParent)
{

}

CTxDetailDlg::~CTxDetailDlg()
{
}

void CTxDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTxDetailDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CLOSE, &CTxDetailDlg::OnBnClickedClose)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CTxDetailDlg::OnBnClickedOk)
END_MESSAGE_MAP()

string CTxDetailDlg::GetContacrDetail(uistruct::REVTRANSACTION_t tx)
{
	string strCommand,strShowData;
	strCommand = strprintf("%s %s","getaccountinfo" ,tx.desaddr.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData  ==""){		
		return _T("");
	}
	Json::Reader reader;  
	Json::Value root; 

	if (reader.parse(strShowData, root)) {		

		if (strShowData.find("RegID") >0)
		{
			tx.desregid = root["RegID"].asString();
		}
	}

	string txdetail ="";
	string nValue = tx.Contract;
	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

	string strContract = CSoyPayHelp::getInstance()->HexStr(vTemp);
	txdetail +=strprintf("%s:   %d\r\n\r\n",theApp.m_bChinese ? _T("��Լ") : _T("Contract") ,strContract.c_str() );


	return txdetail;
}
// CTxDetailDlg ��Ϣ�������
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		string strShowData;
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("�˽��ײ�����") : _T("This transaction does not exist")) ;
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData.c_str());
		return;
	}
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	string txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("�ڿ�������") : _T("Mining Reward Transaction")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("ע���˻�����") : _T("Register Account Transaction")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("ת�˽���") : _T("Transfer Transaction")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("��Լ����") : _T("Contract Transaction")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData+=strprintf("%s" ,theApp.m_bChinese ? _T("ע��Ӧ�ý���") : _T("Register App Transaction")) ;
	}
	txdetail=strprintf("%s: %s\r\n\r\n",theApp.m_bChinese ? _T("��������") : _T("Transaction Type"),strShowData.c_str());
	txdetail+=strprintf("%s: %s\r\n\r\n",theApp.m_bChinese ? _T("����ID") : _T("Transaction ID"),tx.txhash);
	txdetail+=strprintf("%s:   %d\r\n\r\n",theApp.m_bChinese ? _T("�汾��") : _T("Version"),tx.ver);
	txdetail+=strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("Դ��ַ") : _T("Source Address"),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("��Կ") : _T("Public Key"),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("���ڿ�Կ") : _T("Cold Mining Public Key"),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail+=strprintf("%s:   %.8f\r\n\r\n",theApp.m_bChinese ? _T("С��") : _T("Fee"),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail+=strprintf("%s:   %d\r\n\r\n",theApp.m_bChinese ? _T("���׳�ʱ�߶�") : _T("Transaction Timeout Height"),height);
	}
	if (tx.desaddr != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("Ŀ�ĵ�ַ") : _T("Destination Address"),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail+=strprintf("%s:   %.2f\r\n\r\n",theApp.m_bChinese ? _T("���׽��") : _T("Amount"),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		//txdetail.AppendFormat(_T("��Լ����:   %d\r\n\r\n"),tx.Contract.c_str() );
		txdetail+=strprintf("%s",GetContacrDetail(tx).c_str());
		
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail+=strprintf("%s:   %d\r\n\r\n",theApp.m_bChinese ? _T("ȷ�ϸ߶�") : _T("Confirmed Height"),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail+=strprintf("%s:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",theApp.m_bChinese ? _T("ȷ��ʱ��") : _T("Confirmed Time"),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",theApp.m_bChinese ? _T("ȷ��blockHash") : _T("Confirmed blockHash"),tx.blockhash.c_str());
	}

	if ( theApp.blocktipheight != 0 && tx.confirmedHeight != 0)
	{
		unsigned int quredCount = theApp.blocktipheight - tx.confirmedHeight;
		if (quredCount >=0)
		{
			txdetail+=strprintf("%s:  %d\r\n\r\n",theApp.m_bChinese ? _T("ȷ����") : _T("Confirmed Count"),quredCount );
		}	
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDOK)->SetWindowText("OK");
		SetWindowText("Detail of transaction");
	}


	ShowTxDetail(theApp.m_strAddress);

	GetDlgItem(IDOK)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTxDetailDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}



HBRUSH CTxDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	/*
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_TXDETAIL)
	{

		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));

	}
	*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CTxDetailDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
