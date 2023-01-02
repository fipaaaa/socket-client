#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include<iostream>
#include <io.h>
#pragma comment (lib, "ws2_32.lib")  
using namespace std;


#define BUF_SIZE 1024


DWORD WINAPI ThreadFunc(LPVOID);
void initialization();
char* listFiles(const char* dir);
int main() {
    initialization();



    SOCKET sockse = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in se_addr;
    memset(&se_addr, 0, sizeof(se_addr));
    se_addr.sin_family = AF_INET;
    se_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    se_addr.sin_port = htons(1234);
    if (bind(sockse, (struct sockaddr*)&se_addr, sizeof(struct sockaddr)) == SOCKET_ERROR) {
        cout << "�׽���ռ��" << endl;
    }
    else {
        cout << "�׽��ֿ���" << endl;
    }

    listen(sockse, 10);

    SOCKADDR cl_addr;
    SOCKET sockcl;
    int nSize = sizeof(SOCKADDR);

    //ѭ���������ݣ�ֱ���ļ���β


    HANDLE hThread;//windows���߳�
    DWORD  threadId;
    while (1) {
        sockcl = accept(sockse, (SOCKADDR*)&cl_addr, &nSize);
        hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)sockcl, 0, NULL);
        if (hThread == NULL) {
            cout << "�����ӽ���ʧ�ܡ�" << endl;
            break;
        }

        CloseHandle(hThread);
    }



    closesocket(sockcl);
    closesocket(sockse);
    WSACleanup();
    return 0;
}

DWORD WINAPI ThreadFunc(LPVOID cl) {

    char recv_buf1[BUF_SIZE] = "";
    char recv_buf2[BUF_SIZE] = "";
    char send_buf1[BUF_SIZE] = "";  //������
    int nCount;

    SOCKET sockcl = (SOCKET)cl;
    
    if (recv(sockcl, recv_buf2, BUF_SIZE, 0) > 0) {
        cout << "�ͻ��������ļ��б�" << endl;
    }

    char file_search[100] = "C:\\Users\\LongRow\\Desktop\\�����¿μ�\\��������\\FTP\\source\\";
    strcat(file_search, "*.*");        // ��Ҫ������Ŀ¼�����ͨ���
    char* name_list=listFiles(file_search);

    if (send(sockcl, name_list, BUF_SIZE, 0)) {
        cout << "�ɹ������ļ��б�" << endl;
    }
    
    char file_in[100] = "C:\\Users\\LongRow\\Desktop\\�����¿μ�\\��������\\FTP\\source\\";

    if (recv(sockcl, recv_buf1, BUF_SIZE, 0) > 0) {
        cout << "�յ������ļ���" << recv_buf1 << endl;
    }

    //char in_name[100] = "a.pdf";
    //cin >> in_name ;
    //char head[10] = "#";
    char* file_new = strcat(file_in, recv_buf1);
    //char* send_name = strcat(in_name, head);
    //cout << file_new << endl;
    const char* filename = file_new;  //�ļ���
    //cout << filename << endl;
    FILE* fp = fopen(filename, "rb");  //�Զ����Ʒ�ʽ���ļ�
    if (fp == NULL) {
        cout << "�޷����ļ�:" << filename << endl;
        //send(sockcl, filename,BUF_SIZE, 0);
        return 0;
    }
    else {
        cout << "�ļ�" << filename << "��" << endl;
    }

    //int name_len = strlen(send_name);
    cout << "�����ļ�����" << file_new << endl;
    //send(sockcl, send_name, name_len, 0);
    while ((nCount = fread(send_buf1, 1, BUF_SIZE, fp)) > 0) {
        //cout << nCount << endl;
        send(sockcl, send_buf1, nCount, 0);
    }
    shutdown(sockcl, SD_SEND);  //�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��
    //recv(sockcl, buffer, BUF_SIZE, 0);  //�������ȴ��ͻ��˽������
    cout << "�������" << sockcl << endl;
    cout << "�ѷ����ļ�" <<"\n\n" << endl;
    fclose(fp);
}

char* listFiles(const char* dir)
{
    intptr_t handle;
    _finddata_t findData;
    char arr[1000] = "";

    handle = _findfirst(dir, &findData);    // ����Ŀ¼�еĵ�һ���ļ�
    if (handle == -1)
    {
        cout << "�Ҳ����ļ�" << endl;
        return 0;
    }

    do
    {
        if (findData.attrib & _A_SUBDIR && (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0))    // �Ƿ�����Ŀ¼���Ҳ�Ϊ"."��".."
            //cout << "Ŀ¼��" << findData.name << "\t<dir>\n";
            continue;
        else {
            //cout << "�ļ�����" << findData.name << "\t" << endl;
            strcat(arr, findData.name);
            strcat(arr, " ");
            //cout << arr << endl;
        }


    } while (_findnext(handle, &findData) == 0);    // ����Ŀ¼�е���һ���ļ�

    //cout << "Done!\n";
    _findclose(handle);    // �ر��������
    return arr;
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