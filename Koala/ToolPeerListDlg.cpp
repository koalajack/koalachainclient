// ToolPeerListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "ToolPeerListDlg.h"
#include "afxdialogex.h"

// CToolPeerListDlg 对话框

#define  TIMER_PEERINFO 1001

IMPLEMENT_DYNCREATE(CToolPeerListDlg, CDialogEx)

CToolPeerListDlg::CToolPeerListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolPeerListDlg::IDD, pParent)
{
	m_nIndex = 0;
}

CToolPeerListDlg::~CToolPeerListDlg()
{
}

void CToolPeerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PEERLIST, m_lstPeerList);
}


BEGIN_MESSAGE_MAP(CToolPeerListDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PEERLIST, &CToolPeerListDlg::OnNMClickListPeerlist)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CToolPeerListDlg 消息处理程序


BOOL CToolPeerListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[5]  = {
		{"Address/Hostname" ,      150},
		{"User Agent" ,      160},
		{"Ping Time" ,      60}, 
	};
	m_lstPeerList.SetBkColor(RGB(255,255,255));       
	m_lstPeerList.SetRowHeigt(23);               
	m_lstPeerList.SetHeaderHeight(1.5);         
	//m_lstHistroy.SetHeaderFontHW(15,0);
	COLORREF btnColor = /*GetSysColor(COLOR_BTNFACE)*/RGB(255, 255, 255);
	m_lstPeerList.SetHeaderBKColor(GetRValue(btnColor), GetGValue(btnColor),GetBValue(btnColor),0);
	m_lstPeerList.SetHeaderTextColor(RGB(0,0,0)); 
	m_lstPeerList.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i <5 ; i++  ) {
		m_lstPeerList.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}

	GetPeerlist(m_PeerListInfo);

	map<string,LIST_PEER_DATA>::iterator found = m_PeerListInfo.begin();
	if(found != m_PeerListInfo.end())
	{
		SetInfo(found->second);
	}

	if(m_PeerListInfo.size())
	{
		map<string,LIST_PEER_DATA>::iterator it;

		int i = 0, iSub = 0;
		CString strShowData;
		for(it = m_PeerListInfo.begin();it != m_PeerListInfo.end();++it)
		{
			iSub = 0;
		    LIST_PEER_DATA data = it->second;
			m_lstPeerList.InsertItem(i, data.addr.c_str());

			m_lstPeerList.SetItemText(i, ++iSub, data.subver.c_str());

			strShowData.Format("%lld", (data.pingtime));
			m_lstPeerList.SetItemText(i, ++iSub,strShowData);

			i++;
		}
		
	}
	m_lstPeerList.SetItemState(m_nIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED); 
	m_lstPeerList.SetSelectionMark(m_nIndex);

	SetTimer(TIMER_PEERINFO, 1000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

string CToolPeerListDlg::Second2HMS(ULONGLONG ullSecond)
{
	CString str;
	CString strTemp;
	if(ullSecond > 0)
	{
		ULONGLONG day    = ullSecond / (3600*24);
		ULONGLONG hour   = (ullSecond - day*(3600*24)) / (3600);
		ULONGLONG minute = (ullSecond - day*(3600*24) - hour*60*60)/(60);
		ULONGLONG second = ullSecond - day*(3600*24) - hour*60*60 - minute*60; 

		if(second >= 60 ) 
		{ 
			second = second % 60; 
			minute+=second/60; 
		} 

		if(minute >= 60) 
		{ 
			minute = minute %60; 
			hour += minute/60; 
		} 

		if(hour >= 24)
		{
			hour = hour % 24;
			day += hour / 24;
		}

		if(0 <= day)
		{
			strTemp.Format(theApp.m_bChinese ? "%d天" : "%ddays ", day);
			str += strTemp;
		}

		if( 0 <= hour && hour <= 24)
		{
			strTemp.Format(theApp.m_bChinese ? "%d时" : "%02d:", hour);
			str += strTemp;
		}

		if(0 <= minute && minute <= 60)
		{
			strTemp.Format(theApp.m_bChinese ? "%d分" : "%02d:", minute);
			str += strTemp;
		}

		if(0 <= second && second <= 60)
		{
			strTemp.Format(theApp.m_bChinese ? "%d秒" : "%02d", second);
			str += strTemp;
		}	
	}
	return str.GetBuffer();
}


void CToolPeerListDlg::SetInfo(LIST_PEER_DATA& data)
{
	CString strShowData;
	GetDlgItem(IDC_VERSION)->SetWindowText(data.version.c_str());

	strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - data.lastrecv));
	strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - data.lastrecv).c_str();
	GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);

	strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - data.lastsend));
	strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - data.lastsend).c_str();
	GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);

	GetDlgItem(IDC_USERAGENT)->SetWindowText(data.subver.c_str());

	strShowData.Format("%lld", (data.banscore));
	GetDlgItem(IDC_BAN_SCORE)->SetWindowText(strShowData);


	strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - data.conntime));
	strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - data.conntime).c_str();
	GetDlgItem(IDC_CONNT_TIME)->SetWindowText(strShowData);

	strShowData.Format("%lld", (data.btyesrecv ));
	GetDlgItem(IDC_BYTERECV)->SetWindowText(strShowData);

	strShowData.Format("%lld", (data. bytessend));
	GetDlgItem(IDC_BYTESSEND)->SetWindowText(strShowData);

	strShowData.Format("%lld", (data.pingtime ));
	GetDlgItem(IDC_PINTTIME)->SetWindowText(strShowData);

	strShowData.Format("%llu", (data.startinghigh));
	GetDlgItem(IDC_STATIC_START_HEIGHT)->SetWindowText(strShowData);

	strShowData.Format("%llu", (data.startinghigh));
	GetDlgItem(IDC_SYNC_HEIGHT)->SetWindowText(strShowData);

	GetDlgItem(IDC_SERVICES)->SetWindowText(data.services.c_str());

	if(!data.inbound)
	{
		strShowData.Format("Outbound");
		GetDlgItem(IDC_DIRECTION)->SetWindowText(strShowData);
	}

	strShowData = data.addr.c_str();
	strShowData += "\n";
	strShowData += "via";
	strShowData += data.addrlocal.c_str();
	GetDlgItem(IDC_ADDRANDLOCALADDR)->SetWindowText(strShowData);


}

