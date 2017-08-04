#pragma once
#include "afxcmn.h"


// CRPDlg 对话框

class CRPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRPDlg)

public:
	CRPDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRPDlg();

// 对话框数据
	enum { IDD = IDD_RP_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CString m_strURI;
	CString m_strRecvAddr;
	CString m_strRecvLabel;
	CString m_strMoney;
	CString m_strRecvMessage;
	afx_msg void OnBnClickedButtonCopyuri();
	afx_msg void OnBnClickedButtonCopyaddr();
	afx_msg void OnBnClickedButtonSavebmp();

	string CombineURI( string strAddr, string strLabel, string strMsg, string strMoney);
//	CRichEditCtrl m_richeditInfo;
	CRichEditCtrl m_richeditInfo;

private:

	int m_iWidth;
	int m_iHeight;
	BYTE *m_lpByte;

	char qrcodeErrorCorrect;
	char qrcodeEncodeMode;
	int qrcodeVersion;
	int qrcodeStructureappendN;
	int qrcodeStructureappendM;
	int qrcodeStructureappendParity;
	CString qrcodeStructureappendOriginaldata;

public:
	void	setQrcodeErrorCorrect(char c);
	char	getQrcodeErrorCorrect();

	void	setQrcodeVersion(int i);
	int		getQrcodeVersion();

	void	setQrcodeEncodeMode(char c);
	char	getQrcodeEncodeMode();

	void	setStructureappend(int i, int j, int k);
	void	TestMsg(char* myStr,int length);

	bool** calQrcode(char* abyte0,int iLen0,int& iLenRet);

	int calStructureappendParity(char* abyte0,int i);


private: //私有方法
	char* divideDataBy8Bits(int* ai, char* abyte0, int i,int iLen,int& iLRet);//,,,返回长度
	char* calculateRSECC(char* abyte0, char byte0, char* abyte1, int i, int j,
		int iLen0,int iLen1);	
	char selectMask(char** abyte0, int i,int iLen0);
	char* calculateByteArrayBits(char* abyte0, char* abyte1, CString s,
		int iLen0,int iLen1,int& iLRet);

public:	
	BOOL DrawMemBmp( LPTSTR szFile, CWnd *pWnd);
	HANDLE DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );
	BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB);

	BYTE* GenerateBarCode(const char *_pStrInfo, int *_pWidth, int *_pHeight);
	bool SaveBmp(char*bmpName, unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);
	bool DrawMemBmp(unsigned char*imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable);

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonClose();
};
