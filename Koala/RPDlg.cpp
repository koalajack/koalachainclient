// RPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Koala.h"
#include "RPDlg.h"
#include "afxdialogex.h"


// CRPDlg �Ի���

IMPLEMENT_DYNAMIC(CRPDlg, CDialogEx)

CRPDlg::CRPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRPDlg::IDD, pParent)
{
	m_strRecvAddr = "";
	m_strRecvLabel = "";
	m_strMoney = "";
	m_strRecvMessage = "";
	m_strURI = "";

	m_iHeight = 0;
	m_iWidth = 0;
	m_lpByte = NULL;
}

CRPDlg::~CRPDlg()
{
}

void CRPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_RICHEDIT2_LIST, m_richeditInfo);
	DDX_Control(pDX, IDC_RICHEDIT2_LIST, m_richeditInfo);
}


BEGIN_MESSAGE_MAP(CRPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_COPYURI, &CRPDlg::OnBnClickedButtonCopyuri)
	ON_BN_CLICKED(IDC_BUTTON_COPYADDR, &CRPDlg::OnBnClickedButtonCopyaddr)
	ON_BN_CLICKED(IDC_BUTTON_SAVEBMP, &CRPDlg::OnBnClickedButtonSavebmp)

	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CRPDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CRPDlg ��Ϣ�������


BOOL CRPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(!theApp.m_bChinese) {
		GetDlgItem(IDC_BUTTON_COPYURI)->SetWindowText("Copy URI");
		GetDlgItem(IDC_BUTTON_COPYADDR)->SetWindowText("Copy Address");
		GetDlgItem(IDC_BUTTON_SAVEBMP)->SetWindowText("Save image");
		GetDlgItem(IDC_BUTTON_CLOSE)->SetWindowText("Close");
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strTitle = NULL;
	CString strInfo = NULL;
	string strURI;

	strTitle += theApp.m_bChinese ? _T("���󸶿") : _T("Request payment to");
	strTitle += m_strRecvAddr;
	SetWindowText(strTitle);

	strInfo += theApp.m_bChinese ? "������Ϣ\n" : "Payment info\n";

	strURI = CombineURI(m_strRecvAddr.GetBuffer(), m_strRecvLabel.GetBuffer(), m_strRecvMessage.GetBuffer(), m_strMoney.GetBuffer() );
	m_strURI += _T("URI:");
	m_strURI += strURI.c_str();

	strInfo += m_strURI;

	strInfo += "\n";

	//strInfo += strURI.c_str();

	if(m_strRecvAddr.Compare(""))
	{
		CString strAddr = NULL;
		strAddr += theApp.m_bChinese ? "��ַ:" : "Address";
		strAddr += m_strRecvAddr;
		strAddr += "\n";

		strInfo += strAddr;

	}

	if(m_strMoney.Compare(""))
	{
		string strAccount;
		strAccount += theApp.m_bChinese ? "���:" : "Amount";
		strAccount += m_strMoney;
		strAccount += "\n";

		strInfo += strAccount.c_str();
	}

	

	if(m_strRecvLabel.Compare(""))
	{
		CString strLabel = NULL;
		strLabel += theApp.m_bChinese ? "��ǩ:" : "Label";
		strLabel += m_strRecvLabel;

		strLabel += "\n";

		strInfo += strLabel;
	}

	if(m_strRecvMessage.Compare(""))
	{
		CString strMessage = NULL;
		strMessage += theApp.m_bChinese ? "��Ϣ:" : "Message";
		strMessage += m_strRecvMessage;

		strMessage += "\r\n";

		strInfo += strMessage;
	}


	m_richeditInfo.SetSel(-1, -1);
	m_richeditInfo.ReplaceSel( (LPCTSTR)strInfo );

	m_lpByte = GenerateBarCode(m_strURI, &m_iWidth, &m_iHeight);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


string CRPDlg::CombineURI( string strAddr, string strLabel, string strMsg, string strMoney)
{
	string strURI;
	BOOL bBeginFlag = FALSE;

	strURI = "koalacoin:";
	if(strAddr.compare(""))
	{
		strURI += strAddr;
		strURI += "?\n";
	}

	if(strMoney.compare(""))
	{
		if(bBeginFlag)
		{
			strURI += "&";
		}
		strURI += "amount=";
		strURI += strMoney;
		bBeginFlag = TRUE;
		
	}

	if(strLabel.compare(""))
	{
		if(bBeginFlag)
		{
			strURI += "&";
		}
		strURI += "label=";
		strURI += strLabel;
		
		bBeginFlag = TRUE;
		
		
	}

	if(strMsg.compare(""))
	{
		if(bBeginFlag)
		{
			strURI += "&";
		}
		strURI += "message=";
		strURI += strMsg;
	}

	return strURI;
}



void CRPDlg::OnBnClickedButtonCopyuri()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(OpenClipboard())   
	{   
		HGLOBAL   clipbuffer;   
		char   *   buffer;   
		EmptyClipboard();   
		clipbuffer   =   GlobalAlloc(GMEM_DDESHARE,   m_strURI.GetLength()+1);   
		buffer   =   (char*)GlobalLock(clipbuffer);   
		strcpy(buffer,  LPCSTR(m_strURI));   
		GlobalUnlock(clipbuffer);   
		SetClipboardData(CF_TEXT,clipbuffer);   
		CloseClipboard();   
	}   
	char   *pBuffer;   
	if(OpenClipboard())   
	{   
		pBuffer   =   (char*)GetClipboardData(CF_TEXT);   //do   something   with   buffer   here before   it   goes   out   of   scope
	}   

	CloseClipboard(); 
}


void CRPDlg::OnBnClickedButtonCopyaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(OpenClipboard())   
	{   
		HGLOBAL   clipbuffer;   
		char   *   buffer;   
		EmptyClipboard();   
		clipbuffer   =   GlobalAlloc(GMEM_DDESHARE,   m_strRecvAddr.GetLength()+1);   
		buffer   =   (char*)GlobalLock(clipbuffer);   
		strcpy(buffer,   LPCSTR(m_strRecvAddr));   
		GlobalUnlock(clipbuffer);   
		SetClipboardData(CF_TEXT,clipbuffer);   
		CloseClipboard();   
	}   
	char   *   buffer;   
	if(OpenClipboard())   
	{   
		buffer   =   (char*)GetClipboardData(CF_TEXT);  //do   something   with   buffer   here  before   it   goes   out   of   scope    
	}   

	CloseClipboard(); 
}


