// WinFileTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketFunction.h"
#include "WinFileTransfer.h"
#include "WinFileTransferDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	MessageBox("长江大学计科院杨恒，C语言局域网文件传输工具");
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


CWinFileTransferDlg::CWinFileTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinFileTransferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CWinFileTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinFileTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CWinFileTransferDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, OnClient)
	ON_BN_CLICKED(IDC_BUTTON2, OnServer)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CWinFileTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText("局域网文件传输工具");

	GetDlgItem(IDC_EDIT1)->SetWindowText("520");

	GetDlgItem(IDC_IPADDRESS1)->SetWindowText("10.201.2.93");

	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	GetCurrentDirectory(1000,currentDir);

	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(currentDir);

	char *ips[10][20];
	
	getIPs(ips);

	for(int i = 0;*ips[i]!=NULL;i++){
		
		CString msg = "本次文件传输是否使用以下IP：\n";

		CString msg1 = *ips[i];

		msg = msg + msg1;

		if (AfxMessageBox(msg, MB_OKCANCEL) == IDOK) {
			
			((CEdit*)GetDlgItem(IDC_IPADDRESS1))->SetWindowText(*ips[i]);//设置时要保证传入的是char[]数组

			return true;

		}
	
	}


	return TRUE;
}

void CWinFileTransferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CWinFileTransferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CWinFileTransferDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}






void CWinFileTransferDlg::OnServer()
{
		
	char fileAddr[500];//传输文件地址

	for (int a = 0; a < 500; a++) {

		fileAddr[a] = NULL;
	}

	char fileName[200];//传输文件名


	CString filter;
	filter = "所有文件(.*)|*|文本文档(*.txt)|*.txt|PDF文档(*.pdf)|*.pdf|doc文档*.doc||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		for (int i = 0; i < str.GetLength(); i++)
			fileAddr[i] = str.GetAt(i);

	}else return;

	char ask[200] = "即将向客户端发送文件： ";

	for (int k = 0; k<200; k++) {
		ask[22 + k] = fileAddr[k];
	}


	if (AfxMessageBox(ask, MB_OKCANCEL) == IDOK) {

		char *str = "服务器发起文件传输请求，是否接受？";

		send(s_d, str, strlen(str) + 1, 0);

		char confirm;

		int nResult = recv(s_d, &confirm, sizeof(char), 0);

		if (confirm == 'Y' || confirm == 'y') {

			//
			//
			char *str1 = "....确认成功....";

			send(s_d, str1, strlen(str1) + 1, 0); //向客户端发送验证信息

			Sleep(100);

			long i; //文件分的的块数 

			if ((fp = fopen(fileAddr, "rb")) == NULL)//打开文件 
			{

			}

			getW_name(fileAddr, fileName);
			
			char confirm;

			recv(s_d, &confirm, sizeof(char), 0);//收到确认后才发送文件名

			Sleep(100);

			send(s_d, fileName, strlen(fileName) + 1, 0); //发送文件名

			//AfxMessageBox(_T(fileName), MB_OK);

			recv(s_d, &confirm, sizeof(char), 0);

			Sleep(100);

			fseek(fp, 0L, 2);

			long fileLength = ftell(fp); //取得文件的长度 

			fseek(fp, 0L, 0);


			char Length[100];

			LongToChar(fileLength, Length);

			send(s_d, Length, strlen(Length) + 1, 0); //发送文件大小到客户端 

			recv(s_d, &confirm, sizeof(char), 0);

			Sleep(100);

			if (0 == fileLength%M) //对文件进行分块 
			{
				i = fileLength / M;
			}

			else {
				i = fileLength / M + 1;
			}


			char mess[M];

			long readLength; //从文件读取的长度 

			long sendLength; //发送文件的长度 

			int k = 1; //正在传送的模块号 


			char *temp = "开始接收文件"; //向客户端开始传送，确认同步开始

			Sleep(100);

			nResult = recv(s_d, mess, M, 0);

			Sleep(100);

			if (nResult == -1) //判断客户端是否关闭 
			{
				MessageBox("\n客户端已断开\n");

				return;

			}

			send(s_d, temp, strlen(temp) + 1, 0);

			long total = 0;

			while (k <= i)
			{

				Sleep((DWORD)200); //此非常重要，降低了传输时间，保证了两边的同步 

				readLength = fread(mess, sizeof(char), M, fp);

				sendLength = send(s_d, mess, readLength, 0);

				total += sendLength;

				if (sendLength == SOCKET_ERROR)
				{
					MessageBox("失 败!");
					return;
				}
				k++;
			}

			fclose(fp);

			//fp = NULL;

			MessageBox("传输完毕");

		}
		else
			MessageBox("客户端拒绝文件传输请求或客户端不在线！");
	}

}



void CWinFileTransferDlg::OnChangeEdit1()
{

	CEdit *edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);//获取文本框内的值
	CString port1;
	edit1->GetWindowText(port1);//获取文本1的内容 赋值到CString变量里面；
	port = _ttoi(port1);

}

void CWinFileTransferDlg::OnChangeEdit2()
{

	CString s;

	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(s);

	strcpy(currentDir,s);

	//MessageBox(currentDir);

}


void CWinFileTransferDlg::OnButton3()
{
	socket_union soc;

	soc.sc = &s_d;

	CreateThread(NULL, 0, server, &soc, 0, 0);

	((CEdit*)GetDlgItem(IDC_IPADDRESS1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT2))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText("服务器模式下不可用");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);

	SetWindowText("局域网文件传输工具（服务器端）");


}

void CWinFileTransferDlg::OnButton4()
{

	CEdit *edit2 = (CEdit*)GetDlgItem(IDC_IPADDRESS1);//获取文本框内的值

	CString IPaddress;

	edit2->GetWindowText(IPaddress);//获取文本1的内容 赋值到CString变量里面；

	CString directory;

	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(directory);

	SetCurrentDirectory(directory);

	for (int i = 0; i<IPaddress.GetLength(); i++)
	{
		IP[i] = IPaddress.GetAt(i);
	}

	CreateThread(NULL, 0, client, NULL, 0, 0);//等待主线程发送的文件

	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	SetWindowText("局域网文件传输工具（客户端）");
	GetDlgItem(IDC_BUTTON4)->SetWindowText("等待服务器传输文件请求...");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(false);

}
