struct TCP
{
    unsigned short   tcp_sport;
    unsigned short   tcp_dport;
    unsigned int     tcp_seq;
    unsigned int     tcp_ack;
    unsigned char    tcp_lenres;
    unsigned char    tcp_flag;
    unsigned short   tcp_win;
    unsigned short   tcp_sum;
    unsigned short   tcp_urp;
};

typedef struct _ip_hdr
{
#if LITTLE_ENDIAN   
    unsigned char ihl : 4;     //首部长度   
    unsigned char version : 4, //版本    
#else   
    unsigned char version : 4; //版本
    unsigned char ihl : 4;     //首部长度   
#endif   
    unsigned char tos;       //服务类型   
    unsigned short tot_len;  //总长度   
    unsigned short id;       //标志   
    unsigned short frag_off; //分片偏移   
    unsigned char ttl;       //生存时间   
    unsigned char protocol;  //协议   
    unsigned short chk_sum;  //检验和   
    struct in_addr srcaddr;  //源IP地址   
    struct in_addr dstaddr;  //目的IP地址   
}IP;

#pragma once
