// OpenUriDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Koala.h"
#include "OpenUriDlg.h"
#include "afxdialogex.h"
#include <malloc.h>
#include <string.h>




// COpenUriDlg 对话框

IMPLEMENT_DYNAMIC(COpenUriDlg, CDialogEx)

COpenUriDlg::COpenUriDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COpenUriDlg::IDD, pParent)
	, m_strOpenURI(_T(""))
{
	m_strAddr = "";
	m_strLabel = "";
	m_strMsg = "";
	m_account = "";
}

COpenUriDlg::~COpenUriDlg()
{
}

void COpenUriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITURI, m_strOpenURI);
	DDX_Control(pDX, IDC_EDITURI, m_ctlOpenURI);
}


BEGIN_MESSAGE_MAP(COpenUriDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPENURI, &COpenUriDlg::OnBnClickedBtnOpenuri)
	ON_BN_CLICKED(IDC_BTN_OK, &COpenUriDlg::OnBnClickedBtnOk)
//	ON_WM_CTLCOLOR()
//	ON_WM_DESTROY()
ON_BN_CLICKED(IDC_BTN_CANCEL, &COpenUriDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// COpenUriDlg 消息处理程序


BOOL COpenUriDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_STAT77)->SetWindowText("Select a payment request from the file:");
		GetDlgItem(IDC_BTN_OK)->SetWindowText("OK");
		GetDlgItem(IDC_BTN_CANCEL)->SetWindowText("Cancel");
		SetWindowText("Open URI");
	}

	// TODO:  在此添加额外的初始化
	m_redcolor=RGB(255,0,0);                      // 红色  
	m_textcolor=RGB(255,255,255);                 // 文本颜色设置为白色  
	m_redbrush.CreateSolidBrush(m_redcolor);      // 红色背景色  


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COpenUriDlg::OnBnClickedBtnOpenuri()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_strOpenURI.IsEmpty())
	{
		m_strOpenURI.Empty();
	}
	CFileDialog dlg(TRUE,NULL,NULL,OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ,_T("*.txt||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".txt")))
		{
			return;
		}

		CStdioFile file;
		CString str;

		if(file.Open(strPath, CFile::modeRead))
		{
			while (file.ReadString(str))
			{
				//处理读到的每一行数据
				m_strOpenURI += str;
				
			}
			file.Close();
		}
		UpdateData(FALSE);
		

	}
	
}

BOOL COpenUriDlg::GetFileName(CString &fileName,CString strexe )
{
	int spcace = fileName.Find(" ");
	if (spcace >=0)
	{
		//UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_PATHNOTNULL" ,1) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) ,MFB_OK|MFB_TIP );
		return false;
	}
	int pos = fileName.Find(".",0);
	if (pos <0)
	{
		fileName.AppendFormat(strexe);
	}
	pos = fileName.Find(".",0);
	fileName = fileName.Left(pos);
	fileName.AppendFormat(strexe);
	if(PathFileExistsA(fileName)){
		CString strDisplay;
		strDisplay=UiFun::UI_LoadString("DACRSU" , "DACRSU_INSTEAD" ,1) ;
		/*if (IDOK == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,1) , MFB_OKCANCEL|MFB_TIP ) )
		{
		return TRUE;
		}*/
	}

	return TRUE;
}


void COpenUriDlg::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	string strOpenURI(m_strOpenURI.GetBuffer());
	ParseURI(strOpenURI, &m_strAddr, &m_strLabel, &m_strMsg, &m_account);

	CDialogEx::OnOK();
	
}

BOOL COpenUriDlg::ParseURI(string strURI, string *pStrAddr, string *pStrLabel, string *pStrMsg, string *pAmount)
{
	string::size_type iFind, iEndFind;

	iFind = strURI.find("koalacoin:");

	if (iFind == string::npos)
	{
		return FALSE;
	}

	strURI.erase(0, iFind + 10);

	iFind = strURI.find_first_of('?');
	if (iFind == string::npos)
	{
		if(iFind != 34)
		{
			
		}
		return FALSE;
	}
	*pStrAddr = strURI.substr(0, iFind);

	iEndFind = strURI.find_last_of("?");
	strURI.erase(0, iEndFind + 1);

	char *tokenPtr=strtok(const_cast<char *>(strURI.c_str()),"&"); 
	char *pTemp = NULL;
	while(NULL != tokenPtr)
	{
		if(0 == strncmp(tokenPtr, "message=", 8))
		{
			pTemp = tokenPtr + 8;
			if(NULL != pTemp || '\0' != *pTemp)
			{
				(*pStrMsg) = pTemp;
			}
			
		}
		if(0 == strncmp(tokenPtr, "label=", 6))
		{
			pTemp = tokenPtr + 6;
			if(NULL != pTemp && ('\0' != *pTemp))
			{
				(*pStrLabel) = pTemp;
			}

			
		}
		if(0 == strncmp(tokenPtr, "amount=", 7))
		{
			pTemp = tokenPtr + 7;
			if(NULL != pTemp || "" != pTemp)
			{
				(*pAmount) = pTemp;
			}
			
		}
		 tokenPtr = strtok(NULL, "&"); 
	}

	return TRUE;
	
}


//HBRUSH COpenUriDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  在此更改 DC 的任何特性
//
//	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
//	switch (nCtlColor)
//	{
//
//		case CTLCOLOR_EDIT:
//		case CTLCOLOR_MSGBOX:
//			{
//				switch (pWnd->GetDlgCtrlID())
//				{
//					case IDC_EDITURI:        
//					
//						pDC->SetBkColor(m_redcolor);    // change the background  
//				 
//						pDC->SetTextColor(m_textcolor); // change the text color  
//						hbr = (HBRUSH) m_redbrush;    // apply the blue brush  
//					
//						break;    
//
//				}
//			}
//
//		default:
//			hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);  
//
//	}
//
//	return hbr;
//}


//void COpenUriDlg::OnDestroy()
//{
//	CDialogEx::OnDestroy();
//
//	// TODO: 在此处添加消息处理程序代码
//
//}


void COpenUriDlg::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
