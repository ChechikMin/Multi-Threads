#pragma once
#include"../../Client/Client/ISocketProvader.h"
using namespace std;
class Server
{
private:
	bool winsockStarted;
	SOCKET sock;
public:
	Server();
	~Server();
	bool Start(const char *port);
	void Stop();
};

Server::Server()
	: sock(INVALID_SOCKET), winsockStarted(false)
{
	WSADATA WSAData = { 0 };
	int status = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (status != 0)
		std::cout << "[ERROR]: " << status << " Unable to start Winsock." << std::endl;
	else
		winsockStarted = true;
}

Server::~Server()
{
	Stop();

	if (winsockStarted)
		WSACleanup();
}

bool Server::Start(const char *port)
{
	WSADATA WSAData; //������ 
	SOCKET *server, *client; //������ ������� � �������
	SOCKADDR_IN serverAddr, clientAddr; //������ �������
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	server = ISocketProvider::getSocketProvider(); //������� ������
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

	if (listen(*server, 0) == SOCKET_ERROR) { //���� �� ������� �������� ������
		cout << "Listen function failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Listening for incoming connections...." << endl;
	char buffer[1024]; //������� ����� ��� ������
	int clientAddrSize = sizeof(clientAddr); //���������������� ����� �������
	if (( accept(*server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
		return true;
	}
	return false;
}

void Server::Stop()
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}
