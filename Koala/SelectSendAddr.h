#pragma once


// CSelectSendAddr �Ի���

class CSelectSendAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectSendAddr)

public:
	CSelectSendAddr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelectSendAddr();

// �Ի�������
	enum { IDD = IDD_SELECTSENDADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSelectsendaddr();
	virtual BOOL OnInitDialog();
};
