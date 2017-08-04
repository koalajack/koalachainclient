#pragma once


// CSelectSendAddr 对话框

class CSelectSendAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectSendAddr)

public:
	CSelectSendAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectSendAddr();

// 对话框数据
	enum { IDD = IDD_SELECTSENDADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSelectsendaddr();
	virtual BOOL OnInitDialog();
};
