#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<string>
#pragma comment(lib, "ws2_32.lib")
#include<iostream>
using namespace std;
#define BUF_SIZE 1024

void initialization();
int main() {
    initialization();

    FILE* fp = NULL;
    SOCKET sockcl = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in se_addr;
    memset(&se_addr, 0, sizeof(se_addr));
    se_addr.sin_family = PF_INET;
    se_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    se_addr.sin_port = htons(1234);
    SOCKET sockse = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(sockse, (SOCKADDR*)&se_addr, sizeof(SOCKADDR));

    //ѭ���������ݣ�ֱ���ļ��������

    int nCount;
    char symbol[10] = "#!";
    int flag = 0;

    while (1) {
        int opt;//ѡ����
        cout << "1.�鿴�������ļ� 2.�˳�" << endl;
        cin >> opt;
        char send_buf1[BUF_SIZE] = "search";
        char send_buf2[BUF_SIZE] = "";
        char recv_buf1[BUF_SIZE] = "";
        char recv_buf2[BUF_SIZE] = "";
        char recv_buf3[BUF_SIZE] = "";
        
        if (opt == 2) {
            cout << "�ͻ��˽�������" << endl;
        }
        if (opt != 1 && opt != 2) {
            cout << "����������Ч" << endl;
        }
        if (opt == 1) {
            int send_len;
            if (send_len = send(sockse, send_buf1, BUF_SIZE, 0)) {
                cout << "��ѯ��Ϣ�ѷ���" << endl;
            }
            else {
                cout << "��ѯ��Ϣ����ʧ��" << endl;
                break;
            }
            if ((nCount = recv(sockse, recv_buf1, BUF_SIZE, 0)) > 0) {
                cout <<"�������ļ��У�" << recv_buf1 << endl;
            }
        }
        char* file[30];
        char* token;
        int num = 0;
        token = strtok(recv_buf1, " ");
        while (token != NULL) {
            
            //printf("%s\n", token);
            file[num] = token;
            token = strtok(NULL, " ");
            num++;
        }
        //for (int i = 0; i < num; i++) {
         //   cout <<"�ļ���" << file[i] << endl;
        //}
        bool find = 0;
        while (1) {
            cout << "������Ҫ��ȡ���ļ�:" ;
            cin >> send_buf2;
            for (int i = 0; i < num; i++) {
                //cout <<"�ļ���" << file[i] << endl;
                if (strcmp(send_buf2, file[i]) == 0) {
                    cout << "�������д��ڸ��ļ�" << endl;
                    find = 1;
                }

            }
            if (find == 1) {
                int send_len = send(sockse, send_buf2, BUF_SIZE, 0);
                cout << "�����ļ���������" << endl;
                break;
            }
            else if (find == 0) {
                cout << "��������ļ����������ڷ�������" << endl;
                cout << "�����������ļ���" << endl;
            }
        }
        //if ((nCount = recv(sockse, recv_buf3, BUF_SIZE, 0)) > 0) {
         //   cout <<  recv_buf3 << endl;
        //}


        fp = fopen(send_buf2, "wb");  //�Զ����Ʒ�ʽ�򿪣��������ļ�
        if (fp == NULL) {
            cout << "�޷��򿪸��ļ�" << endl;
            exit(0);
        }
        //if ((nCount = recv(sockse, buffer, BUF_SIZE, 0)) > 0){
        //    cout << buffer << endl;
//}
        while ((nCount = recv(sockse, recv_buf2, BUF_SIZE, 0)) > 0) {
            //cout << buffer << endl;
            //cout << nCount << endl;
            //cout << buffer[0] << endl;



            //if (nCount == 2) {
            //    cout << nCount << endl;
            //    flag = 1;
            //    break;
            //}


                //cout << "������ļ�������Ϊ" << buffer << endl;
                //cout << nCount << endl;
            fwrite(recv_buf2, BUF_SIZE, 1, fp);
            //cout << "�ļ��Ѵ���" << send_buf << endl;

            //�ļ�������Ϻ�ֱ�ӹر��׽��֣��������shutdown()

        //fwrite(buffer, nCount, 1, fp);
        }
        cout << "�ļ��Ѵ���" << send_buf2 << endl;
        fclose(fp);
        //if (nCount == 0) {
        //    flag = 1;
        //    cout << nCount << endl;
        //}
        /*
        if (flag == 0) {
            cout << "�ļ��Ѵ���" << send_buf2 << endl;
            fclose(fp);
        }
        else {
            cout << "ɾ�������ļ�" << endl;
            fclose(fp);
            remove(send_buf2);
        }
        */
        break;
    }

    closesocket(sockcl); 
    closesocket(sockse);
    WSACleanup();

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