void CRPDlg::OnBnClickedButtonSavebmp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strAppName;
	::GetModuleFileName(NULL, strAppName.GetBuffer(_MAX_PATH),_MAX_PATH);
	strAppName.ReleaseBuffer();

	int nPos = strAppName.ReverseFind('\\');
	strAppName = strAppName.Left(nPos+1);

	LPCTSTR szFilter = "Chart Files(*.bmp)|*.bmp||";

	CFileDialog openFileDlg(FALSE, NULL, strAppName + "", OFN_CREATEPROMPT | OFN_PATHMUSTEXIST , szFilter, NULL);

	if(openFileDlg.DoModal() == IDOK)
	{
	this->SetDlgItemText(IDC_EDITFROM, openFileDlg.GetPathName());
	}

	CString str = openFileDlg.GetPathName();
	str += ".bmp";

	if(NULL == m_lpByte)
	{
		return;
	}

	SaveBmp(str.GetBuffer(), m_lpByte,m_iWidth, m_iHeight, 32, NULL);
}



void CRPDlg::setQrcodeErrorCorrect(char c)
{
	qrcodeErrorCorrect = c;
}

char CRPDlg::getQrcodeErrorCorrect()
{
	return qrcodeErrorCorrect;
}

int CRPDlg::getQrcodeVersion()
{
	return qrcodeVersion;
}

void CRPDlg::setQrcodeVersion(int i)
{
	if(i >= 0 && i <= 40)
		qrcodeVersion = i;
}

void CRPDlg::setQrcodeEncodeMode(char c)
{
	qrcodeEncodeMode = c;
}

char CRPDlg::getQrcodeEncodeMode()
{
	return qrcodeEncodeMode;
}

void CRPDlg::setStructureappend(int i, int j, int k)
{
	if(j > 1 && j <= 16 && i > 0 && i <= 16 && k >= 0 && k <= 255)
	{
		qrcodeStructureappendM = i;
		qrcodeStructureappendN = j;
		qrcodeStructureappendParity = k;
	}
}

//int Qrcode::calStructureappendParity(char abyte0[],int i)
int CRPDlg::calStructureappendParity(char* abyte0,int i)
{
	int j = 0;
	int k = 0;
	//int i = abyte0.length;
	if(i > 1)
	{
		k = 0;
		for(; j < i; j++)
			k ^= abyte0[j] & 255;
		
	} else
	{
		k = -1;
	}
	return k;
}

