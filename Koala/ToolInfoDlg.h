#pragma once

// CToolInfoDlg 对话框

struct INFO_DATA
{
	string strClientName;
	string strClientVer;

	string strInitTime;
	string StartTime;

	string strName;
	string strConCount;
	string strMNodes;

	string strCurblockNum;
	string strLastBlockTime;

	INFO_DATA() {strClientName = ""; strClientVer = ""; strInitTime = ""; StartTime = ""; strName = ""; strConCount = "";
	strCurblockNum = ""; strLastBlockTime = "";};
};

class CToolInfoDlg : public CDialogEx
{
	DECLARE_DYNCREATE(CToolInfoDlg)

public:
	CToolInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolInfoDlg();

// 对话框数据
	enum { IDD = IDD_TOOLINFO_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	void ShowInfo();
	void GetInfo();

	void GetMainNode();				//获取所有主节点的信息

	INFO_DATA m_info;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	void long2time(ULONGLONG nTime, CString& strTime);
	CString m_strDateTime;

	vector<string> m_vecMainNodes;

};
