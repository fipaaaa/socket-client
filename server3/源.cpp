#include<winsock.h>
#include <string.h>
#include<time.h>  
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;


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

	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "套接字占用" << endl;
	}
	else {
		cout << "套接字可用" << endl;
	}
	listen(sockse, 5);//最大接受数量为10个

	fd_set redset;//文件描述符
	FD_ZERO(&redset);//初始化
	FD_SET(sockse, &redset);
	int so_max = sockse;
	while (1) {
		FD_SET tmp = redset;//每次循环都进行初始化排序，删去结束的进程
		int re = select(so_max + 1,&tmp,NULL,NULL,NULL);
		if (FD_ISSET(sockse, &tmp)) {
			sockcl = accept(sockse,NULL,NULL);
			FD_SET(sockcl, &redset);
			if (sockcl > so_max) {
				so_max = sockcl;
				cout << "最大套接字已更改:"<<so_max<< endl;
			}
			else {
				cout << "最大套接字未更改:"<<so_max << endl;
			}
		}
		for (int i = 0; i <= so_max+1; ++i) {
			if (i != sockse && FD_ISSET(i, &tmp)) {
				int len = sizeof(SOCKADDR);
				//sockcl = accept(sockse, (struct sockaddr*)&cl_addr, &len);
				char rec_buf[100];
				char send_buf[100];
				
				int recv_len = recv(i, rec_buf, 100, 0);
				if (recv_len == -1) {
					cout << "接收出错" << endl;
				}
				else if (len == 0) {
					cout << "客户端已断开" << endl;
					FD_CLR(i, &redset);
					closesocket(i);
				}
				cout << i << "信息:" << rec_buf << endl;
				cout << "已发送本机时间信息"<<endl;
				time_t t = time(NULL);
				tm* local = localtime(&t); //转为本地时间
				strftime(send_buf, 64, "%Y-%m-%d %H:%M:%S", local);
				int send_len = send(i, send_buf, 100, 0);
				if (send_len == -1) {
					cout << "发送失败" << endl;
					exit(1);
				}
			}
		}
	}
	closesocket(sockse);
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