int CToolPeerListDlg::FindAddr(string strAddr)
{
	int nCount = m_lstPeerList.GetItemCount();

	for(int i = 0; i < nCount; i++)
	{
		if(strAddr == m_lstPeerList.GetItemText(i, 0).GetBuffer(0))
		{
			return  i;
		}

	}

	return -1;
}

void CToolPeerListDlg::InsertListData(int nRow, LIST_PEER_DATA& data)
{
	m_lstPeerList.InsertItem(nRow, data.addr.c_str());
	m_lstPeerList.SetItemText(nRow, 1, data.subver.c_str());
	CString strShowData;
	strShowData.Format("%lld", (data.pingtime));
	m_lstPeerList.SetItemText(nRow, 2,strShowData);
}

void CToolPeerListDlg::ModifyListData(int nRow, LIST_PEER_DATA& data)
{
	CString strShowData;
	m_lstPeerList.SetItemText(nRow, 1, data.subver.c_str());

	strShowData.Format("%lld", (data.pingtime));
	m_lstPeerList.SetItemText(nRow, 2,strShowData);
}

void CToolPeerListDlg::OnItemchangedListPeerlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int nRow = pNMLV->iItem;

	if(nRow < 0)
		return;

	//m_lstPeerList.SetItemState(nRow, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED); 
	//m_lstPeerList.SetSelectionMark(nRow);

	map<string,LIST_PEER_DATA>::iterator it;

	//POSITION pos = m_lstPeerList.GetFirstSelectedItemPosition() ;
	//int nRow = m_lstPeerList.GetNextSelectedItem(pos);
	m_nIndex = nRow;
	CString source =m_lstPeerList.GetItemText(nRow, 0);
	CString strShowData;
	for(it = m_PeerListInfo.begin();it != m_PeerListInfo.end();++it)
	{
		if(strcmp(it->first.c_str(), source))
		{
			//GetDlgItem(IDC_VERSION)->SetWindowText(it->second.version.c_str());

			///*strShowData.Format("%llu", (it->second.lastsend ));
			//GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);*/

			//strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastrecv));
			//strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastrecv).c_str();
			//GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);

			//strShowData.Format("%llu", (it->second.startinghigh));
			//GetDlgItem(IDC_STATIC_START_HEIGHT)->SetWindowText(strShowData);

			///*strShowData.Format("%llu", (it->second.lastrecv));
			//GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);*/
			//strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastsend));
			//strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastsend).c_str();
			//GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);

			//strShowData.Format("%llu", (it->second.btyesrecv));
			//GetDlgItem(IDC_BYTERECV)->SetWindowText(strShowData);


			//strShowData.Format("%llu", (it->second.pingtime));
			//GetDlgItem(IDC_PINTTIME)->SetWindowText(strShowData);

			CString strShowData;
			GetDlgItem(IDC_VERSION)->SetWindowText(it->second.version.c_str());

			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastrecv));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastrecv).c_str();
			GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);

			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastsend));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastsend).c_str();
			GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);

			GetDlgItem(IDC_USERAGENT)->SetWindowText(it->second.subver.c_str());

			strShowData.Format("%lld", (it->second.banscore));
			GetDlgItem(IDC_BAN_SCORE)->SetWindowText(strShowData);


			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.conntime));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.conntime).c_str();
			GetDlgItem(IDC_CONNT_TIME)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second.btyesrecv ));
			GetDlgItem(IDC_BYTERECV)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second. bytessend));
			GetDlgItem(IDC_BYTESSEND)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second.pingtime ));
			GetDlgItem(IDC_PINTTIME)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.startinghigh));
			GetDlgItem(IDC_STATIC_START_HEIGHT)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.startinghigh));
			GetDlgItem(IDC_SYNC_HEIGHT)->SetWindowText(strShowData);



			GetDlgItem(IDC_SERVICES)->SetWindowText(it->second.services.c_str());

			if(!it->second.inbound)
			{
				strShowData.Format("Outbound");
				GetDlgItem(IDC_DIRECTION)->SetWindowText(strShowData);
			}

			strShowData = it->second.addr.c_str();
			strShowData += "\n";
			strShowData += "via";
			strShowData += it->second.addrlocal.c_str();
			GetDlgItem(IDC_ADDRANDLOCALADDR)->SetWindowText(strShowData);

		}
	}

	*pResult = 0;
}

