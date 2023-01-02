#include<winsock.h>
#include <string.h>
#include<iostream>
#include <thread>
#include<Windows.h>
#include<time.h>  
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

DWORD WINAPI ThreadFunc(LPVOID);
void initialization();
int main()
{
	initialization();
	int server_port = 1234;

	struct sockaddr_in se_addr;//ipv4信息结构体
	struct sockaddr_in cl_addr;
	se_addr.sin_family = AF_INET; /* 主机字节序 */
	se_addr.sin_port = htons(server_port); /* short, 网络字节序 */
	se_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//试试TCP//创建套接字
	SOCKET sockcl;

	//memset(&(my_addr.sin_zero), 0, 8);//数值初始化
	//bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));//赋予地址协议
	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "套接字占用" << endl;
	}
	else {
		cout << "套接字可用" << endl;
	}
	listen(sockse, 5);//最大接受数量为10个
	int len = sizeof(SOCKADDR);
	HANDLE hThread;//windows多线程
	DWORD  threadId;
	while (1) {
		sockcl = accept(sockse, (struct sockaddr*)&cl_addr, &len);
		hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)sockcl, 0, NULL);
		if (hThread == NULL) {
			cout << "创建子进程失败。" << endl;
			break;
		}
			
			CloseHandle(hThread);
		}
	    closesocket(sockse);
		closesocket(sockcl);
	return 0;
}

DWORD WINAPI ThreadFunc(LPVOID cl)
{
	SOCKET sockcl= (SOCKET)cl;
	char rec_buf[100];
	char send_buf[100];
	while (1) {
		int recv_len = recv(sockcl, rec_buf, 100, 0);


		cout << sockcl<<"信息:" << rec_buf << endl;
		cout << "已发送本机时间信息";
		time_t t = time(NULL);
		tm* local = localtime(&t); //转为本地时间
		strftime(send_buf, 64, "%Y-%m-%d %H:%M:%S", local);
		int send_len = send(sockcl,send_buf, 100, 0);
		break;
	}
	return 0;
}

void initialization() {
	//初始化套接字库
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "初始化套接字库失败！" << endl;
	}
	else {
		cout << "初始化套接字库成功！" << endl;
	}
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	else {
		cout << "套接字库版本正确！" << endl;
	}
	//填充服务端地址信息
}
