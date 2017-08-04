#pragma once
#include "afxwin.h"
#include "AutoComplete.h"
#include "afxcmn.h"
#include "DialogBase.h"
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CRPCDlg �Ի���


// CToolCmdDlg �Ի���

class CToolCmdDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CToolCmdDlg)

public:
	CToolCmdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToolCmdDlg();

// �Ի�������
	enum { IDD = IDD_TOOLCMD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnShowSendData(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCmdclearBtn();

private:
	void UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CAutoComplete AutoCombo;

private:
	Json::Value mRoot; 

public:
	void ShowJson(string rpcname,string jsomstr);


	/*CRect m_rtBtnClear;
	CRect m_rtRPCInput;*/
	CTreeCtrl m_rpccommand;
	CMFCButton m_rpcBtnClear;
	CComboBox m_comboxinput;

//	CStaticTrans     m_headText;
	
};
