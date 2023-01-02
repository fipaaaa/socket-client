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
    unsigned char ihl : 4;     //�ײ�����   
    unsigned char version : 4, //�汾    
#else   
    unsigned char version : 4; //�汾
    unsigned char ihl : 4;     //�ײ�����   
#endif   
    unsigned char tos;       //��������   
    unsigned short tot_len;  //�ܳ���   
    unsigned short id;       //��־   
    unsigned short frag_off; //��Ƭƫ��   
    unsigned char ttl;       //����ʱ��   
    unsigned char protocol;  //Э��   
    unsigned short chk_sum;  //�����   
    struct in_addr srcaddr;  //ԴIP��ַ   
    struct in_addr dstaddr;  //Ŀ��IP��ַ   
}IP;

#pragma once
