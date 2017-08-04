// NewRecvAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "NewRecvAddr.h"
#include "afxdialogex.h"


// CNewRecvAddr 对话框

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


// CNewRecvAddr 消息处理程序


void CNewRecvAddr::OnBnClicked1058()
{
	// TODO: 在此添加控件通知处理程序代码
}
