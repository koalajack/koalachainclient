#include "StdAfx.h"
#include "RpcCmd.h"
#include "SynchronousSocket.h"
#include "json\reader.h"
#include "json\value.h"
#include "UiHead.h"
#include "Koala.h"

CRpcCmd::CRpcCmd(void)
{
	mIp = "127.0.0.1";
	mPort = 18332;

	if (theApp.m_nNetWorkType == MAIN)
	{
		mPort = 5964 ; 
	}
	else if (theApp.m_nNetWorkType == TESTNET || theApp.m_nNetWorkType == REGTEST)
	{
		mPort = 15964; 
	}
}



CRpcCmd::~CRpcCmd(void)
{
}
int CRpcCmd::SendContactRpc(string cmd,string &rev){

	string strSendData = "";
	strSendData +=m_sendPreHeadstr;
	char buffer[100] = {0};
	sprintf_s(buffer,"%d",cmd.length());
	strSendData +="Content-Length: ";
	strSendData+=buffer;
	strSendData +="\r\n";
	strSendData +=m_sendendHeadstr;
	strSendData +=cmd;

//	std::shared_ptr<char> pChRev;
	std::string strRecv;
	if(CSynchronousSocket::GetRpcRes(mIp, mPort, strSendData, strRecv,7000)>0)
	{
		const char *cPos = strstr(strRecv.c_str(), "Server:");
		if(NULL != cPos) 
		{
			cPos = strchr(cPos, '\n');
			if(NULL != cPos) 
			{
				rev = cPos;
			}
		}

	}	
	return rev.length();
}

int CRpcCmd::SendRpc(string strCommand,string &rev)
{
	string strSendData;
	BuildSendString(strCommand,strSendData);

	TRACE("========================================%s", strSendData.c_str());
	
//	std::shared_ptr<char> pChRev;
	std::string strRecv;
	if(CSynchronousSocket::GetRpcRes(mIp,mPort,strSendData,strRecv,7000)>0)
	{
		const char * cPos = strstr(strRecv.c_str(), "Server:");
		if(NULL != cPos) 
		{
			cPos = strchr(cPos, '\n');
			if(NULL != cPos) 
			{
				rev = cPos;
			}
		}
		/*	
		int pos = teprev.find("Server:");
		if (pos >=0)
		{
			pos = teprev.find('\n',pos);
			if (pos >=0)
			{
				rev = teprev.substr(pos);
			}
		}
		*/
	}	
	return rev.length();
}
static bool IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		//if (!isdigit(pData[i]))
		//	return false;
		if (!(pData[i] >= '0' && pData[i] <='9'))
		{
			return false;
		}
	}

	return true;
}

void CRpcCmd:: BuildSendString(const string &cmd,string &sendStr)
{
   string sendStrte;
   RPCCommandToJson(cmd,sendStrte);
  // RpcJosnStr = sendStrte;
 //  RpcJosnStr.Format(_T("%s"),sendStrte);
   //string tepsend = sendStrte.GetString();
   //string nSendStr = "";
   //nSendStr += m_sendPreHeadstr;
   //CStringA nMidStr = "Content-Length: ";
   //nMidStr.AppendFormat("%d\r\n",tepsend.length());
   //nSendStr += nMidStr.GetString();
   //nSendStr += m_sendendHeadstr.GetString();
   //nSendStr += sendStrte;
   //sendStr = nSendStr.c_str();

   //CStringA nSendStr = _T("");
   //nSendStr.AppendFormat(_T("%s"),m_sendPreHeadstr);
   //CStringA nMidStr = "Content-Length: ";
   //nMidStr.AppendFormat("%d\r\n",sendStrte.length());
   //nSendStr.AppendFormat(_T("%s"),nMidStr);
   //nSendStr.AppendFormat(_T("%s"),m_sendendHeadstr);
   //nSendStr.AppendFormat(_T("%s"),sendStrte);
  // sendStr=nSendStr;

   sendStr +=m_sendPreHeadstr;
   char buffer[100] = {0};
   sprintf_s(buffer,"%d",sendStrte.length());
   sendStr +="Content-Length: ";
   sendStr+=buffer;
   sendStr +="\r\n";
   sendStr +=m_sendendHeadstr;
   sendStr +=sendStrte;
}

