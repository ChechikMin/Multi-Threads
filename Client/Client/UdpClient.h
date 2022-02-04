#pragma once
#include "Info.h"
class UdpClient
{
public:
	UdpClient();
	~UdpClient();

private:
	WSADATA wsaData;
	SOCKET SendRecvSocket;  
	sockaddr_in ServerAddr; 
	int err, maxlen = 512;  
	std::shared_ptr< char* > recvbuf;  
	std::shared_ptr< char* > query;

};

UdpClient::UdpClient()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a SOCKET for connecting to server
	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(8000);

	sendto(SendRecvSocket, *query, strlen(*query), 0, (sockaddr *)&ServerAddr, sizeof(ServerAddr));
	printf("Sent: %s\n", query);

	err = recvfrom(SendRecvSocket, *recvbuf, maxlen, 0, 0, 0);
	if (err > 0) {
		recvbuf = 0;
		printf("Result: %s\n", *recvbuf);
	}
	else {
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(SendRecvSocket);
		WSACleanup();
	}

	closesocket(SendRecvSocket);

}

UdpClient::~UdpClient()
{
}
