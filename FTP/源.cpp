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
        cout << "套接字占用" << endl;
    }
    else {
        cout << "套接字可用" << endl;
    }

    listen(sockse, 10);

    SOCKADDR cl_addr;
    SOCKET sockcl;
    int nSize = sizeof(SOCKADDR);

    //循环发送数据，直到文件结尾


    HANDLE hThread;//windows多线程
    DWORD  threadId;
    while (1) {
        sockcl = accept(sockse, (SOCKADDR*)&cl_addr, &nSize);
        hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID)sockcl, 0, NULL);
        if (hThread == NULL) {
            cout << "创建子进程失败。" << endl;
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
    char send_buf1[BUF_SIZE] = "";  //缓冲区
    int nCount;

    SOCKET sockcl = (SOCKET)cl;
    
    if (recv(sockcl, recv_buf2, BUF_SIZE, 0) > 0) {
        cout << "客户端请求文件列表" << endl;
    }

    char file_search[100] = "C:\\Users\\LongRow\\Desktop\\大三下课件\\计网课设\\FTP\\source\\";
    strcat(file_search, "*.*");        // 在要遍历的目录后加上通配符
    char* name_list=listFiles(file_search);

    if (send(sockcl, name_list, BUF_SIZE, 0)) {
        cout << "成功发送文件列表" << endl;
    }
    
    char file_in[100] = "C:\\Users\\LongRow\\Desktop\\大三下课件\\计网课设\\FTP\\source\\";

    if (recv(sockcl, recv_buf1, BUF_SIZE, 0) > 0) {
        cout << "收到请求文件：" << recv_buf1 << endl;
    }

    //char in_name[100] = "a.pdf";
    //cin >> in_name ;
    //char head[10] = "#";
    char* file_new = strcat(file_in, recv_buf1);
    //char* send_name = strcat(in_name, head);
    //cout << file_new << endl;
    const char* filename = file_new;  //文件名
    //cout << filename << endl;
    FILE* fp = fopen(filename, "rb");  //以二进制方式打开文件
    if (fp == NULL) {
        cout << "无法打开文件:" << filename << endl;
        //send(sockcl, filename,BUF_SIZE, 0);
        return 0;
    }
    else {
        cout << "文件" << filename << "打开" << endl;
    }

    //int name_len = strlen(send_name);
    cout << "发送文件名：" << file_new << endl;
    //send(sockcl, send_name, name_len, 0);
    while ((nCount = fread(send_buf1, 1, BUF_SIZE, fp)) > 0) {
        //cout << nCount << endl;
        send(sockcl, send_buf1, nCount, 0);
    }
    shutdown(sockcl, SD_SEND);  //文件读取完毕，断开输出流，向客户端发送FIN包
    //recv(sockcl, buffer, BUF_SIZE, 0);  //阻塞，等待客户端接收完毕
    cout << "传输对象：" << sockcl << endl;
    cout << "已发送文件" <<"\n\n" << endl;
    fclose(fp);
}

char* listFiles(const char* dir)
{
    intptr_t handle;
    _finddata_t findData;
    char arr[1000] = "";

    handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
    if (handle == -1)
    {
        cout << "找不到文件" << endl;
        return 0;
    }

    do
    {
        if (findData.attrib & _A_SUBDIR && (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0))    // 是否是子目录并且不为"."或".."
            //cout << "目录：" << findData.name << "\t<dir>\n";
            continue;
        else {
            //cout << "文件名：" << findData.name << "\t" << endl;
            strcat(arr, findData.name);
            strcat(arr, " ");
            //cout << arr << endl;
        }


    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

    //cout << "Done!\n";
    _findclose(handle);    // 关闭搜索句柄
    return arr;
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