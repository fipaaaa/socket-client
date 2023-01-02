#include<winsock.h>
#include <string.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main() {
	int client_port = 8000;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//试试TCP
	struct sockaddr_in my_addr;//ipv4信息结构体
	my_addr.sin_family = AF_INET; /* 主机字节序 */
	my_addr.sin_port = htons(client_port); /* short, 网络字节序 */
	my_addr.sin_addr.s_addr = inet_addr("192.168.0.2");
	memset(&(my_addr.sin_zero), 0, 8);//数值初始化
	connect(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	char rec_buf[10000];
	char send_buf[10000];
	while (1) {
		cout << "请输入发送信息:";
		cin >> send_buf;
		send(sockfd, send_buf, 100, 0);
		
		recv(sockfd, rec_buf, 100, 0);
		
		cout << "服务端信息:" << rec_buf << endl;
		

	}

}