void CToolPeerListDlg::GetPeerlist(map<string,LIST_PEER_DATA>& PeerListInfo)
{
	string strcom =strprintf("%s","getpeerinfo");
	string strSendData,strretshow;
	RPCCommandToJson(strcom,strSendData);
	CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);

	TRACE("=======%s\n", strretshow);
	//ShowJson(strcom,strretshow);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strretshow, root)) 
		return ;

	int size = root.size();

	LIST_PEER_DATA peerlist;

	
	for ( int index =0; index < size; ++index )
	{
		static char buffer[500];
		sprintf_s( buffer, "[%d]", index );
		string name(buffer);

		Json::Value arr = root[index];

		peerlist.addr = arr["addr"].asString();
		peerlist.addrlocal = arr["addrlocal"].asString();
		peerlist.banscore = arr["banscore"].asUInt64();
		peerlist.btyesrecv = arr["bytesrecv"].asUInt64();
		peerlist.bytessend = arr["bytessent"].asUInt64();
		peerlist.conntime = arr["conntime"].asUInt64();
		peerlist.inbound = arr["inbound"].asBool();
		peerlist.lastrecv = arr["lastrecv"].asUInt64();
		peerlist.lastsend = arr["lastsend"].asUInt64();
		peerlist.pingtime = arr["pingtime"].asUInt64();
		peerlist.services = arr["services"].asString();
		peerlist.banscore = arr["banscore"].asUInt64();
		peerlist.services = arr["services"].asString();
		peerlist.inbound = arr["banscore"].asBool();

		peerlist.startinghigh = strcmp(arr["startingheight"].asString().c_str(), "-1") ? arr["startingheight"].asUInt64() : 0;

		peerlist.subver = arr["subver"].asString();
		peerlist.syncnode = arr["syncnode"].asBool();
		peerlist.version = arr["version"].asString();

		PeerListInfo.insert(map<string, LIST_PEER_DATA >::value_type(arr["addr"].asString(), peerlist));
	}
}


