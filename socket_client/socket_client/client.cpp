#include<winsock.h>
#include <string.h>
#include<iostream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void initialization();
int main() {
	initialization();
	int client_port = 1234;

	struct sockaddr_in se_addr;//ipv4��Ϣ�ṹ��
	se_addr.sin_family = AF_INET; /* �����ֽ��� */
	se_addr.sin_port = htons(client_port); /* short, �����ֽ��� */
	se_addr.sin_addr.s_addr = inet_addr("127.0.0.3");
	//my_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	
	SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP//�����׽���
	//memset(&(my_addr.sin_zero), 0, 8);//��ֵ��ʼ��
	//connect(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	if (connect(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "����������ʧ�ܣ�" << endl;
	}
	
	char rec_buf[100];
	char send_buf[100];
	while (1) {
		cout << "�����뷢����Ϣ:";
		cin >> send_buf;
		int send_len = send(sockse, send_buf, 100, 0);
		
		int recv_len = recv(sockse, rec_buf, 100, 0);
		
			cout << "�������Ϣ:" << rec_buf << endl;
		

	}
	closesocket(sockse);

}
void initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	err = WSAStartup(w_req, &wsadata);
	if (err != 0) {
		cout << "��ʼ���׽��ֿ�ʧ�ܣ�" << endl;
	}
	else {
		cout << "��ʼ���׽��ֿ�ɹ���" << endl;
	}
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	else {
		cout << "�׽��ֿ�汾��ȷ��" << endl;
	}
	//������˵�ַ��Ϣ

}