//void CRpcCmd::RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData)
//{
//	USES_CONVERSION;
//	char* pData = W2A(A2CW((LPCSTR)strRPCCommand));
//	CStringA rpcCommand(pData);
//
//	Json::Value root;
//	Json::Value arrayObj;  
//	Json::Value item;
//
//	int pos = rpcCommand.Find(" ");
//	CStringA method;
//	CStringA param;
//	if (pos >=0)
//	{
//		method = rpcCommand.Left(pos);
//		rpcCommand = rpcCommand.Mid(pos + 1);
//
//	}
//	else if (!rpcCommand.IsEmpty())
//	{
//		root["method"] = rpcCommand.GetString();
//		root["params"]=arrayObj;
//	}
//	if (method != "")
//	{
//		root["method"] = method.GetString();
//		pos = rpcCommand.Find(" ");
//		while(pos >=0)
//		{
//			param = rpcCommand.Left(pos);
//			param.TrimLeft();
//			param.TrimRight();
//			rpcCommand = rpcCommand.Mid(pos+1);
//			rpcCommand = rpcCommand.TrimLeft();
//			if (param.GetLength()<10&&IsAllDigtal(param))
//			if (IsAllDigtal(param))
//			{
//				INT64 nparam;
//				sscanf_s(param,"%lld",&nparam);
//				root["params"].append(nparam);
//			}
//			else
//			{
//				root["params"].append(param.GetString());
//			}
//			pos = rpcCommand.Find(" ");
//		}
//
//		if (IsAllDigtal(rpcCommand) && (method != _T("gethash"))) //&& rpcCommand.GetLength() != 12)
//		{
//			INT64 param;
//			sscanf_s(rpcCommand,"%lld",&param);
//			root["params"].append(param);
//		}
//		else
//		{
//			if (rpcCommand == "true")
//			{
//				root["params"].append(true);
//			}
//			else if (rpcCommand == "false")
//			{
//				root["params"].append(false);
//			}
//			else
//			{
//				root["params"].append(rpcCommand.GetString());
//			}
//
//		}
//	}
//
//	strSendData = root.toStyledString().c_str();
//}

void CRpcCmd::RPCCommandToJson(const string& strRPCCommand,string& strSendData)
{

	Json::Value root;
	Json::Value arrayObj;  
	Json::Value item;

	string rpcCommand = strRPCCommand;
	int pos = rpcCommand.find(" ");
	string method;
	string param;
	if (pos >=0)
	{
		method = rpcCommand.substr(0,pos);
		rpcCommand = rpcCommand.substr(pos + 1);

	}
	else if (!rpcCommand.empty())
	{
		root["method"] = rpcCommand;
		root["params"]=arrayObj;
	}
	if (method != "")
	{
		root["method"] = method;
		pos = rpcCommand.find(" ");
		while(pos >=0)
		{
			param = rpcCommand.substr(0,pos);
			param =UiFun::trimleft(param);
			param =UiFun::trimright(param);
		
			rpcCommand = rpcCommand.substr(pos+1);
			rpcCommand = UiFun::trimleft(rpcCommand);
			//if (param.GetLength()<10&&IsAllDigtal(param))
			if (IsAllDigtal(param.c_str()))
			{
				INT64 nparam;
				sscanf_s(param.c_str(),"%lld",&nparam);
				root["params"].append(nparam);
			}
			else
			{
				if (param == "true" || param == "TRUE")
				{
					root["params"].append(true);
				}
				else if (param == "false" || param == "FALSE")
				{
					root["params"].append(false);
				}
				else
				{
					root["params"].append(param);
				}
			}
			pos = rpcCommand.find(" ");
		}

		if (IsAllDigtal(rpcCommand.c_str()) && (method !="verifymessage")&&(method != _T("gethash"))) //&& rpcCommand.GetLength() != 12)
		{
			INT64 param;
			sscanf_s(rpcCommand.c_str(),"%lld",&param);
			root["params"].append(param);
		}
		else
		{
			if (rpcCommand == "true"|| param == "TRUE")
			{
				root["params"].append(true);
			}
			else if (rpcCommand == "false" || param == "FALSE")
			{
				root["params"].append(false);
			}
			else
			{
				root["params"].append(rpcCommand);
			}

		}
	}

	strSendData = root.toStyledString();
}