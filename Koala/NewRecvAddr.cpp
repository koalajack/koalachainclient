// NewRecvAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "NewRecvAddr.h"
#include "afxdialogex.h"


// CNewRecvAddr �Ի���

IMPLEMENT_DYNAMIC(CNewRecvAddr, CDialogEx)

CNewRecvAddr::CNewRecvAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewRecvAddr::IDD, pParent)
{

}

CNewRecvAddr::~CNewRecvAddr()
{
}

void CNewRecvAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewRecvAddr, CDialogEx)
	ON_BN_CLICKED(1058, &CNewRecvAddr::OnBnClicked1058)
END_MESSAGE_MAP()


// CNewRecvAddr ��Ϣ�������


void CNewRecvAddr::OnBnClicked1058()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