//bool** Qrcode::calQrcode(char abyte0[],int iLen0,int& iLenRet)
bool** CRPDlg::calQrcode(char* abyte0,int iLen0,int& iLenRet)
{
	CString strText;
	TCHAR exepath[MAX_PATH]={0};
	::GetModuleFileName(NULL,exepath,MAX_PATH);
	strText = exepath;
	int nPos = strText.ReverseFind( _T('\\') );   
	strText=strText.Left(nPos+1); 

		
	int j = 0;
	int n1;
	int i = iLen0;
	//int ai[] = new int[i + 32];
	int* ai = new int[i + 32];
	//char abyte1[] = new char[i + 32];
	char* abyte1 = new char[i + 32];
	memset(abyte1,0,i+32);
	if(i <= 0)
	{		
		bool** aflag=new bool*[1];
		aflag[0]=new bool[1];
		aflag[0][0]=false;
		iLenRet=1;
		return aflag;
	}
	if(qrcodeStructureappendN > 1)
	{
		ai[0] = 3;
		abyte1[0] = 4;
		ai[1] = qrcodeStructureappendM - 1;
		abyte1[1] = 4;
		ai[2] = qrcodeStructureappendN - 1;
		abyte1[2] = 4;
		ai[3] = qrcodeStructureappendParity;
		abyte1[3] = 8;
		j = 4;
	}
	abyte1[j] = 4;
	//int* ai1=new int[41];
	int ai1[41];
	int k;
	switch(qrcodeEncodeMode)
	{
	case 65: // 'A'
			 /*ai1 = (new int[] {
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
			 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 
			 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
			 4
	});*/
		{
			for(int ii=0;ii<41;ii++)
			{
				if(ii<=9)ai1[ii]=0;
				else if(ii<=26)ai1[ii]=2;
				else ai1[ii]=4;
			}
			
			ai[j] = 2;
			j++;
			ai[j] = i;
			abyte1[j] = 9;
			k = j;
			j++;
			for(int l = 0; l < i; l++)
			{
				char c = (char)abyte0[l];
				char byte0 = 0;
				if(c >= '0' && c < ':')
					byte0 = (char)(c - 48);
				else
					if(c >= 'A' && c < '[')
					{
						byte0 = (char)(c - 55);
					} else
					{
						if(c == ' ')
							byte0 = 36;
						if(c == '$')
							byte0 = 37;
						if(c == '%')
							byte0 = 38;
						if(c == '*')
							byte0 = 39;
						if(c == '+')
							byte0 = 40;
						if(c == '-')
							byte0 = 41;
						if(c == '.')
							byte0 = 42;
						if(c == '/')
							byte0 = 43;
						if(c == ':')
							byte0 = 44;
					}
					if(l % 2 == 0)
					{
						ai[j] = byte0;
						abyte1[j] = 6;
					} else
					{
						ai[j] = ai[j] * 45 + byte0;
						abyte1[j] = 11;
						if(l < i - 1)
							j++;
					}
			}			
			j++;			
		}		
		break;		
	case 78: // 'N'
			 /*ai1 = (new int[] {
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
			 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 
			 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
			 4
	});*/
		{
			for(int ii=0;ii<41;ii++)
			{
				if(ii<=9)ai1[ii]=0;
				else if(ii<=26)ai1[ii]=2;
				else ai1[ii]=4;
			}
			ai[j] = 1;
			j++;
			ai[j] = i;
			abyte1[j] = 10;
			k = j;
			j++;
			for(int i1 = 0; i1 < i; i1++)
				if(i1 % 3 == 0)
				{
					ai[j] = abyte0[i1] - 48;
					abyte1[j] = 4;
				} else
				{
					ai[j] = ai[j] * 10 + (abyte0[i1] - 48);
					if(i1 % 3 == 1)
					{
						abyte1[j] = 7;
					} else
					{
						abyte1[j] = 10;
						if(i1 < i - 1)
							j++;
					}
				}
				
				j++;
				
		}		
		break;			
	default:
	/*ai1 = (new int[] {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
                8
		});*/		
		for(int ii=0;ii<41;ii++)
		{
			if(ii<=9)ai1[ii]=0;			
			else ai1[ii]=8;
		}
		ai[j] = 4;
		j++;
		ai[j] = i;
		abyte1[j] = 8;
		k = j;
		j++;
		for(int j1 = 0; j1 < i; j1++)
		{
			ai[j1 + j] = abyte0[j1] & 255;
			abyte1[j1 + j] = 8;
		}
		
		j += i;
		break;
        }
        int k1 = 0;
        for(int l1 = 0; l1 < j; l1++)
            k1 += abyte1[l1];
		
        char byte1;
        switch(qrcodeErrorCorrect)
        {
        case 76: // 'L'
            byte1 = 1;
            break;
			
        case 81: // 'Q'
            byte1 = 3;
            break;
			
        case 72: // 'H'
            byte1 = 2;
            break;
			
        default:
            byte1 = 0;
            break;
        }

        //int ai2[4][41] = {
		int ai2[4][41] = {
            {
					0, 128, 224, 352, 512, 688, 864, 992, 1232, 1456, 
					1728, 2032, 2320, 2672, 2920, 3320, 3624, 4056, 4504, 5016, 
					5352, 5712, 6256, 6880, 7312, 8000, 8496, 9024, 9544, 10136, 
					10984, 11640, 12328, 13048, 13800, 14496, 15312, 15936, 16816, 17728, 
					18672
            }, 
			{
					0, 152, 272, 440, 640, 864, 1088, 1248, 1552, 1856, 
					2192, 2592, 2960, 3424, 3688, 4184, 4712, 5176, 5768, 6360, 
					6888, 7456, 8048, 8752, 9392, 10208, 10960, 11744, 12248, 13048, 
					13880, 14744, 15640, 16568, 17528, 18448, 19472, 20528, 21616, 22496, 
					23648
				}, 
				{
						0, 72, 128, 208, 288, 368, 480, 528, 688, 800, 
							976, 1120, 1264, 1440, 1576, 1784, 2024, 2264, 2504, 2728, 
							3080, 3248, 3536, 3712, 4112, 4304, 4768, 5024, 5288, 5608, 
							5960, 6344, 6760, 7208, 7688, 7888, 8432, 8768, 9136, 9776, 
							10208
					},
					{
							0, 104, 176, 272, 384, 496, 608, 704, 880, 1056, 
								1232, 1440, 1648, 1952, 2088, 2360, 2600, 2936, 3176, 3560, 
								3880, 4096, 4544, 4912, 5312, 5744, 6032, 6464, 6968, 7288, 
								7880, 8264, 8920, 9368, 9848, 10288, 10832, 11408, 12016, 12656, 
								13328
						}
        };
        int i2 = 0;
        if(qrcodeVersion == 0)
        {
            qrcodeVersion = 1;
            for(int j2 = 1; j2 <= 40; j2++)
            {
                if(ai2[byte1][j2] >= k1 + ai1[qrcodeVersion])
                {
                    i2 = ai2[byte1][j2];
                    break;
                }
                qrcodeVersion++;
            }
			
        } else
        {
            i2 = ai2[byte1][qrcodeVersion];
        }
        k1 += ai1[qrcodeVersion];
        abyte1[k] += ai1[qrcodeVersion];
        int ai3[] = {
            0, 26, 44, 70, 100, 134, 172, 196, 242, 292, 
				346, 404, 466, 532, 581, 655, 733, 815, 901, 991, 
				1085, 1156, 1258, 1364, 1474, 1588, 1706, 1828, 1921, 2051, 
				2185, 2323, 2465, 2611, 2761, 2876, 3034, 3196, 3362, 3532, 
				3706
        };
        int k2 = ai3[qrcodeVersion];
        int l2 = 17 + (qrcodeVersion << 2);
        int ai4[] = {
            0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 
				0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 
				3, 4, 4, 4, 4, 4, 4, 4, 3, 3, 
				3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 
				0
        };
        int i3 = ai4[qrcodeVersion] + (k2 << 3);
        char* abyte2 = new char[i3];
        char* abyte3 = new char[i3];
        char* abyte4 = new char[i3];		
		char abyte5[15];
		char abyte6[15];
		char abyte7[1];
		char abyte8[128];
		
		memset(abyte2,0,i3);
		memset(abyte3,0,i3);
		memset(abyte4,0,i3);
		memset(abyte5,0,15);
		memset(abyte6,0,15);
		memset(abyte7,0,1);
		memset(abyte8,0,128);
		
        try
        {
            //CString s = "qrcode_data/qrv" + Integer.toString(qrcodeVersion) + "_" + Integer.toString(byte1) + ".dat";
			/*CString strText;
			TCHAR exepath[MAX_PATH]={0};
			::GetModuleFileName(NULL,exepath,MAX_PATH);
			strText = exepath;
			int nPos = strText.ReverseFind( _T('\\') );   
			strText=strText.Left(nPos+1); */

			CString s;
			s.Format(strText + "qrcode_data/qrv%d_%d.dat",qrcodeVersion,byte1);			
			
			//CFile f(s, CFile::modeCreate | CFile::modeWrite );	
			CFile f(s, CFile::modeRead);
			try
			{				
				f.Read(abyte2, i3);
				f.Read(abyte3, i3);
				f.Read(abyte4, i3);
				f.Read(abyte5, 15);
				f.Read(abyte6, 15);
				f.Read(abyte7, 1);
				f.Read(abyte8, 128);
				f.Close();
			
			}
			catch(...)
			{			
				f.Close();
			}
			
			//д��ʱ�ļ�
            /*InputStream inputstream = (com.swetake.util.Qrcode.class).getResourceAsStream(s);
            BufferedInputStream bufferedinputstream = new BufferedInputStream(inputstream);
            bufferedinputstream.read(abyte2);
            bufferedinputstream.read(abyte3);
            bufferedinputstream.read(abyte4);
            bufferedinputstream.read(abyte5);
            bufferedinputstream.read(abyte6);
            bufferedinputstream.read(abyte7);
            bufferedinputstream.read(abyte8);
            bufferedinputstream.close();
            inputstream.close();*/
        }
        catch(...)
        {
			//AfxMessageBox(exception.)
            //exception.printStackTrace();
        }
        int j3 = 1;
        for(char byte2 = 1; byte2 < 128; byte2++)
        {
            if(abyte8[byte2] != 0)
                continue;
            j3 = byte2;
            break;
        }
		
        char* abyte9 = new char[j3+1];
		memset(abyte9,0,j3);
		memcpy(abyte9,abyte8,j3);
		abyte9[j3]=0;
        //System.arraycopy(abyte8, 0, abyte9, 0, j3);
        char abyte10[15] = {
			0, 1, 2, 3, 4, 5, 7, 8, 8, 8, 
				8, 8, 8, 8, 8
        };
        char abyte11[15] = {
			8, 8, 8, 8, 8, 8, 8, 8, 7, 5, 
				4, 3, 2, 1, 0
        };
        int k3 = i2 >> 3;
        int l3 = 4 * qrcodeVersion + 17;
        int i4 = l3 * l3;
        char* abyte12 = new char[i4 + l3];
		memset(abyte12,0,i4 + l3);
        try
        {
            //CString s1 = "qrcode_data/qrvfr" + Integer.toString(qrcodeVersion) + ".dat";

			CString s1;
			s1.Format(strText + "qrcode_data/qrvfr%d.dat",qrcodeVersion);
			//CFile f1(s1, CFile::modeCreate | CFile::modeWrite );	
			CFile f1(s1, CFile::modeRead);
			try
			{				
				f1.Read(abyte12, i4 + l3);
				f1.Close();
			}
			catch(...)
			{			
				f1.Close();
			}          
        }
        catch(...)
        {
			// exception1.printStackTrace();
        }
        if(k1 <= i2 - 4)
        {
            ai[j] = 0;
            abyte1[j] = 4;
        } else
			if(k1 < i2)
			{
				ai[j] = 0;
				abyte1[j] = (char)(i2 - k1);
			} else
				if(k1 > i2)
					TRACE("overflow");
				//AfxMessageBox()
				//	System.out.println("overflow");
		int iLret1=0;
	  char* abyte13 = divideDataBy8Bits(ai, abyte1, k3,i + 32,iLret1);
		

		char* abyte14 = calculateRSECC(abyte13, abyte7[0], abyte9, k3, k2,iLret1,j3);		
	
		char** abyte15=new char*[l3];
		for( n1 = 0; n1 <l3; n1++)
		{
			//abyte15[n1] = new char[l3];
			abyte15[n1] = new char[l3];		
		}
		
		for(int j4 = 0; j4 < l3; j4++)
		{
			for(int k4 = 0; k4 < l3; k4++)
				abyte15[k4][j4] = 0;			
		}		

		for(int l4 = 0; l4 < k2; l4++)
		{
			char byte3 = abyte14[l4];
			for(int j5 = 7; j5 >= 0; j5--)
			{
				int l5 = l4 * 8 + j5;
				//if((abyte2[l5]==0)&&(abyte3[l5]==34)) AfxMessageBox("1");
				abyte15[abyte2[l5] & 255][abyte3[l5] & 255] = (char)(255 * (byte3 & 1) ^ abyte4[l5]);
				//byte3 = (char)((byte3 & 255) >>> 1);
				byte3 = (char)((byte3 & 255) >> 1);
				//byte3 = (char)((unsigned char)((byte3 & 255) >> 1));
			}					
		}

		
	
		/*for( j4 = 0; j4 < l3; j4++)
		{
			TestMsg(abyte15[j4],l3);			
		}*/
		
		for(int i5 = ai4[qrcodeVersion]; i5 > 0; i5--)
		{
			int k5 = (i5 + k2 * 8) - 1;
			abyte15[abyte2[k5] & 255][abyte3[k5] & 255] = (char)(255 ^ abyte4[k5]);
		}
		
	
		char byte4 = selectMask((char**)abyte15, ai4[qrcodeVersion] + k2 * 8,l3);
		char byte5 = (char)(1 << byte4);
		char byte6 = (char)(byte1 << 3 | byte4);
		CString as[32] = {
			"101010000010010", "101000100100101", "101111001111100", "101101101001011", "100010111111001", "100000011001110", "100111110010111", "100101010100000", "111011111000100", "111001011110011", 
				"111110110101010", "111100010011101", "110011000101111", "110001100011000", "110110001000001", "110100101110110", "001011010001001", "001001110111110", "001110011100111", "001100111010000", 
				"000011101100010", "000001001010101", "000110100001100", "000100000111011", "011010101011111", "011000001101000", "011111100110001", "011101000000110", "010010010110100", "010000110000011", 
				"010111011011010", "010101111101101"
		};
		for(int i6 = 0; i6 < 15; i6++)
		{			
			char byte7=as[byte6][i6]-48;			
			abyte15[abyte10[i6] & 255][abyte11[i6] & 255] = (char)(byte7 * 255);
			abyte15[abyte5[i6] & 255][abyte6[i6] & 255] = (char)(byte7 * 255);			
		}

		//boolean [][]aflag1 = new boolean[l3][l3];
		bool** aflag1 = new bool*[l3];	//\\\\\\\\\\\			
		for( n1 = 0; n1 <l3; n1++)
		{
			aflag1[n1] = new bool[l3];
		}
		int j6 = 0;
		for(int k6 = 0; k6 < l3; k6++)
		{
			for(int l6 = 0; l6 < l3; l6++)
			{
				if((abyte15[l6][k6] & byte5) != 0 || abyte12[j6] == 49)
					aflag1[l6][k6] = true;
				else
					aflag1[l6][k6] = false;
				j6++;
			}
			
			j6++;
		}
		//delete[] aflag1;
		iLenRet=l3;
		/************************************************************************/
		/*				ɾ��new ����											 */
		/************************************************************************/
		delete []ai;
		delete []abyte1;
		delete []abyte2;
		delete []abyte3;
		delete []abyte4;
		delete []abyte9;
		delete []abyte12;
		delete []abyte13;
		delete []abyte14;				
		
		for( n1 = 0; n1 <l3; n1++)
		{
			delete []abyte15[n1];
		}
		delete[] abyte15;
		return aflag1;
 }
	
	//char* Qrcode::divideDataBy8Bits(int ai[], char abyte0[], 
