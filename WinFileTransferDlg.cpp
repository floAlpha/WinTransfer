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
	MessageBox("������ѧ�ƿ�Ժ��㣬C���Ծ������ļ����乤��");
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

	SetWindowText("�������ļ����乤��");

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
		
		CString msg = "�����ļ������Ƿ�ʹ������IP��\n";

		CString msg1 = *ips[i];

		msg = msg + msg1;

		if (AfxMessageBox(msg, MB_OKCANCEL) == IDOK) {
			
			((CEdit*)GetDlgItem(IDC_IPADDRESS1))->SetWindowText(*ips[i]);//����ʱҪ��֤�������char[]����

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
		
	char fileAddr[500];//�����ļ���ַ

	for (int a = 0; a < 500; a++) {

		fileAddr[a] = NULL;
	}

	char fileName[200];//�����ļ���


	CString filter;
	filter = "�����ļ�(.*)|*|�ı��ĵ�(*.txt)|*.txt|PDF�ĵ�(*.pdf)|*.pdf|doc�ĵ�*.doc||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		for (int i = 0; i < str.GetLength(); i++)
			fileAddr[i] = str.GetAt(i);

	}else return;

	char ask[200] = "������ͻ��˷����ļ��� ";

	for (int k = 0; k<200; k++) {
		ask[22 + k] = fileAddr[k];
	}


	if (AfxMessageBox(ask, MB_OKCANCEL) == IDOK) {

		char *str = "�����������ļ����������Ƿ���ܣ�";

		send(s_d, str, strlen(str) + 1, 0);

		char confirm;

		int nResult = recv(s_d, &confirm, sizeof(char), 0);

		if (confirm == 'Y' || confirm == 'y') {

			//
			//
			char *str1 = "....ȷ�ϳɹ�....";

			send(s_d, str1, strlen(str1) + 1, 0); //��ͻ��˷�����֤��Ϣ

			Sleep(100);

			long i; //�ļ��ֵĵĿ��� 

			if ((fp = fopen(fileAddr, "rb")) == NULL)//���ļ� 
			{

			}

			getW_name(fileAddr, fileName);
			
			char confirm;

			recv(s_d, &confirm, sizeof(char), 0);//�յ�ȷ�Ϻ�ŷ����ļ���

			Sleep(100);

			send(s_d, fileName, strlen(fileName) + 1, 0); //�����ļ���

			//AfxMessageBox(_T(fileName), MB_OK);

			recv(s_d, &confirm, sizeof(char), 0);

			Sleep(100);

			fseek(fp, 0L, 2);

			long fileLength = ftell(fp); //ȡ���ļ��ĳ��� 

			fseek(fp, 0L, 0);


			char Length[100];

			LongToChar(fileLength, Length);

			send(s_d, Length, strlen(Length) + 1, 0); //�����ļ���С���ͻ��� 

			recv(s_d, &confirm, sizeof(char), 0);

			Sleep(100);

			if (0 == fileLength%M) //���ļ����зֿ� 
			{
				i = fileLength / M;
			}

			else {
				i = fileLength / M + 1;
			}


			char mess[M];

			long readLength; //���ļ���ȡ�ĳ��� 

			long sendLength; //�����ļ��ĳ��� 

			int k = 1; //���ڴ��͵�ģ��� 


			char *temp = "��ʼ�����ļ�"; //��ͻ��˿�ʼ���ͣ�ȷ��ͬ����ʼ

			Sleep(100);

			nResult = recv(s_d, mess, M, 0);

			Sleep(100);

			if (nResult == -1) //�жϿͻ����Ƿ�ر� 
			{
				MessageBox("\n�ͻ����ѶϿ�\n");

				return;

			}

			send(s_d, temp, strlen(temp) + 1, 0);

			long total = 0;

			while (k <= i)
			{

				Sleep((DWORD)200); //�˷ǳ���Ҫ�������˴���ʱ�䣬��֤�����ߵ�ͬ�� 

				readLength = fread(mess, sizeof(char), M, fp);

				sendLength = send(s_d, mess, readLength, 0);

				total += sendLength;

				if (sendLength == SOCKET_ERROR)
				{
					MessageBox("ʧ ��!");
					return;
				}
				k++;
			}

			fclose(fp);

			//fp = NULL;

			MessageBox("�������");

		}
		else
			MessageBox("�ͻ��˾ܾ��ļ����������ͻ��˲����ߣ�");
	}

}



void CWinFileTransferDlg::OnChangeEdit1()
{

	CEdit *edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);//��ȡ�ı����ڵ�ֵ
	CString port1;
	edit1->GetWindowText(port1);//��ȡ�ı�1������ ��ֵ��CString�������棻
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
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText("������ģʽ�²�����");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);

	SetWindowText("�������ļ����乤�ߣ��������ˣ�");


}

void CWinFileTransferDlg::OnButton4()
{

	CEdit *edit2 = (CEdit*)GetDlgItem(IDC_IPADDRESS1);//��ȡ�ı����ڵ�ֵ

	CString IPaddress;

	edit2->GetWindowText(IPaddress);//��ȡ�ı�1������ ��ֵ��CString�������棻

	CString directory;

	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(directory);

	SetCurrentDirectory(directory);

	for (int i = 0; i<IPaddress.GetLength(); i++)
	{
		IP[i] = IPaddress.GetAt(i);
	}

	CreateThread(NULL, 0, client, NULL, 0, 0);//�ȴ����̷߳��͵��ļ�

	//GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	SetWindowText("�������ļ����乤�ߣ��ͻ��ˣ�");
	GetDlgItem(IDC_BUTTON4)->SetWindowText("�ȴ������������ļ�����...");
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT1))->EnableWindow(false);

}
