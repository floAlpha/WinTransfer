#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <Winsock2.h> 
#include <process.h> 
#include <time.h> 



static int nResult;//sendfile��ı���

#pragma comment(lib,"Ws2_32") 

#define M 512000 //ÿ�鴫���ļ��Ĵ�С500k 

static int port;

static char IP[20];

SOCKET s_d; //5���׽��ֵȴ�����:�����������ˣ� 

SOCKET sc; //�ͻ���socket

FILE *fp;//�ļ�����

char currentDir[1000];//Ĭ���ļ�����·��


struct socket_union{
	SOCKET *sc;
};

struct Timeout{
	CButton *button;
};

CString BrowseSaveFile(char fileName[300])
{
	
	CFileDialog dlg(false, NULL, fileName,  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, NULL);

	dlg.m_ofn.lpstrTitle   =   "��ѡ���ļ�����λ��"; 
	
	//AfxMessageBox(_T(currentDir), MB_OK);

	dlg.m_ofn.lpstrInitialDir   =   currentDir;//��ʼ·�� 

	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		
		return fileName;

	}

	return "";

}

void getIPs(char *ips[10][20])  
{  
    WORD v = MAKEWORD(1, 1);    

    WSADATA wsaData;    

    WSAStartup(v, &wsaData); // �����׽��ֿ�    
  
 
    struct hostent *phostinfo = gethostbyname("");    

    for(int i = 0; NULL != phostinfo&& NULL != phostinfo->h_addr_list[i]; ++i)  
    {  
        *ips[i]=inet_ntoa(*(struct in_addr *)phostinfo->h_addr_list[i]);  
    }  

	 *ips[i] = NULL;

    WSACleanup( );   

}

void getW_name(char waddr[], char wname[]);

void LongToChar(long x, char a[]);

long StrToLong(char str[]);


void getW_name(char waddr[], char wname[])
{

	int i, k = 0, j, w_len = strlen(waddr);

	for (i = w_len - 1; i >= 0; i--)
	{
		if (waddr[i] == '\\')
			break;
		wname[k++] = waddr[i];
	}

	wname[k++] = '\0';

	i = strlen(wname);

	for (j = 0, k = i - 1; j<i / 2; j++, k--)
	{
		char temp = wname[j]; wname[j] = wname[k]; wname[k] = temp;
	}

}


int receiveFile()
{

	char mess[M];

	nResult = recv(sc, mess, strlen(mess), 0); //ȷ���ļ�����ģ�����ӳɹ� 


	if (nResult == -1) //�жϷ�����Ƿ�ر� 
	{
		AfxMessageBox(_T("������ѶϿ�"), MB_OK);
	}

	//printf("%s\n", mess);

	//AfxMessageBox(_T(filename), MB_OK);

	send(sc, "Y" , 2, 0); //ȷ�Ͽ�ʼ�����ļ���

	char filename[200];

	nResult = recv(sc, filename, 200, 0); //�����ļ��� 

	CString c = BrowseSaveFile(filename);

	char fileName[300];

	strcpy(fileName,c);

	send(sc, "Y" , 2, 0); //ȷ���ļ���


	if (nResult == -1) //�жϷ�����Ƿ�ر� 
	{

		AfxMessageBox(_T("������ѶϿ�"), MB_OK);

	}

	nResult = recv(sc, mess, M, 0); //�����ļ��Ĵ�С 

	send(sc, "Y" , 2, 0); //ȷ���ļ���С
									

	if (nResult == 804 - 1) //�жϷ�����Ƿ�ر� 
	{
		AfxMessageBox(_T("������ѶϿ�"), MB_OK);

	}

	long fileLength = StrToLong(mess); //ȡ�Ĵ������ļ��ĳ��� 

	long i; //����ļ����� 


	if (0 == fileLength%M) //���ļ���N��λ���зֿ� 
	{
		i = fileLength / M;
	}

	else
	{
		i = fileLength / M + 1;
	}

	int k = 1; //��ǰ���ڽ����ļ��Ŀ��� 

	long receiveLength; //�����ļ��ĳ��� 

	long writeLength; //��д���ļ����ַ����� 

	fp = fopen(filename, "wb");

	char *temp = "��ʼ�����ļ�";

	send(sc, temp, strlen(temp) + 1, 0); //�����˿�ʼ���ͣ�ȷ��ͬ����ʼ 

	nResult = recv(sc, mess, M, 0); //���շ���˵�ȷ����Ϣ 

	if (nResult == -1) //�жϷ�����Ƿ�ر� 
	{
		AfxMessageBox(_T("������ѶϿ�"), MB_OK);

	}

	long total = 0; //�ѽ����ļ��ĳ��� 

	while (k <= i) //ѭ���Ľ����ļ� 
	{
		receiveLength = recv(sc, mess, M, 0);

		//send(sc, "y", 2, 0); //����ȷ��

		if (receiveLength == -1) //�жϷ�����Ƿ�ر� 
		{
			AfxMessageBox(_T("������ѶϿ�"), MB_OK);
		}

		writeLength = fwrite(mess, sizeof(char), receiveLength, fp);

		total += writeLength;

		k++;

	}

	fclose(fp);

	fp = NULL;

	return 0;

}



