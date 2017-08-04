#pragma once


// CNewRecvAddr 对话框

class CNewRecvAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CNewRecvAddr)

public:
	CNewRecvAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewRecvAddr();

// 对话框数据
	enum { IDD = IDD_NEWRECVADDR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked1058();
};
