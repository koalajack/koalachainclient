#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CEntInfoDlg �Ի���

class CEntInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEntInfoDlg)

public:
	CEntInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEntInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	string m_strEntInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
