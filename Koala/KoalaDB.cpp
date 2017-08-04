#include "stdafx.h"
#include "Koala.h"

void CKoalaApp::UpdateAppRecord(string txdetail){
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{	/*	
		if(!strcmp(transcion.desregid.c_str(),theApp.m_strFreeze5MonthId.c_str()) 
			|| !strcmp(transcion.desregid.c_str(),theApp.m_strFreeze10MonthId.c_str()) 
			|| !strcmp(transcion.desregid.c_str(),theApp.m_strFreeze20MonthId.c_str()))
		{
			PopupContactBalloonTip(transcion);
		}
		*/
	}
}


//弹出交易冒泡提示
void CKoalaApp::PopupContactBalloonTip(uistruct::REVTRANSACTION_t tx)
{
	if (theApp.m_pMainWnd == NULL)
	{
		return;
	}
	string strCond;
	strCond = strprintf(" address='%s' ", tx.addr);
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"), strCond) ;
	if (nItem == 0)
	{
		return;
	}

	string strShow = theApp.m_bChinese ? "领币\r\n" : "receive coins\r\n";
	
	string nValue = tx.Contract;
	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
	if (vTemp[0] == 0xff)
	{

		if (vTemp[1] == 0x02)    /// 充值
		{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
			strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n", theApp.m_bChinese ? "确认时间" : "confirmed time", curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			strShow+=strprintf("%s:  %.8f\r\n",theApp.m_bChinese ? "金额" : "Amount", tx.money );
			strShow+=strprintf("%s:  %s\r\n",theApp.m_bChinese ? "交易类型" : "Transaction Type",theApp.m_bChinese ? "充  值" : "Charge");
			strShow+=strprintf("%s:  %s\r\n",theApp.m_bChinese ? "地址" : "Address",tx.regid );
		}else if (vTemp[1] == 0x03 ||vTemp[1] == 0x01 )   /// 提现
		{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
			strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",theApp.m_bChinese ? "确认时间" : "confirmed time",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			if (vTemp.size() == sizeof(APPACC))
			{
				APPACC drawtx;
				memcpy(&drawtx, &vTemp[0],sizeof(APPACC));
				strShow+=strprintf("%s:  %.8f\r\n", theApp.m_bChinese ? "金额" : "Amount", (drawtx.money*1.0)/COIN );
				strShow+=strprintf("%s:  %s\r\n", theApp.m_bChinese ? "交易类型": "Transaction Type", theApp.m_bChinese ?  "提  现" : "Withdraw");
				strShow+=strprintf("%s:  %s\r\n", theApp.m_bChinese ? "地址" : "Address", tx.regid );
			}else{
				strShow="";
			}
		}
	}
	::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)strShow.c_str());
}


void CKoalaApp::PopupCommBalloonTip(string hash)
{
	string conditon;
	conditon =strprintf("hash='%s'",hash);
	uistruct::REVTRANSACTION_t commtx;
	theApp.m_SqliteDeal.GetTransactionItem(conditon,&commtx);
	if (commtx.txhash == "" || commtx.confirmedHeight ==0 || theApp.m_pMainWnd == NULL)
	{
		return;
	}
	string strShow = "";
	if (strcmp(commtx.txtype.c_str(),"REWARD_TX") == 0)
	{
		//strShow ="  流入交易\r\n";
		strShow =strprintf("  %s\r\n",theApp.m_bChinese ? "流入交易" : "Income transaction");

		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n", theApp.m_bChinese ? "确认时间" : "confirmed time", curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: +%.8f\r\n", theApp.m_bChinese ? "金额" : "Amount", commtx.money);
		strShow += strprintf("%s: %s\r\n",theApp.m_bChinese ? "交易类型": "Transaction Type", theApp.m_bChinese ? "挖矿" : "Mining");
		strShow += strprintf("%s: (%s)\r\n", theApp.m_bChinese ? "地址" : "Address", commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 1){
		//strShow ="  流出交易\r\n";
		strShow =strprintf("  %s\r\n", theApp.m_bChinese ? "流出交易" : "Outflow transaction");
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",theApp.m_bChinese ? "确认时间" : "confirmed time", curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: -%.8f\r\n", theApp.m_bChinese ? "金额": "Amount", commtx.money);
		strShow += strprintf("%s: %s\r\n", theApp.m_bChinese ? "交易类型": "Transaction Type", theApp.m_bChinese ? "转账" : "Transfer");
		strShow += strprintf("%s: (%s)\r\n", theApp.m_bChinese ? "地址" : "Address", commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 2){
		//strShow ="  流入交易\r\n";
		strShow =strprintf("  %s\r\n", theApp.m_bChinese ? "流入交易" : "Income transaction");
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",theApp.m_bChinese ? "确认时间" : "confirmed time",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: +%.8f\r\n", theApp.m_bChinese ? "金额" : "Amount", commtx.money);
		strShow += strprintf("%s: %s\r\n",theApp.m_bChinese ? "交易类型": "Transaction Type",theApp.m_bChinese ? "接受于" : "Accepted");
		strShow += strprintf("%s: (%s)\r\n",theApp.m_bChinese ? "地址" : "Address", commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 3){
		//strShow =" 转账交易\r\n";
		strShow =strprintf("  %s\r\n", theApp.m_bChinese ? "转账交易" : "Transfer transaction");
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",theApp.m_bChinese ? "确认时间" : "confirmed time",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: %.8f\r\n",theApp.m_bChinese ? "金额" : "Amount",commtx.money);
		strShow += strprintf("%s: %s\r\n",theApp.m_bChinese ? "交易类型": "Transaction Type", theApp.m_bChinese ?  "平账" : "Wallet Transfer");
		strShow += strprintf("%s: %s\r\n",theApp.m_bChinese ? "地址" : "Address", commtx.addr.c_str());
	}
	if(strShow != "")
	::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)strShow.c_str());
}


