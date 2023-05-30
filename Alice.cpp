#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include"DES.h"
#include"DH.h"
#include"main.h"
#pragma comment(lib, "ws2_32.lib") //加载 ws2_32.dll
#define BUF_SIZE 100
int main(){
	//初始化DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//向服务器发起请求
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr)); //每个字节都用0填充
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(8888);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//协商g和大素数n 
	char bufSend[BUF_SIZE] = {0}; 
	int r1=ran();
	int p1=p();
	int g1=g(p1);
	int key1=key(g1,r1,p1);
	printf("大素数为：%d\n",p1);
	printf("原根为:%d\n",g1);
	printf("A为:%d\n",key1);

	struct s{
		int p;
		int g;
		int key;
	};
	struct str{
		char str0[maxn];
		char str1[maxn];
	};
	char temp[sizeof(str)];
	struct s s1,s2;
	struct str message;
	s1.p=p1;
	s1.g=g1;
	s1.key=key1;
	memset(temp,0,sizeof(temp));
	memcpy(temp,&s1,sizeof(s));
	
	//将A=g^r1发送给Bob 
	send(sock,temp,sizeof(temp),0);
	memset(temp,0,sizeof(temp));
	recv(sock, temp, sizeof(temp), 0);
	memcpy(&s2,temp,sizeof(s));
	int key2=s2.key;
	printf("%d\n",key2);
	int k0=MoChongFu(key2,r1,p1);
	printf("%d\n",k0); 
	int k1= MoChongFu(key2/key1,r1,p1);
	printf("%d\n",k1);
	//对M0信息进行加密 
	memcpy(message.str0,final(k0),maxn);
//	message.str0=final(k0);
//	printf("%s\n", message.str0);
	result(message.str0,k0);

	//对M1信息进行加密 
	memcpy(message.str1,final(k1),maxn);
//	message.str1=final(k1);
//	printf("%s\n", message.str1);
	result(message.str1,k1);
	
	memset(temp,0,sizeof(temp));
	memcpy(temp,&message,sizeof(str));
	//将加密后的M0和M1发送给Bob 
	send(sock,temp,sizeof(temp),0);
	
	//关闭套接字
	closesocket(sock);
	//终止使用DLL
	WSACleanup();
	system("pause");
	return 0;
}
