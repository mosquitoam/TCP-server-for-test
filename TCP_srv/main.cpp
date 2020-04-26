#include <stdio.h>  
#include <winsock2.h>  
#include <iostream>

#pragma comment(lib,"ws2_32.lib")  

int main(int argc, char* argv[])
{
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[50000];

	int flag = 0;
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		std::cout << inet_ntoa(remoteAddr.sin_addr) << std::endl;
		flag = 1;
		break;
	}



	//接收数据  
	/*	int ret = recv(sClient, revData, 50000, 0);
	if (ret > 0)
	{
		revData[ret] = 0x00;
		printf(revData);
	}*/

	double xyz[3] = { 150.1,80.7,0 };
	char out[100];
	memcpy(out, xyz, sizeof(xyz));

	//发送数据  
	int t = 0;
	while (flag == 1) 
	{
		std::cout << "t::" << t << std::endl;
		//const char * sendData = "你好，TCP客户端！\n";
		send(sClient, out, sizeof(xyz), 0);

		t++;

		Sleep(100);
	}

	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}