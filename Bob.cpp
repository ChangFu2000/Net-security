#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#include"DES.h"
#include"DH.h"
#include"main.h"
#pragma comment (lib, "ws2_32.lib") //加载 ws2_32.dll
#define BUF_SIZE 100
int main(){
	WSADATA wsaData;
	WSAStartup( MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr)); //每个字节都用0填充
	sockAddr.sin_family = PF_INET; //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
	sockAddr.sin_port = htons(8888); //端口 
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//进入监听状态
	listen(servSock, 20);
	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	
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
	memset(temp,0,sizeof(temp));
	recv(clntSock, temp, sizeof(temp), 0);
	memcpy(&s1,temp,sizeof(s));
	int p1=s1.p;
	int g1=s1.g;
	int key1=s1.key;
	int r1=ran();
	int hope=0,key2=0;
	printf("请输入想要获取的消息0/1：\n");
	scanf("%d",&hope);
	if(hope==0){
		key2=MoChongFu(g1,r1,p1);
	}else{
		key2=MoChongFu(g1,r1,p1);
		key2=key2*key1;
	//	key2=(key2*key1)%p1;
	}
	s2.key=key2;
	printf("%d\n",key2);
	memset(temp,0,sizeof(temp));
	memcpy(temp,&s2,sizeof(s));
	send(clntSock,temp,sizeof(temp),0);
	//接收加密后的信息M0和M1 
	memset(temp,0,sizeof(temp));
	recv(clntSock, temp, sizeof(temp), 0); 
	memcpy(&message,temp,sizeof(temp));
	printf("%s\n",message.str0); 
	printf("%s\n",message.str1);
	//解密M0/M1信息 
	int k=MoChongFu(key1,r1,p1);
	printf("%d\n",k);
//	if(hope==0){
		result(message.str0,k);
//	}else{
		result(message.str1,k);
//	}
	
	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
	//终止DLL的使用 
	WSACleanup();
	system("pause");
	return 0;
}
