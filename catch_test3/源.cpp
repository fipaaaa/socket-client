#include<stdio.h>
#include <string.h>
#include<iostream>
#include<Winsock2.h>
#include <WS2tcpip.h>
#include <mstcpip.h>
#include"headers.h";
#pragma comment(lib,"ws2_32.lib")
;
using namespace std;
#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
void initialization();
int main() {
	initialization();
	int server_port = 1234;
	char LocalName[256];
	hostent* pHost;
	struct sockaddr_in se_addr;//ipv4��Ϣ�ṹ��

	if (gethostname((char*)LocalName, sizeof(LocalName) - 1) != 0)
	{
		cout << "��������ȡʧ��" << endl;
	}
	else
	{
		cout << "��������ȡ�ɹ�" << endl;
	}

	// ��ȡ���� IP ��ַ
	pHost = gethostbyname((char*)LocalName);
	if (pHost == NULL)
	{
		cout << "��ȡIP��ַʧ��" << endl;
	}
	else {
		cout << "��ȡIP��ַ�ɹ�" << endl;
	}

	//struct sockaddr_in cl_addr;
	se_addr.sin_family = AF_INET; /* �����ֽ��� */
	se_addr.sin_port = htons(0); /* short, �����ֽ��� */
	se_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//se_addr.sin_addr=*(in_addr*)pHost->h_addr_list[0];//inet_addr("127.0.0.1");
	cout << inet_ntoa(se_addr.sin_addr) << endl;


	SOCKET sockse;
	sockse = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sockse == INVALID_SOCKET) {//����ԭʼ�׽���
		cout << "ԭʼ�׽��ִ���ʧ��" << endl;
		exit(0);
	}
	else
	{
		cout << "ԭʼ�׽��ִ����ɹ�" << endl;
	}
	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "�׽���ռ��" << endl;
	}
	else {
		cout << "�׽��ֿ���" << endl;
	}
	int a = 1;

	if (setsockopt(sockse, IPPROTO_IP, IP_HDRINCL, (char*)&a, sizeof(a)) != 0) {
		//if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "�����׽���ѡ��ʧ��" << endl;
		//exit(0);
	}
	else {
		cout << "�����׽���ѡ��ɹ�������ΪIP" << endl;
	}


	DWORD dwBytesReturned;
	DWORD dwBufferInLen = 1;
	//����������Ϊ����ģʽ�����ǽ�����������
	if (ioctlsocket(sockse, SIO_RCVALL, &dwBufferInLen) == SOCKET_ERROR) {
		cout << "error:" << WSAGetLastError() << endl;
		closesocket(sockse);
		WSACleanup();
		return -1;
	}
	else {
		cout << "����ģʽ����" << endl;
	}
	char buffer[65535];
	IP ip;
	TCP tcp;
	SOCKET sockcl;
	sockaddr_in SenderAddr;
	int nAddrLen = sizeof(SenderAddr);
	while (1) {
		memset(buffer, 0, 65535);
		int rec_b = recvfrom(sockse, buffer, sizeof(buffer), 0, (struct sockaddr*)&SenderAddr, &nAddrLen);
		if (rec_b > 0) {
			ip = *(IP*)buffer;
			tcp = *(TCP*)(buffer + 4 * (ip.ihl & 0x0F)); //ip.HdrLen  & 0xF �õ�IPͷ���ȣ����������32λ�ֵĸ���
				printf("Applications++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("data:%s\r\n", buffer);
				printf("Transportation++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("src port�� %d\r\n", ntohs(tcp.tcp_sport));
				printf("dest port��%d\r\n", ntohs(tcp.tcp_dport));
				printf("Network+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("src IP�� %s\r\n", inet_ntoa(*(in_addr*)&ip.srcaddr));
				printf("dest IP: %s\r\n\n\n\n", inet_ntoa(*(in_addr*)&ip.dstaddr));

			//printf("%d", rec_b);
			//for (int i = 0; i < rec_b; i++) {
				//printf("%c", buffer[i]);
			//}
		}
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