char* CRPDlg::divideDataBy8Bits(int* ai, char* abyte0, 
		int i,int iLen,int& iLRet)//int i,int j)		
    {
        //int iLen = abyte0.length; iLenAi=ai..length
		int j=iLen;
        int l = 0;
        int i1 = 8;
        int j1 = 0;
        //if(j == ai.length);
        for(int i2 = 0; i2 < j; i2++)
            j1 += abyte0[i2];
		
        int k = (j1 - 1) / 8 + 1;
        char* abyte1 = new char[i];
		memset(abyte1,0,i);
        for(int j2 = 0; j2 < k; j2++)
            abyte1[j2] = 0;
		
        for(int k2 = 0; k2 < j; k2++)
        {
            int k1 = ai[k2];
            int l1 = abyte0[k2];
            bool flag = true;
            if(l1 == 0)
                break;
            while(flag) 
                if(i1 > l1)
                {
                    abyte1[l] = (char)(abyte1[l] << l1 | k1);
                    i1 -= l1;
                    flag = false;
                } else
                {
                    l1 -= i1;
                    abyte1[l] = (char)(abyte1[l] << i1 | k1 >> l1);
                    if(l1 == 0)
                    {
                        flag = false;
                    } else
                    {
                        k1 &= (1 << l1) - 1;
                        flag = true;
                    }
                    l++;
                    i1 = 8;
                }
        }
		
        if(i1 != 8)
            abyte1[l] = (char)(abyte1[l] << i1);
        else
            l--;
        if(l < i - 1)
        {
            for(bool flag1 = true; l < i - 1; flag1 = !flag1)
            {
                l++;
                if(flag1)
                    abyte1[l] = -20;
                else
                    abyte1[l] = 17;
            }
			
        }
		iLRet=i;
        return abyte1;
    }
	
    //private 
	//char* Qrcode::calculateRSECC(char abyte0[], char byte0, char abyte1[], int i, int j,
