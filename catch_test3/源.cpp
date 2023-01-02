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
	struct sockaddr_in se_addr;//ipv4信息结构体

	if (gethostname((char*)LocalName, sizeof(LocalName) - 1) != 0)
	{
		cout << "主机名获取失败" << endl;
	}
	else
	{
		cout << "主机名获取成功" << endl;
	}

	// 获取本地 IP 地址
	pHost = gethostbyname((char*)LocalName);
	if (pHost == NULL)
	{
		cout << "获取IP地址失败" << endl;
	}
	else {
		cout << "获取IP地址成功" << endl;
	}

	//struct sockaddr_in cl_addr;
	se_addr.sin_family = AF_INET; /* 主机字节序 */
	se_addr.sin_port = htons(0); /* short, 网络字节序 */
	se_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//se_addr.sin_addr=*(in_addr*)pHost->h_addr_list[0];//inet_addr("127.0.0.1");
	cout << inet_ntoa(se_addr.sin_addr) << endl;


	SOCKET sockse;
	sockse = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (sockse == INVALID_SOCKET) {//创建原始套接字
		cout << "原始套接字创建失败" << endl;
		exit(0);
	}
	else
	{
		cout << "原始套接字创建成功" << endl;
	}
	if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "套接字占用" << endl;
	}
	else {
		cout << "套接字可用" << endl;
	}
	int a = 1;

	if (setsockopt(sockse, IPPROTO_IP, IP_HDRINCL, (char*)&a, sizeof(a)) != 0) {
		//if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		cout << "设置套接字选项失败" << endl;
		//exit(0);
	}
	else {
		cout << "设置套接字选项成功，类型为IP" << endl;
	}


	DWORD dwBytesReturned;
	DWORD dwBufferInLen = 1;
	//将网卡设置为混听模式，就是接收所有数据
	if (ioctlsocket(sockse, SIO_RCVALL, &dwBufferInLen) == SOCKET_ERROR) {
		cout << "error:" << WSAGetLastError() << endl;
		closesocket(sockse);
		WSACleanup();
		return -1;
	}
	else {
		cout << "混杂模式开启" << endl;
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
			tcp = *(TCP*)(buffer + 4 * (ip.ihl & 0x0F)); //ip.HdrLen  & 0xF 得到IP头长度，这个长度是32位字的个数
				printf("Applications++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("data:%s\r\n", buffer);
				printf("Transportation++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("src port： %d\r\n", ntohs(tcp.tcp_sport));
				printf("dest port：%d\r\n", ntohs(tcp.tcp_dport));
				printf("Network+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("src IP： %s\r\n", inet_ntoa(*(in_addr*)&ip.srcaddr));
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

