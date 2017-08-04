#pragma once
#include "afxcmn.h"
#include "ListCtrlCl.h"
#include "afxwin.h"

// COverviewDlg �Ի���

class COverviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COverviewDlg)

public:
	COverviewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COverviewDlg();

// �Ի�������
	enum { IDD = IDD_OVERVIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	LRESULT OnShowListCtrl(WPARAM wParam, LPARAM lParam);  //���������ҳ�ռ�ؼ����ݵ���Ϣ
public:
	virtual BOOL OnInitDialog();
	void SetEntInfo(string strEntAddr);
	void SetEntInfoEx();
	CListCtrlCl m_lstReceTrade;
private:
	void SetCtrlText();                  //�������ø��ؼ�������
	void OnInitialCtrlText();            //��ʼ�����пؼ�����
public:
	CStaticTrans m_staUnconfirmMoney;
	CStaticTrans m_staUsageMoney;
	CStaticTrans m_staQuotaByTime;
	CStaticTrans m_staQuotaByDay;
	CStaticTrans m_staQuotaRemain;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