char* CRPDlg::calculateRSECC(char* abyte0, char byte0, char* abyte1, int i, int j,
		int iLen0,int iLen1)
    {
		CString strText;
		TCHAR exepath[MAX_PATH]={0};
		::GetModuleFileName(NULL,exepath,MAX_PATH);
		strText = exepath;
		int nPos = strText.ReverseFind( _T('\\') );   
		strText=strText.Left(nPos+1); 

	//iLen1 abyte1.length  ; iLen0 abyte0.length
        char** abyte2 = new char*[256];
		//char abyte2[][] = new char[256][byte0];
		int n1;
		for(n1=0;n1<256;n1++){
			abyte2[n1]=new char[byte0];
			memset(abyte2[n1],0,byte0);
		}
	
        try
        { 
			CString s; 
			s.Format(strText + "qrcode_data/rsc%d.dat" ,byte0);						
			CFile f1(s, CFile::modeRead);
			try
			{			
				//for(int i2 = 0; i2 < 256; i2++)   bufferedinputstream.read(abyte2[i2]);
				for(int i2 = 0; i2 < 256; i2++)                
				f1.Read(abyte2[i2], byte0);
				f1.Close();
			}
			catch(...)
			{			
				f1.Close();
			}
        }
        catch(...)
        {
            //exception.printStackTrace();
        }
        bool flag = false;
        int i1 = 0;
        int k1 = 0;       
		
		char** abyte3 = new char*[iLen1];
        char* abyte4 = new char[j];
		memset(abyte4,0,j);		
		memcpy(abyte4, abyte0, iLen0);
       
		int ab3Length=0;
		for(int k = 0; k < iLen1; k++){
			ab3Length= (abyte1[k] & 255) - byte0;
            abyte3[k] = new char[(abyte1[k] & 255) - byte0];
			memset(abyte3[k],0,(abyte1[k] & 255) - byte0);
		}

       for(int l = 0; l < i; l++)
        {
         abyte3[k1][i1] = abyte0[l];				
		 if(++i1 >= (abyte1[k1] & 255) - byte0)
            {
                i1 = 0;
                k1++;
            }
        }
	 /* for(int kkk=0;kkk<ab3Length;kkk++)
	  {
        FILE *fp=NULL; 
	fp=fopen("C:\\test.txt","at+");	
	fprintf(fp,"%d:  %d\r\n",kkk,abyte3[3][kkk]);
	fclose(fp);
	  }*/

		
        //for(int l1 = 0; l1 < abyte1.length; l1++)
		for(int l1 = 0; l1 < iLen1; l1++)
        {
            //char abyte5[] = (char[])abyte3[l1].clone();			
			int iLen5=(abyte1[l1] & 255) - byte0;
			char* abyte5=new char[iLen5];
			memset(abyte5,0,iLen5);
			for (n1=0;n1<iLen5;n1++)
			{
				abyte5[n1]=abyte3[l1][n1];
			}
			//char* abyte5 = abyte3[l1];
			
            int j2 = abyte1[l1] & 255;
            int k2 = j2 - byte0;
            for(int j1 = k2; j1 > 0; j1--)
            {
                char byte1 = abyte5[0];
                if(byte1 != 0)
                {
                    //char abyte6[] = new char[abyte5.length - 1];
					char* abyte6 = new char[iLen5];
					memcpy(abyte6,abyte5+1,iLen5-1);
					abyte6[iLen5-1]=0;
                    //System.arraycopy(abyte5, 1, abyte6, 0, iLen5 - 1);
                    //char abyte9[] = abyte2[byte1 & 255];
					char* abyte9 = abyte2[byte1 & 255];
					int iLret1;
                    abyte5 = calculateByteArrayBits(abyte6, abyte9, "xor",iLen5-1,byte0,iLret1);
					//abyte5[iLen5-1]=0;					
					delete [] abyte6;
                } else				
					if(byte0 < iLen5)
					{
						//char abyte7[] = new char[abyte5.length - 1];
						char* abyte7 = new char[iLen5 - 1];
						memcpy(abyte7,abyte5+1,iLen5-1);
						for(n1=0;n1<iLen5-1;n1++)
							abyte5[n1]=abyte7[n1];
						//abyte5 = abyte7;
						delete []abyte7;
					} else
					{						
						//char abyte8[] = new char[byte0];
						char* abyte8 = new char[byte0];
						memcpy(abyte8,abyte5+1,iLen5-1);						
					//	abyte8[byte0 - 1] = 0;	
						for(n1=0;n1<byte0;n1++)
							abyte5[n1]=abyte8[n1];
						//abyte5 = abyte8;
						delete []abyte8;
					}

            }
			memcpy(abyte4+iLen0+l1*byte0,abyte5,byte0);	
			delete []abyte5;
        }
	
		for(n1=0;n1<256;n1++){			
			delete []abyte2[n1];
		}
		delete []abyte2;
		

		for(n1=0;n1<iLen1;n1++){			
			delete []abyte3[n1];
		}
		delete []abyte3;

        return abyte4;
    }
	
    //private 
