#pragma once


// CNewRecvAddr �Ի���

class CNewRecvAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CNewRecvAddr)

public:
	CNewRecvAddr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewRecvAddr();

// �Ի�������
	enum { IDD = IDD_NEWRECVADDR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1058();
};
