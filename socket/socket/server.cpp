#include<winsock.h>
#include <string.h>
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
	//my_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

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
	//accept(sockfd, (struct sockaddr*)&my_addr, &len);
	sockcl = accept(sockse, (struct sockaddr*)&cl_addr, &len);
	char rec_buf[100];
	char send_buf[100];
	while (1) {
		int recv_len = recv(sockcl, rec_buf, 100, 0);
		
		
			cout << "客户端信息:" << rec_buf << endl;
		
		cout << "请输入回复信息:";
		cin >> send_buf;
		int send_len = send(sockcl, send_buf, 100, 0);
	}
	closesocket(sockse);
	closesocket(sockcl);
	//释放DLL资源
	return 0;
}
void initialization(){
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