char* CRPDlg::calculateByteArrayBits(char* abyte0, char* abyte1, CString s,
		int iLen0,int iLen1,int& iLRet)
    {
		char* abyte3;//
		char* abyte4;	
		int i,j,n1;
		if(iLen0 > iLen1)
        {            
			i=iLen0;			
			j=iLen1;			
			abyte3=new char[i+1];
			abyte4=new char[j+1];			
			memset(abyte3,0,i+1);
			for (n1=0;n1<i;n1++){
				abyte3[n1]=abyte0[n1];			
			}
			memset(abyte4,0,j+1);
			for (n1=0;n1<j;n1++)
			{
				abyte4[n1]=abyte1[n1];			
			}
        } else
        {            
			i=iLen1;		
			j=iLen0;	
			abyte3=new char[i+1];
			abyte4=new char[j+1];
			memset(abyte3,0,i+1);
			for (n1=0;n1<i;n1++)
			{				
				abyte3[n1]=abyte1[n1];			
			}
			memset(abyte4,0,j+1);
			for (n1=0;n1<j;n1++)
			{
				abyte4[n1]=abyte0[n1];			
			}
        }

		char* abyte2 = new char[i+1];
		memset(abyte2,0,i+1);
        for(int k = 0; k < i; k++){
            if(k < j)
            {
                if(s == "xor")
                    abyte2[k] = (char)(abyte3[k] ^ abyte4[k]);
                else
                    abyte2[k] = (char)(abyte3[k] | abyte4[k]);
            } else
            {
                abyte2[k] = abyte3[k];
            }			
			
		}
		delete []abyte3;
		delete []abyte4;		
		return abyte2;
	}
	

void CRPDlg::TestMsg(char* myStr,int length) 
{ 
//int kkk = strlen(myStr);
for(int iii=0;iii<length;iii++)
{
	FILE *fp=NULL; 
	fp=fopen("C:\\test.txt","at+");	
	fprintf(fp,"%d:  %d\r\n",iii,myStr[iii]);
	fclose(fp);
}
}