void LongToChar(long x, char a[])
{
	int count = 0;
	if (x == 0)
		return;
	while (x)
	{

		a[count++] = x % 10 + '0';

		x = x / 10;

	}

	a[count] = '\0';

	for (int i = 0, j = count - 1; i<count / 2; i++, j--)

	{
		char temp;

		temp = a[i];

		a[i] = a[j];

		a[j] = temp;
	}

}


long StrToLong(char str[])
{

	long result;

	bool bEnd;

	bEnd = true;

	result = 0;

	for (int i = 0; bEnd; i++)
	{
		if (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
		{
			result = result * 10 + str[i] - '0';
		}
		if (str[i] == '\0')
		{
			bEnd = false;
		}
	}

	return result;

}


DWORD WINAPI server(LPVOID pM)
{



	WORD wVersion = MAKEWORD(1, 1);

	WSADATA wsData;

	int nResult = WSAStartup(wVersion, &wsData); //1������Winsock 

	if (nResult != 0)
	{
		//printf("����Winsockʧ��!\n");
	}


	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); //2.�����׽��� 
	if (s == INVALID_SOCKET)
	{
		//printf("�����׽���ʧ��!\n");
	}


	SOCKADDR_IN addr;

	addr.sin_family = AF_INET;

init:
	
	addr.sin_port = htons(port); //��֤�ֽ�˳�� 

	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	nResult = bind(s, (sockaddr*)&addr, sizeof(sockaddr)); //3���׽��ֵİ� 

	if (nResult == SOCKET_ERROR)
	{
		AfxMessageBox(_T("�˿ڱ�ռ�ã�����ʹ��Ĭ�϶˿�"), MB_OK);
		port = 520;
		goto init;

	}


	nResult = listen(s, 5); //���5�����ӣ� �׽��ֵļ��� 

	if (nResult == SOCKET_ERROR)
	{
		//printf("�׽��ֵļ���(��������)ʧ��!\n");
	}

	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);

	//printf("\n�ȴ��ͻ��˵�����");

	s_d = accept(s, (sockaddr*)&addrClient, &len); //5���׽��ֵȴ�����:�����������ˣ� 

	if (s_d == INVALID_SOCKET)
	{
		//printf("�׽��ֵȴ�����(��������)ʧ��!\n");
	}

	char *success = "�ɹ���½������!";

	send(s_d, success, strlen(success) + 1, 0); //��ͻ��˷�����֤��Ϣ 

	char mess[M];

	nResult = recv(s_d, mess, strlen(mess), 0);

	AfxMessageBox(_T("�ͻ����ѳɹ����ӷ�����"), MB_OK);

	if (nResult == -1) //�жϷ�����Ƿ�ر� 
	{
		AfxMessageBox(_T("�ͻ����ѶϿ�"), MB_OK);

		exit(0);
	}

	//int nNetTimeout=3000;//1�룬
	//���÷��ͳ�ʱ
	//setsockopt(s_d,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//���ý��ճ�ʱ

	return NULL;
}

bool recvOnClient() {

	receiveFile();

	return true;

}