void CKoalaApp::InsertTransaction(string hash)
{
	string strCommand,strShowData;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str() );
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd gettxdetail error");
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{

		if (!strcmp("COMMON_TX",transcion.txtype.c_str()))
		{
			string conditon;
			conditon = strprintf("address = '%s'",transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			conditon = strprintf("address = '%s'",transcion.desaddr.c_str());
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&&nItem != 0)
			{
				transcion.state = 3;      /// 平
			}else if (nItem != 0)
			{
				transcion.state = 1;        //// 扣钱
			}else if (nItem1 != 0)
			{
				transcion.state = 2;        ////加钱
			}
		}

		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d" , transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData ) ;

		//// 数据插入了，更新到交易详情界面
		CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_INSERT);
		string temp =  transcion.ToJson();
		Postmsg.SetData(temp.c_str());	
		m_msgUITxDetailDlgQueue.push(Postmsg);
		DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_TRANSRECORD_UI ,WM_INSERT,0);
		PopupCommBalloonTip(transcion.txhash);
	}
}
void  CKoalaApp::InsertAddrBook(uistruct::ADDRBOOK_t addr)
{
	string strSourceData;
	strSourceData = strprintf("'%s' , '%s' " , addr.label.c_str(), addr.address.c_str() ) ;
	m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) ;
}

void  CKoalaApp::UpdateAddrBook(uistruct::ADDRBOOK_t addr)
{
	string strSourceData,strWhere;
	strSourceData = strprintf("Label = '%s'" ,addr.label.c_str() ) ;
	strWhere =strprintf("address = '%s'" ,addr.address.c_str() ) ;
	if ( !m_SqliteDeal.UpdateTableItem(_T("t_address_book") , strSourceData , strWhere)){
		TRACE(_T("t_address_book数据更新失败!") );
	}
}

void CKoalaApp::SyncTransaction(string obj)
{
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(obj))
	{
		/*CString strCond;
		strCond.Format(_T(" hash = '%s' "), transcion.txhash);
		int txitem = theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond);
		if (txitem != 0)
		{
			return;
		}*/

		transcion.state = 0;
		if (!strcmp("COMMON_TX",transcion.txtype.c_str()))
		{
			string conditon;
			conditon = strprintf("address = '%s'",transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			conditon = strprintf("address = '%s'",transcion.desaddr.c_str());
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&&nItem != 0)
			{
				transcion.state = 3; 
			}else if (nItem != 0)
			{
				transcion.state = 1;        //// 扣钱
			}else if (nItem1 != 0)
			{
				transcion.state = 2;        ////加钱
			}
		}
		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d", transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData );

		PopupCommBalloonTip(transcion.txhash);
	}
}

void CKoalaApp::DeleteItemTransaction(int height)
{
	string strCondition =strprintf("confirm_height >=%d or confirm_height=0",height);
	m_SqliteDeal.DeleteTableItem(_T("t_transaction"),strCondition);
}

void CKoalaApp::UpdateAddressData(){
	string strCommand;
	strCommand =strprintf("%s","listaddr");

	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}


	for(unsigned int i = 0; i < root.size(); ++i){
		//memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		uistruct::LISTADDR_t listaddr;
		//address
		listaddr.address = root[i]["addr"].asString();
		//RegID
		listaddr.RegID = root[i]["regid"].asString();
		//金额
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//是否支持冷挖矿
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//是否注册GetLength();
		string strSign ;
		strSign = strprintf("%s" ,listaddr.RegID.c_str() ) ;
		if ( 3 <= strSign.length() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}
		string strSourceData,feild;
		feild="address";
		strSourceData = strprintf("%s",listaddr.address.c_str());

		string strCond;
		strCond =strprintf(" address = '%s' ", listaddr.address.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			strSourceData=strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , listaddr.address.c_str() ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label.c_str() ) ;
			if (!m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
			{
				TRACE("Insert t_wallet_address error!\n");
			}

			string Temp = listaddr.ToJson();
			SendUIMessage((int)WM_UP_NEWADDRESS,Temp.c_str());
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				string strSourceData,strWhere;
				strSourceData = strprintf("reg_id = '%s', money = %.8f ,cold_dig =%d,sign =%d",listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere = strprintf("address = '%s'", listaddr.address.c_str()  ) ;
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}

				listaddr.Label=addrsql.Label;
				string Temp = listaddr.ToJson();
				SendUIMessage((int)WM_UP_ADDRESS,Temp);
			}
		}
	}
}

void CKoalaApp::UpdateTransaction(string hash){

	string strCommand;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str());
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateTransaction rpccmd gettxdetail error");
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{
		string strSourceData,strWhere;
		strSourceData = strprintf("confirm_height = %d , confirmed_time = %d ,block_hash ='%s'" ,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
		strWhere = strprintf("hash = '%s'" , hash.c_str()) ;
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_transaction") , strSourceData , strWhere ) ){
			TRACE(_T("update t_transaction failed\n"));
		}

		//// 数据更新了，更新到交易详情界面
		CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_UPDATE);
		string temp =  transcion.ToJson();
		Postmsg.SetData(temp.c_str());	
		m_msgUITxDetailDlgQueue.push(Postmsg);
		DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_TRANSRECORD_UI ,WM_UPDATE,0);
	}
	PopupCommBalloonTip(hash);
}