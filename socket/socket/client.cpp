#include<winsock.h>
#include <string.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main() {
	int client_port = 8000;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP
	struct sockaddr_in my_addr;//ipv4��Ϣ�ṹ��
	my_addr.sin_family = AF_INET; /* �����ֽ��� */
	my_addr.sin_port = htons(client_port); /* short, �����ֽ��� */
	my_addr.sin_addr.s_addr = inet_addr("192.168.0.2");
	memset(&(my_addr.sin_zero), 0, 8);//��ֵ��ʼ��
	connect(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	char rec_buf[10000];
	char send_buf[10000];
	while (1) {
		cout << "�����뷢����Ϣ:";
		cin >> send_buf;
		send(sockfd, send_buf, 100, 0);
		
		recv(sockfd, rec_buf, 100, 0);
		
		cout << "�������Ϣ:" << rec_buf << endl;
		

	}

}