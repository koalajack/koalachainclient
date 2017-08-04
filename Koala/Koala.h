
// Koala.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "tinyformat.h"
#include "HSocket.h"
#include "GIFControl.h"
#include <vector>
#include <map>
#include <list>
#include "config.h"
#include "tinyformat.h"
#include "NoUiMsgBuffer.h"
#include "MyQueue.h"
#include "Log.h"
#include "SqliteDeal.h"
#include "JsonConfigHelp.h"
#include "SplashThread.h"
#include "MessageBoxEx.h"

// CKoalaApp:
// �йش����ʵ�֣������ Koala.cpp
//
#define strprintf tfm::format

enum ENU_SHOW_TYPE
{
	SHOW_ENT,
	SHOW_ADMIN
};



typedef struct _AMADDR
{
	CString		strAddr;		//��ַ
	double		fMoney;		    //���
	_AMADDR()
	{
		strAddr = "";
		fMoney = 0.0;
	}
}AMADDR;

class CKoalaApp : public CWinApp
{
public:
	CKoalaApp();
	DECLARE_MESSAGE_MAP()
// ��д
public:
	virtual BOOL InitInstance();
	UINT					gsLanguage            ;   //��������
	//string					str_InsPath	;
	string					m_strInsPath			;
	string					m_strModuleFilename	;  
	BOOL                    m_bReIndexServer;
	CString                 m_strAddress;
	vector<AMADDR>			m_verAMAddr;
	void   GetMoFilename( string &path , string &filename ) ;		// ��ȡ�ļ�����·��
	BOOL					m_bOutApp     ;     //�˳����
public:
	HANDLE	        GetMtHthrd() const { return hMtThrd; }  
	unsigned int	GetMtHthrdId() const { return nMtThrdID; }  
public:
	CRITICAL_SECTION		cs_MsgDisptch ; 
public:
	HANDLE		 hMtThrd				;	
	HANDLE		 hMtStartEvent			;	
	unsigned int nMtThrdID				;	
	BOOL		 CreateMaintainThrd()	;	
	static UINT __stdcall MtProc(LPVOID pParam) ;
public:
	std::vector<MSG_NOTIFY> sSubscribeList ;
	std::vector< sThrd > v_ProcSubThrd ;
public:
	void RegThrdToMtThrd(  HANDLE hThread , unsigned int hThrdId ) ;	
	void UnRegThrdToMtThrd(HANDLE hThread , unsigned int hThrdId ) ;
	void SubscribeMsg( DWORD nThreadId , HWND hWnd , UINT msg , void * relMsgMem = NULL )	;
	void UnSubscribeMsg( DWORD , HWND , UINT )	;
	void DispatchMsg( unsigned int threadID , UINT msg , WPARAM wParam , LPARAM lParam ) ;

public:
	string      m_betScritptid;
	string      m_ipoScritptid;
	string      m_darkScritptid;
	string      m_redPacketScriptid;
	CDarkTxCfg		m_DarkCfg;
	CP2PBetCfg		m_P2PBetCfg;
	CRedPacketCfg   m_RedPacketCfg;
	CNewestScriptCfg m_neststcriptid;

	CRedPacketStepCfg m_redPackestep;
	CP2PBetStepCfg m_p2pbetstep;

	CBussinessStepCfg m_bussStep;
	CBussinessCfg m_BussCfg;

//public:
//	void         OnInitList();   //��ʼ��list
//	int          SendPostThread(DWORD msgtype);
//	void         ParseUIConfigFile(const string& strExeDir);
public:
	void StartSeverProcess(const string& strdir);
	void CloseProcess(const string& exename);
	PROCESS_INFORMATION		sever_pi; 
	bool Update();

	BOOL IsLockWallet();                      //Ǯ���Ƿ����
	void OnCheckSeverfile();                  //������core�����Ƿ����
	void CheckPathValid(const string& strDir);//У�鹤��Ŀ¼�Ƿ��пո����ĵȷǷ�Ŀ¼

	void StartWorkThread();											//�������ݴ����߳� 
	static UINT __stdcall ReceiveMessageFromServer(LPVOID pParam);  //������Ϣ�ӷ����
	static UINT __stdcall PraseJsonToMessage(LPVOID pParam);        //����Json����Ϊmessage
	static UINT __stdcall ProcessMessage(LPVOID pParam);            //������Ϣ
	static UINT __stdcall CheckUpdate(LPVOID pParam);               //�������Ƿ��и���

	void ParseUIConfigFile(const string& strExeDir);             //�������������ļ�
public:

	//���ݿ����
	void UpdateAppRecord(string txdetail);						 //����APP����״̬
	void PopupContactBalloonTip(uistruct::REVTRANSACTION_t tx);  //ð�ݵ�����Լ������ʾ
	void InsertTransaction(string hash);						 //�����µĽ���
	void PopupCommBalloonTip(string hash);                       //ð�ݵ�����ͨ������ʾ
	void InsertAddrBook(uistruct::ADDRBOOK_t addr);              //��ַ�������û���ַ
	void UpdateAddrBook(uistruct::ADDRBOOK_t addr);              //��ַ�������û���ַ
	void SyncTransaction(string obj);                            //ͬ������
	void DeleteItemTransaction(int height);                      //ɾ������
	void InsertAppTransaction(string txdetail);                  //������Լ����
	void UpdateAddressData();                                    //���µ�ַ��Ϣ
	void UpdateTransaction(string hash);						 //���½�����Ϣ
	BOOL RunOnlyOneApp();

public:
	UINT         m_uReceiveThreadId;        //������Ϣ�߳�ID
	HANDLE       m_hReceiveThread;          //������Ϣ�߳̾��

