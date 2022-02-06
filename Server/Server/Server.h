#pragma once
#include"../../Client/Client/ISocketProvider.h"
#include <vector>
#define BUFF_SIZE 64
using namespace std;
class TcpServer
{
private:
	bool winsockStarted;
	SOCKET client;
	WSADATA WSAData; //Данные 
	SOCKET* server; //Сокеты сервера и клиента
	SOCKADDR_IN serverAddr, clientAddr;
public:
	TcpServer();
	virtual ~TcpServer();
	bool start(const char *port);
	void process();
};

TcpServer::TcpServer()
	: server(),client(), winsockStarted(false)
{
	WSADATA WSAData = { 0 };
	int status = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (status != 0)
		std::cout << "[ERROR]: " << status << " Unable to start Winsock." << std::endl;
	else
		winsockStarted = true;
}

TcpServer::~TcpServer()
{

	if (winsockStarted)
		WSACleanup();
	delete server;
}

bool TcpServer::start(const char *port)
{
	 //Адреса сокетов
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = ISocketProvider::getSocketProvider(); //Создали сервер
	if (*server == INVALID_SOCKET) {
		cout << "Socket creation failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8000);
	if (bind(*server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		cout << "Bind function failed with error: " << WSAGetLastError() << endl;
		return -1;
	}

	if (listen(*server, 0) == SOCKET_ERROR) { 
		cout << "Listen function failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Listening for incoming connections....\n" << endl;
	int clientAddrSize = sizeof(clientAddr); 
	if (( client = accept(*server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
		return true;
	}
	return false;
}

void TcpServer::process() {
	
	size_t size = 64;

	while (true)
	{
		std::string servBuff(size, ' ');
		if (recv(client, (char*)servBuff.data(), servBuff.size(), 0) == SOCKET_ERROR)
		{
			cout << "Cant get msg\n" << WSAGetLastError() << endl;
			return;
		}
		else
		{
			cout << "Get msg\n ";
			for (std::string::iterator it = servBuff.begin(); 
				it != servBuff.end(); ++it)
				cout << *it;
		}
		cout << "\n ";
			char sendbuf[] = "from server hello client.";
		if (send(client, sendbuf, sizeof(sendbuf
			), 0) == SOCKET_ERROR)
		{
			cout << "Cant get msg\n";
		}
	}
}