void CToolPeerListDlg::OnNMClickListPeerlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nRow = pNMItemActivate->iItem;

	if(nRow < 0)
		return;

	m_lstPeerList.SetItemState(nRow, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED); 
	m_lstPeerList.SetSelectionMark(nRow);

	map<string,LIST_PEER_DATA>::iterator it;

	//POSITION pos = m_lstPeerList.GetFirstSelectedItemPosition() ;
	//int nRow = m_lstPeerList.GetNextSelectedItem(pos);
	m_nIndex = nRow;
	CString source =m_lstPeerList.GetItemText(nRow, 0);
	CString strShowData;
	for(it = m_PeerListInfo.begin();it != m_PeerListInfo.end();++it)
	{
		if(!strcmp(it->first.c_str(), source))
		{
			/*GetDlgItem(IDC_VERSION)->SetWindowText(it->second.version.c_str());

			strShowData.Format("%llu", (it->second.lastsend ));
			GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.startinghigh));
			GetDlgItem(IDC_STATIC_START_HEIGHT)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.lastrecv));
			GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.btyesrecv));
			GetDlgItem(IDC_BYTERECV)->SetWindowText(strShowData);


			strShowData.Format("%llu", (it->second.pingtime));
			GetDlgItem(IDC_PINTTIME)->SetWindowText(strShowData);*/

			CString strShowData;
			GetDlgItem(IDC_VERSION)->SetWindowText(it->second.version.c_str());

			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastrecv));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastrecv).c_str();
			GetDlgItem(IDC_LASTRECV)->SetWindowText(strShowData);

			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.lastsend));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.lastsend).c_str();
			GetDlgItem(IDC_LASTSEND)->SetWindowText(strShowData);

			GetDlgItem(IDC_USERAGENT)->SetWindowText(it->second.subver.c_str());

			strShowData.Format("%lld", (it->second.banscore));
			GetDlgItem(IDC_BAN_SCORE)->SetWindowText(strShowData);


			strShowData.Format("%lld", (CTime::GetCurrentTime().GetTime() - it->second.conntime));
			strShowData = Second2HMS(CTime::GetCurrentTime().GetTime() - it->second.conntime).c_str();
			GetDlgItem(IDC_CONNT_TIME)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second.btyesrecv ));
			GetDlgItem(IDC_BYTERECV)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second. bytessend));
			GetDlgItem(IDC_BYTESSEND)->SetWindowText(strShowData);

			strShowData.Format("%lld", (it->second.pingtime ));
			GetDlgItem(IDC_PINTTIME)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.startinghigh));
			GetDlgItem(IDC_STATIC_START_HEIGHT)->SetWindowText(strShowData);

			strShowData.Format("%llu", (it->second.startinghigh));
			GetDlgItem(IDC_SYNC_HEIGHT)->SetWindowText(strShowData);

			GetDlgItem(IDC_SERVICES)->SetWindowText(it->second.services.c_str());

			if(!it->second.inbound)
			{
				strShowData.Format("Outbound");
				GetDlgItem(IDC_DIRECTION)->SetWindowText(strShowData);
			}

			strShowData = it->second.addr.c_str();
			strShowData += "\n";
			strShowData += "via";
			strShowData += it->second.addrlocal.c_str();
			GetDlgItem(IDC_ADDRANDLOCALADDR)->SetWindowText(strShowData);

			break;

		}
	}
	*pResult = 0;
}

void CToolPeerListDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	map<string,LIST_PEER_DATA> tmp_map;
	GetPeerlist(tmp_map);

	vector<string> del_key;
	map<string,LIST_PEER_DATA>::iterator it;
	for(it = m_PeerListInfo.begin();it != m_PeerListInfo.end();++it)
	{
		map<string,LIST_PEER_DATA>::iterator found = tmp_map.find(it->first);

		if(found != tmp_map.end())
		{
			it->second.addr = found->second.addr;
			it->second.addrlocal = found->second.addrlocal;
			it->second.banscore = found->second.banscore;
			it->second.btyesrecv = found->second.btyesrecv;

			it->second.bytessend = found->second.bytessend;
			it->second.lastrecv = found->second.lastrecv;
			it->second.lastsend = found->second.lastsend;

			it->second.conntime = found->second.conntime;
			it->second.inbound = found->second.inbound;
			it->second.pingtime = found->second.pingtime;
			it->second.services = found->second.services;
			it->second.startinghigh = found->second.startinghigh;
			it->second.subver = found->second.subver;
			it->second.syncnode = found->second.syncnode;
			it->second.version = found->second.version;


			int nRow = FindAddr(it->second.addr);
			if(nRow >= 0)
			{
				ModifyListData(nRow, it->second);
			}

			tmp_map.erase(found);
		}
		else
		{
			del_key.push_back(it->first);
		}
	}

	int nDelSize = del_key.size();
	for(int i = 0; i < nDelSize; i++)
	{
		map<string,LIST_PEER_DATA>::iterator found = m_PeerListInfo.find(del_key[i]);
		if(found != m_PeerListInfo.end())
		{
			m_PeerListInfo.erase(found);
			int nRow = FindAddr(del_key[i]);
			if(nRow >= 0)
			{
				m_lstPeerList.DeleteItem(nRow);
			}
		}
	}

	for(it = tmp_map.begin();it != tmp_map.end();++it)
	{
		map<string,LIST_PEER_DATA>::iterator found = m_PeerListInfo.find(it->first);
		if(found == m_PeerListInfo.end())
		{
			m_PeerListInfo.insert(map<string, LIST_PEER_DATA >::value_type(it->first, it->second));

			int nListCount = m_lstPeerList.GetItemCount();
			InsertListData(nListCount, it->second);
		}
	}

	POSITION pos = m_lstPeerList.GetFirstSelectedItemPosition() ;
	int nRow = m_lstPeerList.GetNextSelectedItem(pos);
	if(nRow < 0)
	{
		nRow = 0;
		m_lstPeerList.SetItemState(nRow, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
		m_lstPeerList.SetSelectionMark(nRow);
	}

	CString strAddr = m_lstPeerList.GetItemText(nRow, 0);
	map<string,LIST_PEER_DATA>::iterator found = m_PeerListInfo.find(strAddr.GetBuffer(0));
	if(found != m_PeerListInfo.end())
	{
		SetInfo(found->second);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CToolPeerListDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(TIMER_PEERINFO);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CToolPeerListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{  

		switch(pMsg->wParam) 
		{  

		case VK_RETURN: //回车   
			return TRUE;  

		case VK_ESCAPE: //ESC  
			return TRUE;  

		}  

	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}
