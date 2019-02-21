#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <Winsock2.h> 
#include <process.h> 
#include <time.h> 



static int nResult;//sendfile里的变量

#pragma comment(lib,"Ws2_32") 

#define M 512000 //每块传送文件的大小500k 

static int port;

static char IP[20];

SOCKET s_d; //5、套接字等待连接:：（服务器端） 

SOCKET sc; //客户端socket

FILE *fp;//文件操作

char currentDir[1000];//默认文件保存路径


struct socket_union{
	SOCKET *sc;
};

struct Timeout{
	CButton *button;
};

CString BrowseSaveFile(char fileName[300])
{
	
	CFileDialog dlg(false, NULL, fileName,  OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, NULL);

	dlg.m_ofn.lpstrTitle   =   "请选择文件保存位置"; 
	
	//AfxMessageBox(_T(currentDir), MB_OK);

	dlg.m_ofn.lpstrInitialDir   =   currentDir;//初始路径 

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

    WSAStartup(v, &wsaData); // 加载套接字库    
  
 
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

	nResult = recv(sc, mess, strlen(mess), 0); //确认文件传送模块连接成功 


	if (nResult == -1) //判断服务端是否关闭 
	{
		AfxMessageBox(_T("服务端已断开"), MB_OK);
	}

	//printf("%s\n", mess);

	//AfxMessageBox(_T(filename), MB_OK);

	send(sc, "Y" , 2, 0); //确认开始接受文件名

	char filename[200];

	nResult = recv(sc, filename, 200, 0); //接收文件名 

	CString c = BrowseSaveFile(filename);

	char fileName[300];

	strcpy(fileName,c);

	send(sc, "Y" , 2, 0); //确认文件名


	if (nResult == -1) //判断服务端是否关闭 
	{

		AfxMessageBox(_T("服务端已断开"), MB_OK);

	}

	nResult = recv(sc, mess, M, 0); //接受文件的大小 

	send(sc, "Y" , 2, 0); //确认文件大小
									

	if (nResult == 804 - 1) //判断服务端是否关闭 
	{
		AfxMessageBox(_T("服务端已断开"), MB_OK);

	}

	long fileLength = StrToLong(mess); //取的待传送文件的长度 

	long i; //存放文件块数 


	if (0 == fileLength%M) //对文件按N单位进行分块 
	{
		i = fileLength / M;
	}

	else
	{
		i = fileLength / M + 1;
	}

	int k = 1; //当前正在接收文件的块数 

	long receiveLength; //接收文件的长度 

	long writeLength; //已写到文件的字符长度 

	fp = fopen(filename, "wb");

	char *temp = "开始传送文件";

	send(sc, temp, strlen(temp) + 1, 0); //向服务端开始传送，确认同步开始 

	nResult = recv(sc, mess, M, 0); //接收服务端的确认消息 

	if (nResult == -1) //判断服务端是否关闭 
	{
		AfxMessageBox(_T("服务端已断开"), MB_OK);

	}

	long total = 0; //已接收文件的长度 

	while (k <= i) //循环的接收文件 
	{
		receiveLength = recv(sc, mess, M, 0);

		//send(sc, "y", 2, 0); //发送确认

		if (receiveLength == -1) //判断服务端是否关闭 
		{
			AfxMessageBox(_T("服务端已断开"), MB_OK);
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

	int nResult = WSAStartup(wVersion, &wsData); //1、启动Winsock 

	if (nResult != 0)
	{
		//printf("启动Winsock失败!\n");
	}


	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); //2.创建套接字 
	if (s == INVALID_SOCKET)
	{
		//printf("创建套接字失败!\n");
	}


	SOCKADDR_IN addr;

	addr.sin_family = AF_INET;

init:
	
	addr.sin_port = htons(port); //保证字节顺序 

	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	nResult = bind(s, (sockaddr*)&addr, sizeof(sockaddr)); //3、套接字的绑定 

	if (nResult == SOCKET_ERROR)
	{
		AfxMessageBox(_T("端口被占用，程序将使用默认端口"), MB_OK);
		port = 520;
		goto init;

	}


	nResult = listen(s, 5); //最多5个连接， 套接字的监听 

	if (nResult == SOCKET_ERROR)
	{
		//printf("套接字的监听(服务器端)失败!\n");
	}

	SOCKADDR_IN addrClient;

	int len = sizeof(SOCKADDR);

	//printf("\n等待客户端的连接");

	s_d = accept(s, (sockaddr*)&addrClient, &len); //5、套接字等待连接:：（服务器端） 

	if (s_d == INVALID_SOCKET)
	{
		//printf("套接字等待连接(服务器端)失败!\n");
	}

	char *success = "成功登陆服务器!";

	send(s_d, success, strlen(success) + 1, 0); //向客户端发送验证信息 

	char mess[M];

	nResult = recv(s_d, mess, strlen(mess), 0);

	AfxMessageBox(_T("客户端已成功连接服务器"), MB_OK);

	if (nResult == -1) //判断服务端是否关闭 
	{
		AfxMessageBox(_T("客户端已断开"), MB_OK);

		exit(0);
	}

	//int nNetTimeout=3000;//1秒，
	//设置发送超时
	//setsockopt(s_d,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
	//设置接收超时

	return NULL;
}

bool recvOnClient() {

	receiveFile();

	return true;

}


DWORD WINAPI Listening(LPVOID pM){//创建新线程监听是否有文件发送

	char rMess[100];

	int nResult = recv(sc, rMess, strlen(rMess), 0); //接收服务端发来的操作请求 
		
	if(nResult < 0){

		AfxMessageBox("服务器异常关闭，客户端程序即将退出", MB_OKCANCEL);

		exit(0);
	
	}

	char confirm = 'N';

	if (AfxMessageBox(rMess, MB_OKCANCEL) == IDOK) {

		confirm = 'Y' ;

		send(sc, &confirm , sizeof(char), 0); //向服务端反馈选择，并执行相应操作 


	}else{
	
		confirm = 'N' ;

		send(sc, &confirm , sizeof(char), 0); //向服务端反馈选择，并执行相应操作 

	}	
		
	if ((confirm == 'Y' || confirm == 'y'))
	{
		
		recvOnClient();
		
		AfxMessageBox("接收完毕",MB_OK);
		
		CreateThread(NULL, 0, Listening, NULL, 0, 0);

		

	}else{

		CreateThread(NULL, 0, Listening, NULL, 0, 0);

		return NULL;

	}

	return NULL;

}

DWORD WINAPI client(LPVOID pM)
{

	//创建套接字
	socket:
	WORD wVersion = MAKEWORD(1, 1);

	WSADATA wsData;

	int nResult = WSAStartup(wVersion, &wsData); //启动WINSOCKET 

	if (nResult != 0)
	{
		if (AfxMessageBox("启动Winsock失败!是否重试？", MB_OKCANCEL) == IDOK) {
		
			goto socket;
		
		}

		 return NULL;
	}


	sc = socket(AF_INET, SOCK_STREAM, IPPROTO_IP); //创建套接字 

	if (sc == INVALID_SOCKET)
	{

		if (AfxMessageBox("创建套接字失败!是否重试？", MB_OKCANCEL) == IDOK) {
		
			goto socket;
		
		}

		 return NULL;
	}


	SOCKADDR_IN addrSc;

	addrSc.sin_family = AF_INET;

	addrSc.sin_port = htons(port); //保证字节顺序 

	addrSc.sin_addr.S_un.S_addr = inet_addr(IP);

	int b = 0;

	while (b<1) //检测3次，如果服务器在此时间内启动，则进行连接 
	{
		nResult = connect(sc, (SOCKADDR*)&addrSc, sizeof(SOCKADDR)); //套接字连接 

		//Sleep(500); //延时 (DWORD)

		if (nResult == SOCKET_ERROR)
		{

		}
		else

			break;
		b++;
	}

	if (nResult == SOCKET_ERROR)
	{

		if (AfxMessageBox("连接服务器超时，确保服务端已开启并检查服务器IP地址是否合法,检查参数再次重试", MB_OKCANCEL) == IDOK) {

			//Timeout *t = (Timeout*)pM;
			
			//t->button->EnableWindow(true);
		
			//return NULL;

		}
		

	}


	char *buf = "连接成功!"; //向服务端验证连接成功 

	nResult = send(sc, buf, strlen(buf) + 1, 0);

	if (nResult == SOCKET_ERROR)
	{
		AfxMessageBox(_T("连接服务器失败"), MB_OKCANCEL);

		return NULL;

	}

	char mess[M];

	nResult = recv(sc, mess, strlen(mess), 0); //接受服务端的连接验证信息 


	AfxMessageBox(_T("成功连接至服务器端"), MB_OK);

	if (nResult == -1) //判断服务端是否关闭 
	{
		//printf("\n服务端已断开，程序将尝试更改端口以解决异常\n");

		//system("pause");



		port = 520;

		AfxMessageBox(_T("连接服务器失败"), MB_OKCANCEL);
		
		return NULL;
	}

	CreateThread(NULL, 0, Listening, NULL, 0, 0);

	//int nNetTimeout=3000;//1秒，
	//设置发送超时
	//setsockopt(sc,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));

	//return 0;
	return NULL;
}

