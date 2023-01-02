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

    //循环接收数据，直到文件传输完毕

    int nCount;
    char symbol[10] = "#!";
    int flag = 0;

    while (1) {
        int opt;//选择功能
        cout << "1.查看服务器文件 2.退出" << endl;
        cin >> opt;
        char send_buf1[BUF_SIZE] = "search";
        char send_buf2[BUF_SIZE] = "";
        char recv_buf1[BUF_SIZE] = "";
        char recv_buf2[BUF_SIZE] = "";
        char recv_buf3[BUF_SIZE] = "";
        
        if (opt == 2) {
            cout << "客户端结束连接" << endl;
        }
        if (opt != 1 && opt != 2) {
            cout << "输入数字无效" << endl;
        }
        if (opt == 1) {
            int send_len;
            if (send_len = send(sockse, send_buf1, BUF_SIZE, 0)) {
                cout << "查询信息已发送" << endl;
            }
            else {
                cout << "查询信息发送失败" << endl;
                break;
            }
            if ((nCount = recv(sockse, recv_buf1, BUF_SIZE, 0)) > 0) {
                cout <<"服务器文件有：" << recv_buf1 << endl;
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
         //   cout <<"文件：" << file[i] << endl;
        //}
        bool find = 0;
        while (1) {
            cout << "输入需要获取的文件:" ;
            cin >> send_buf2;
            for (int i = 0; i < num; i++) {
                //cout <<"文件：" << file[i] << endl;
                if (strcmp(send_buf2, file[i]) == 0) {
                    cout << "服务器中存在该文件" << endl;
                    find = 1;
                }

            }
            if (find == 1) {
                int send_len = send(sockse, send_buf2, BUF_SIZE, 0);
                cout << "发送文件下载请求" << endl;
                break;
            }
            else if (find == 0) {
                cout << "你输入的文件名不存在于服务器中" << endl;
                cout << "请重新输入文件名" << endl;
            }
        }
        //if ((nCount = recv(sockse, recv_buf3, BUF_SIZE, 0)) > 0) {
         //   cout <<  recv_buf3 << endl;
        //}


        fp = fopen(send_buf2, "wb");  //以二进制方式打开（创建）文件
        if (fp == NULL) {
            cout << "无法打开该文件" << endl;
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


                //cout << "输出到文件的内容为" << buffer << endl;
                //cout << nCount << endl;
            fwrite(recv_buf2, BUF_SIZE, 1, fp);
            //cout << "文件已存入" << send_buf << endl;

            //文件接收完毕后直接关闭套接字，无需调用shutdown()

        //fwrite(buffer, nCount, 1, fp);
        }
        cout << "文件已存入" << send_buf2 << endl;
        fclose(fp);
        //if (nCount == 0) {
        //    flag = 1;
        //    cout << nCount << endl;
        //}
        /*
        if (flag == 0) {
            cout << "文件已存入" << send_buf2 << endl;
            fclose(fp);
        }
        else {
            cout << "删除创建文件" << endl;
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