char CRPDlg::selectMask(char** abyte0, int i,int iLen0)
{
	//iLen0 abyte0.GetLength();
	//int j = abyte0.GetLength();		
	int j = iLen0;
	int ai[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};


	int ai1[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int ai2[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int ai3[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	int k = 0;
	int l = 0;
	int ai4[] = {
		0, 0, 0, 0, 0, 0, 0, 0
	};
	
	for(int i1 = 0; i1 < j; i1++)
	{
		int ai5[] = {
			0, 0, 0, 0, 0, 0, 0, 0
		};
		int ai6[] = {
			0, 0, 0, 0, 0, 0, 0, 0
		};
		bool aflag[] = {
			false, false, false, false, false, false, false, false
		};
		bool aflag1[] = {
			false, false, false, false, false, false, false, false
		};
	
for(int l1 = 0; l1 < j; l1++)
{
if(l1 > 0 && i1 > 0)
{
	k = abyte0[l1][i1] & abyte0[l1 - 1][i1] & abyte0[l1][i1 - 1] & abyte0[l1 - 1][i1 - 1] & 255;
	l = abyte0[l1][i1] & 255 | abyte0[l1 - 1][i1] & 255 | abyte0[l1][i1 - 1] & 255 | abyte0[l1 - 1][i1 - 1] & 255;
}
		
      for(int j2 = 0; j2 < 8; j2++)
			{
				// ai5[j2] = (ai5[j2] & 63) << 1 | (abyte0[l1][i1] & 255) >>> j2 & 1;
				// ai6[j2] = (ai6[j2] & 63) << 1 | (abyte0[i1][l1] & 255) >>> j2 & 1;//��λ�Զ�����
				
				int n=abyte0[l1][i1] & 255;
				int x=j2;				

				ai5[j2] = (ai5[j2] & 63) << 1 |(n>>x)&((int)pow((double)2,(double)(32-x))-1) & 1;
				ai6[j2] = (ai6[j2] & 63) << 1 |(n>>x)&((int)pow((double)2,(double)(32-x))-1) & 1;
				//ai5[j2] = (ai5[j2] & 63) << 1 |((abyte0[l1][i1] & 255))>> j2 & 1;
				//ai6[j2] = (ai6[j2] & 63) << 1 |((abyte0[i1][l1] & 255))>> j2 & 1;
				// ai5[j2] = (ai5[j2] & 63) << 1 | (abyte0[l1][i1] & 255) >> j2 & 1;
				// ai6[j2] = (ai6[j2] & 63) << 1 | (abyte0[i1][l1] & 255) >> j2 & 1;//��λ�Զ�����
			   
				
				if((abyte0[l1][i1] & 1 << j2) != 0)
					ai4[j2]++;
				if(ai5[j2] == 93)
					ai2[j2] += 40;
				if(ai6[j2] == 93)
					ai2[j2] += 40;
				if(l1 > 0 && i1 > 0)
				{
					if((k & 1) != 0 || (l & 1) == 0)
						ai1[j2] += 3;
					k >>= 1;
					l >>= 1;
				}
				if((ai5[j2] & 31) == 0 || (ai5[j2] & 31) == 31)
				{
					if(l1 > 3)
						if(aflag[j2])
						{
							ai[j2]++;
						} else
						{
							ai[j2] += 3;
							aflag[j2] = true;
						}
				} else
				{
					aflag[j2] = false;
				}
				if((ai6[j2] & 31) == 0 || (ai6[j2] & 31) == 31)
				{
					if(l1 > 3)
						if(aflag1[j2])
						{
							ai[j2]++;
						} else
						{
							ai[j2] += 3;
							aflag1[j2] = true;
						}
				} else
				{
					aflag1[j2] = false;
				}
			}
			
		}
			
	}
	

	int j1 = 0;
	char byte0 = 0;
	int ai7[] = {
		90, 80, 70, 60, 50, 40, 30, 20, 10, 0, 
			0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 
			90
	};
	for(int k1 = 0; k1 < 8; k1++)
	{
		ai3[k1] = ai7[(20 * ai4[k1]) / i];
		int i2 = ai[k1] + ai1[k1] + ai2[k1] + ai3[k1];
		if(i2 < j1 || k1 == 0)
		{
			byte0 = (char)k1;
			j1 = i2;
		}
	}
	
	return byte0;
}

BOOL CRPDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	return CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CRPDlg::WriteDIB( LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER hdr;
	LPBITMAPINFOHEADER lpbi;
	if (!hDIB)
		return FALSE;
	CFile file;
	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	int nColors = 1 << lpbi->biBitCount;
	// ���Ϊͼ�ļ�ͷ�ṹ
	hdr.bfType = ((WORD) ('M' << 8) | 'B'); // "BM"��־
	hdr.bfSize=::GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1=0;
	hdr.bfReserved2=0; 
	hdr.bfOffBits=(DWORD) (sizeof( hdr ) + lpbi->biSize +nColors * sizeof(RGBQUAD));
	file.Write( &hdr, sizeof(hdr) );
	// DIBͷ�ṹ��λͼ�������ݴ���
	file.Write( lpbi, GlobalSize(hDIB) );
	return TRUE;
}

HANDLE CRPDlg::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal )
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;

	ASSERT( bitmap.GetSafeHandle() );
	if( dwCompression == BI_BITFIELDS )//λͼ�Ƿ�ѹ��
		return NULL;
	hPal = (HPALETTE) pPal->GetSafeHandle();//ѡ���ɫ��
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);//��ȡλͼ��������Ϣ
	// ��ʼ��λͼ�ļ�ͷ�ṹ
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;
	// ����λͼ��Ϣ�ṹ��λͼ��ɫ��Ĵ�С
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 )
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);
	// ΪDIB����һ���豸����
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);
	// Ϊλͼ��Ϣͷ�ṹ����ɫ������ڴ�
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);
	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	*lpbi = bi;
	// ����λͼ�ֽ���
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);
	bi = *lpbi;
	// ���biSizeImageΪ0�����¼���
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
			* bi.biHeight;
		if (dwCompression != BI_RGB)//���Ϊͼѹ��
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}
	// �����յ�λͼ�ߴ�����ڴ�飬����λͼ��Ϣͷ�ṹ����ɫ����λͼ�ֽ���
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// ����ѡ��ԭʼ��ɫ��
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}
	lpbi = (LPBITMAPINFOHEADER)hDIB;
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
		0L,				// Start scan line
		(DWORD)bi.biHeight,		// # of scan lines
		(LPBYTE)lpbi 			// address for bitmap bits
		+ (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi,		// address of bitmapinfo
		(DWORD)DIB_RGB_COLORS);		// Use RGB for color table
	if( !bGotBits )
	{
		GlobalFree(hDIB);

		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}
	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;
}

void TestMsg(int i ,int myVal) 
{ 
	FILE *fp=NULL; 
	fp=fopen("C:\\mwtest.txt","at+"); 	
	fprintf(fp,"%d %d\r\n",i,myVal);
	fclose(fp); 	
}

BOOL CRPDlg::DrawMemBmp( LPTSTR szFile, CWnd *pWnd)
{	
	CWindowDC	dc(pWnd);
	CDC 	memDC;	
	CRect	rect(0,0,140,140);
	CBitmap bitmap;
	CRPDlg testQrcode;
	testQrcode.setQrcodeErrorCorrect('M');
	testQrcode.setQrcodeEncodeMode('B');
	testQrcode.setQrcodeVersion(7);

	memDC.CreateCompatibleDC(&dc);//�����ڴ��豸��������
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height() );
	memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(&rect,RGB(255,255,255));//��Щʱ��ͼ�����������^_^


	CString testString =  m_strURI;

	if (testString.IsEmpty())
	{
		MessageBox("����������!");
		return FALSE;
	}

	int iLen=testString.GetLength();
	char* d=new char[iLen+1];
	sprintf(d,"%s",testString);	

	if (iLen>0 && iLen <120){
		int iLRet;//���س���

		bool** s = testQrcode.calQrcode(d,iLen,iLRet);

		for (int i=0;i<iLRet;i++)
		{
			for (int j=0;j<iLRet;j++)
			{			
				if (s[j][i]){					
					CBrush br(RGB(0,0,0));					
					CRect tmpRec(j*3+2,i*3+2,j*3+5,i*3+5);
					memDC.FillRect(&tmpRec,&br);					
				}				
			}
		}		
	}
	delete []d;

	CPalette pal;
	if( dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries =
			GetSystemPaletteEntries( dc, 0, 255, pLP->palPalEntry );
		// ������ɫ��
		pal.CreatePalette( pLP );
		delete[] pLP;
	}

	HANDLE hDIB = DDBToDIB( bitmap, BI_RGB, &pal );// ת��Bitmap ��DIB
	if( hDIB == NULL )
		return FALSE;
	WriteDIB( szFile, hDIB );//��DIB���浽�ļ�
	GlobalFree( hDIB );//�ͷ�DIB������ռ�ռ�
	return TRUE;

}

