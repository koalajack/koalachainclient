#pragma once


// CToolDlg �Ի���
#include "ToolCmdDlg.h"
#include "ToolInfoDlg.h"
#include "ToolPeerListDlg.h"
#include "CoolTabCtrl.h"
#include "afxcmn.h"

class CToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CToolDlg)

public:
	CToolDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CToolDlg();

// �Ի�������
	enum { IDD = IDD_TOOL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void SetActiveIndex(int nIndex);
	int m_nActiveIndex;

	afx_msg void OnClose();
	afx_msg void OnSelchangeTabTool(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowActiveWnd();
	void ShowToolPeerlistDlg();
	void ShowToolInfoDlg();
	void ShowToolCmd();

	

public:
	CTabCtrl m_ToolTab;

	CDialog* m_pActiveWnd;
	CToolPeerListDlg m_ToolPeerlist;
	CToolInfoDlg m_ToolInfo;
	CToolCmdDlg m_ToolCmd;

	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