DWORD WINAPI Listening(LPVOID pM){//�������̼߳����Ƿ����ļ�����

	char rMess[100];

	int nResult = recv(sc, rMess, strlen(rMess), 0); //���շ���˷����Ĳ������� 
		
	if(nResult < 0){

		AfxMessageBox("�������쳣�رգ��ͻ��˳��򼴽��˳�", MB_OKCANCEL);

		exit(0);
	
	}

	char confirm = 'N';

	if (AfxMessageBox(rMess, MB_OKCANCEL) == IDOK) {

		confirm = 'Y' ;

		send(sc, &confirm , sizeof(char), 0); //�����˷���ѡ�񣬲�ִ����Ӧ���� 


	}else{
	
		confirm = 'N' ;

		send(sc, &confirm , sizeof(char), 0); //�����˷���ѡ�񣬲�ִ����Ӧ���� 

	}	
		
	if ((confirm == 'Y' || confirm == 'y'))
	{
		
		recvOnClient();
		
		AfxMessageBox("�������",MB_OK);
		
		CreateThread(NULL, 0, Listening, NULL, 0, 0);

		

	}else{

		CreateThread(NULL, 0, Listening, NULL, 0, 0);

		return NULL;

	}

	return NULL;

}

DWORD WINAPI client(LPVOID pM)
{

	//�����׽���
	socket:
	WORD wVersion = MAKEWORD(1, 1);

	WSADATA wsData;

	int nResult = WSAStartup(wVersion, &wsData); //����WINSOCKET 

	if (nResult != 0)
	{
		if (AfxMessageBox("����Winsockʧ��!�Ƿ����ԣ�", MB_OKCANCEL) == IDOK) {
		
			goto socket;
		
		}

		 return NULL;
	}


	sc = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); //�����׽��� 

	if (sc == INVALID_SOCKET)
	{

		if (AfxMessageBox("�����׽���ʧ��!�Ƿ����ԣ�", MB_OKCANCEL) == IDOK) {
		
			goto socket;
		
		}

		 return NULL;
	}


	SOCKADDR_IN addrSc;

	addrSc.sin_family = AF_INET;

	addrSc.sin_port = htons(port); //��֤�ֽ�˳�� 

	addrSc.sin_addr.S_un.S_addr = inet_addr(IP);

	int b = 0;

	while (b<1) //���3�Σ�����������ڴ�ʱ������������������� 
	{
		nResult = connect(sc, (SOCKADDR*)&addrSc, sizeof(SOCKADDR)); //�׽������� 

		//Sleep(500); //��ʱ (DWORD)

		if (nResult == SOCKET_ERROR)
		{

		}
		else

			break;
		b++;
	}

	if (nResult == SOCKET_ERROR)
	{

		if (AfxMessageBox("���ӷ�������ʱ��ȷ��������ѿ�������������IP��ַ�Ƿ�Ϸ�,�������ٴ�����", MB_OKCANCEL) == IDOK) {

			//Timeout *t = (Timeout*)pM;
			
			//t->button->EnableWindow(true);
		
			//return NULL;

		}
		

	}


	char *buf = "���ӳɹ�!"; //��������֤���ӳɹ� 

	nResult = send(sc, buf, strlen(buf) + 1, 0);

	if (nResult == SOCKET_ERROR)
	{
		AfxMessageBox(_T("���ӷ�����ʧ��"), MB_OKCANCEL);

		return NULL;

	}

	char mess[M];

	nResult = recv(sc, mess, strlen(mess), 0); //���ܷ���˵�������֤��Ϣ 


	AfxMessageBox(_T("�ɹ���������������"), MB_OK);

	if (nResult == -1) //�жϷ�����Ƿ�ر� 
	{
		//printf("\n������ѶϿ������򽫳��Ը��Ķ˿��Խ���쳣\n");

		//system("pause");



		port = 520;

		AfxMessageBox(_T("���ӷ�����ʧ��"), MB_OKCANCEL);
		
		return NULL;
	}

	CreateThread(NULL, 0, Listening, NULL, 0, 0);

	//int nNetTimeout=3000;//1�룬
	//���÷��ͳ�ʱ
	//setsockopt(sc,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));

	//return 0;
	return NULL;
}