BYTE* CRPDlg::GenerateBarCode(const char *_pStrInfo, int *_pWidth, int *_pHeight)
{
	if(NULL == _pStrInfo)
	{
		return NULL;
	}

	//Copy String
	int		iLen		= 0;
	char	*_pStrCopy	= NULL;

	iLen		= strlen(_pStrInfo)+1;
	if(iLen > 120)
	{
		return NULL;
	}

	if(NULL == (_pStrCopy = new char[iLen]))
	{
		return NULL;
	}	
	strcpy(_pStrCopy, _pStrInfo);


	//Qrcode
	CRPDlg _GenQrcode;
	_GenQrcode.setQrcodeErrorCorrect('M');
	_GenQrcode.setQrcodeEncodeMode('B');
	_GenQrcode.setQrcodeVersion(7);

	int iLRet = 0;//���س���

	bool** s = _GenQrcode.calQrcode(_pStrCopy, iLen, iLRet);


	int nStartX		= 4;
	int nStartY		= 4;
	int nStopX		= 4;
	int nStopY		= 4;
	int nCodeSize	= 4;

	BYTE* pData		= NULL;

	int SizeX				= ((nStartX + iLRet * nCodeSize + nStopX) * 32 + 31)/32;
	int SizeY				= nStartY + iLRet * nCodeSize + nStopY;

	int iLRetX				= (SizeX - nStartX - nStopX)/nCodeSize;
	int iLRetY				= iLRet;

	unsigned int nLineByte	= SizeX * 4;
	unsigned int nSize		= nLineByte * SizeY;

	if(NULL == (pData = new BYTE[nSize]))
	{
		if(NULL != _pStrCopy)
		{
			delete[] _pStrCopy;
			_pStrCopy = NULL;
		}

		return NULL;
	}

	BYTE ColorWhite = 255;
	BYTE ColorBlack = 0;
	BYTE ColorAlpha = 255;
	BYTE ColorValue	= ColorBlack;

	BYTE _R = 0;
	BYTE _G = 1;
	BYTE _B = 2;
	BYTE _A = 3;

	memset(pData, ColorWhite, nSize);

	int i						= 0;
	int j						= 0;
	unsigned int Cur			= 0;
	int _nCodeSize				= nCodeSize;
	bool bBlack					= false;

	for(j=0; j<iLRetY*nCodeSize; ++j)
	{
		for(i=0; i<iLRetX; ++i)
		{
			bBlack		= s[j/nCodeSize][i];			
			ColorValue	= bBlack ? ColorBlack : ColorWhite;

			Cur = (nStartY+j)*nLineByte + (nStartX + i*nCodeSize*4);

			_nCodeSize = 0;			
			while(_nCodeSize  < nCodeSize)
			{
				pData[Cur + _nCodeSize * nCodeSize + _R] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _G] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _B] = ColorValue;
				pData[Cur + _nCodeSize * nCodeSize + _A] = ColorAlpha;
				_nCodeSize++;
			}
		}
	}

	if(NULL != _pStrCopy)
	{
		delete[] _pStrCopy;
		_pStrCopy = NULL;
	}

	*_pWidth	= SizeX;
	*_pHeight	= SizeY;

	return pData;
}

//����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���
bool CRPDlg::SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������

	if(!imgBuf)
		return false;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0

	int colorTablesize=0;

	if(biBitCount==8)
		colorTablesize=1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���

	int lineByte=(width * biBitCount/8+3)/4*4;

	//�Զ�����д�ķ�ʽ���ļ�

	FILE *fp=fopen(bmpName,"wb");

	if(fp==0)
		return false;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ

	BITMAPFILEHEADER fileHead;

	fileHead.bfType =0x4D42;//bmp����

	//bfSize��ͼ���ļ�4����ɲ���֮��

	fileHead.bfSize=sizeof(BITMAPFILEHEADER) +sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 =0;

	fileHead.bfReserved2 =0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��

	fileHead.bfOffBits=54+colorTablesize;

	//д�ļ�ͷ���ļ�

	fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ

	BITMAPINFOHEADER head; 

	head.biBitCount=biBitCount;

	head.biClrImportant=0;

	head.biClrUsed=0;

	head.biCompression=0;

	head.biHeight= -height;

	head.biPlanes=1;

	head.biSize=40;

	head.biSizeImage=lineByte*height;

	head.biWidth=width;

	head.biXPelsPerMeter=0;

	head.biYPelsPerMeter=0;

	//дλͼ��Ϣͷ���ڴ�

	fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 

	if(biBitCount==8)
		fwrite(pColorTable, sizeof(RGBQUAD),256, fp);

	//дλͼ���ݽ��ļ�

	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�

	fclose(fp);

	return true;

}

bool CRPDlg::DrawMemBmp(unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	return TRUE;
}

void CRPDlg::OnPaint()
{
	// TODO: �ڴ˴������Ϣ����������
	
	CPaintDC dc(GetDlgItem(IDC_STATIC_SHOW)); 
	
	CBitmap m_bitmap;
	if ( m_bitmap.CreateBitmap( m_iWidth, m_iHeight, 1, 32, m_lpByte ) )
	{
		CDC memdc;
		memdc.CreateCompatibleDC(&dc);
		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(&dc, m_iWidth, m_iHeight);
		memdc.SelectObject( &bitmap );
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)memdc.m_hDC , 0);
		CDC maskdc;
		maskdc.CreateCompatibleDC(&dc);
		CBitmap maskbitmap;
		maskbitmap.CreateBitmap(m_iWidth, m_iHeight, 1, 32, NULL);
		maskdc.SelectObject( &maskbitmap );
		maskdc.BitBlt( 0, 0, m_iWidth, m_iHeight, &memdc,0, 0, SRCCOPY);
		CBrush brush;
		brush.CreatePatternBrush(&m_bitmap);
		dc.FillRect(CRect(0 , 0 , m_iWidth , m_iHeight ), &brush);
		dc.BitBlt(0, 0, m_iWidth , m_iHeight,&memdc, 0, 0,SRCPAINT);
		brush.DeleteObject();
	}
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}




void CRPDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