	UINT         m_uPraseJsonToMsgThreadId; //����JsonΪ��Ϣ�߳�ID
	HANDLE       m_hPraseJsonToMsgThread;   //����JsonΪ��Ϣ�߳̾��

	UINT         m_uProcessMsgThreadId;    //������Ϣ�߳�ID
	HANDLE       m_hProcessMsgThread;      //������Ϣ�߳̾��

	UINT         m_uCheckUpDateThreadId;   //����������߳�ID
	HANDLE       m_hCheckUpdateThread;     //����������߳̾��

	BOOL         m_bIsCloseAppFlag;        //����ر��˳���־
	BOOL         m_bIsServerClose;         //������core����رձ�־
	BOOL         m_bIsWalletLocked;        //Ǯ�����ܱ�־
	BOOL         m_bIsUpdateCheckRunSwitch;//���¼���߳̿��ر�־  
	BOOL		 m_bIsProcessMsgRunSwitch; //������Ϣ�߳̿��ر�־  
	BOOL         m_bIsReceiveMsgRunSwitch; //���շ���������߳̿��ر�־
	BOOL         m_bIsHaveLocked;          //Ǯ���Ƿ�����״̬
	BOOL         m_bIsPassLock;

	BOOL	     m_bIsDlgCreateFinished;   //���Ի���������dlg�Ƿ񴴽���
	BOOL         m_bIsStartMainDlg;        //�Ƿ��������Ի���
	BOOL         m_bIsSyncTx;              //�Ƿ�ͬ������
	BOOL		 m_bIsCommitDb;            //�Ƿ��ύSQLִ�е�DB��־
	BOOL	     m_bIsSyncBlock;           //������ͬ��block��־

	string		  m_strProgressGifFile;	   //��ȡ����ڵ�GIF�����ļ�
	string		  m_strProgressOutGifFile; //�˳�����GIF�����ļ�

	int          m_nNetWorkType;           //����״̬��0-��main����(��ʽ����) 1-regtest����(��������) 2-testnet(��������) 
	int          m_nLockStatus;            //����״̬
	CNoUiMsgBuffer m_noUiMsgBuffer;        //���շ������Ϣ

	CHSocket*    m_pSocket ;			   //Socketrk����
	string		 m_rpcPort;                //RPC�˿�
	string		 m_strServerCfgFileName;   //�����������ļ���
	string		 m_sendPreHeadstr;         //����RPC�����ͷ
	string		 m_sendEndHeadstr;         //����RPC�����β
	string       m_uiPort;                 //���ܷ�����������Ϣ�˿�
	string       m_severIp;                //����Զ�̷�����IP
	string       m_rpcUser;                //RPC�����û�����Զ�̷���ʱ��Ҫ��
	string       m_rpcPassWord;			   //RPC��������(Զ�̷���ʱ��Ҫ)

	SOCKET       m_uiConnSocket;           //UI���������ͨѶsocket
	int			 m_nSyncHight;             //�����ͬ����UI�˵��������߶�  
	int          m_nBlockTipHight;         //Ǯ��UI�˱��������tip�߶�


	int blocktipheight;
	bool m_poptips;
	int m_reminder;
	bool m_bChinese;

	CSplashThread* pSplashThread;

	map<string,string>  m_mapAppId;        //Ӧ��ID��map

	void  SendUIMessage(int message,string jsonaddr);
	void  GetMainDlgStruct();
	int   SendPostThread(DWORD msgtype);    //������Ϣ������
public:
	CMyQueue m_msgQueue;                    //�����UI���洦�����Ϣ����
	CMyQueue m_msgLockStatusQueue;          //Ǯ����״̬��Ϣ����
	CMyQueue m_msgConnCountQueue;           //������������Ϣ����
	CMyQueue m_msgProcessBarQueue;          //���½�����Bar����
	CMyQueue m_msgUIRecDlgQueue;            //���ս�����Ϣ����
	CMyQueue m_msgUISendDlgQueue;           //���ͽ�����Ϣ����
	CMyQueue m_msgUITxDetailDlgQueue;       //��������Ի�����Ϣ����
	CMyQueue m_msgUIMainDlgQueue;           //��ҳ�Ի�����Ϣ���� 

	string   m_strCurAdminAddress;          //��ǰ����Ա�˺�
	string   m_strCurShortAddress;
	string   m_strCurEntAddress;            //��ǰ��ҵ�˺�

	int      m_bIsAdmin;                    //�Ƿ����Ա
	int      m_nShowType;


	CSqliteDeal   m_SqliteDeal;             //Sqlite3���ݿ����
public:
	BOOL m_passlock;    //�Ƿ�Ҫ������ʾ�򣬹رյ�ʱ�� false�����������ڽ����� true����,�����ڽ�����

};

extern CMutex mutexObj;
extern CKoalaApp theApp;