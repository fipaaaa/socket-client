#include<winsock.h>
#include <string.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void initialization();
int main() {
	initialization();
	int client_port = 1234;

	struct sockaddr_in se_addr;//ipv4信息结构体
	se_addr.sin_family = AF_INET; /* 主机字节序 */
	se_addr.sin_port = htons(client_port); /* short, 网络字节序 */
	se_addr.sin_addr.s_addr = inet_addr("127.0.0.3");
	//my_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//试试TCP//创建套接字
	//memset(&(my_addr.sin_zero), 0, 8);//数值初始化
	//connect(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	if (connect(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "服务器连接失败！" << endl;
	}
	
	char rec_buf[100];
	char send_buf[100];
	while (1) {
		cout << "请输入发送信息:";
		cin >> send_buf;
		int send_len = send(sockse, send_buf, 100, 0);
		
		int recv_len = recv(sockse, rec_buf, 100, 0);
		
			cout << "服务端信息:" << rec_buf << endl;
		

	}
	closesocket(sockse);

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
