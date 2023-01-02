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

	struct sockaddr_in se_addr;//ipv4��Ϣ�ṹ��
	struct sockaddr_in cl_addr;
	se_addr.sin_family = AF_INET; /* �����ֽ��� */
	se_addr.sin_port = htons(server_port); /* short, �����ֽ��� */
	se_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//����TCP//�����׽���
	SOCKET sockcl;

	//memset(&(my_addr.sin_zero), 0, 8);//��ֵ��ʼ��
	//bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));//�����ַЭ��
	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "�׽���ռ��" << endl;
	}
	else {
		cout << "�׽��ֿ���" << endl;
	}
	listen(sockse, 5);//����������Ϊ10��
	int len = sizeof(SOCKADDR);
	HANDLE hThread;//windows���߳�
	DWORD  threadId;
	while (1) {
		sockcl = accept(sockse, (struct sockaddr*)&cl_addr, &len);
		hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)sockcl, 0, NULL);
		if (hThread == NULL) {
			cout << "�����ӽ���ʧ�ܡ�" << endl;
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


		cout << sockcl<<"��Ϣ:" << rec_buf << endl;
		cout << "�ѷ��ͱ���ʱ����Ϣ";
		time_t t = time(NULL);
		tm* local = localtime(&t); //תΪ����ʱ��
		strftime(send_buf, 64, "%Y-%m-%d %H:%M:%S", local);
		int send_len = send(sockcl,send_buf, 100, 0);
		break;
	}
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
