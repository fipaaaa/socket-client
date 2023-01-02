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

	struct sockaddr_in se_addr;//ipv4��Ϣ�ṹ��
	struct sockaddr_in cl_addr;
	se_addr.sin_family = AF_INET; /* �����ֽ��� */
	se_addr.sin_port = htons(server_port); /* short, �����ֽ��� */
	se_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP//�����׽���
	SOCKET sockcl;

	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "�׽���ռ��" << endl;
	}
	else {
		cout << "�׽��ֿ���" << endl;
	}
	listen(sockse, 5);//����������Ϊ10��

	fd_set redset;//�ļ�������
	FD_ZERO(&redset);//��ʼ��
	FD_SET(sockse, &redset);
	int so_max = sockse;
	while (1) {
		FD_SET tmp = redset;//ÿ��ѭ�������г�ʼ������ɾȥ�����Ľ���
		int re = select(so_max + 1,&tmp,NULL,NULL,NULL);
		if (FD_ISSET(sockse, &tmp)) {
			sockcl = accept(sockse,NULL,NULL);
			FD_SET(sockcl, &redset);
			if (sockcl > so_max) {
				so_max = sockcl;
				cout << "����׽����Ѹ���:"<<so_max<< endl;
			}
			else {
				cout << "����׽���δ����:"<<so_max << endl;
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
					cout << "���ճ���" << endl;
				}
				else if (len == 0) {
					cout << "�ͻ����ѶϿ�" << endl;
					FD_CLR(i, &redset);
					closesocket(i);
				}
				cout << i << "��Ϣ:" << rec_buf << endl;
				cout << "�ѷ��ͱ���ʱ����Ϣ"<<endl;
				time_t t = time(NULL);
				tm* local = localtime(&t); //תΪ����ʱ��
				strftime(send_buf, 64, "%Y-%m-%d %H:%M:%S", local);
				int send_len = send(i, send_buf, 100, 0);
				if (send_len == -1) {
					cout << "����ʧ��" << endl;
					exit(1);
				}
			}
		}
	}
	closesocket(sockse);
	return 0;
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
