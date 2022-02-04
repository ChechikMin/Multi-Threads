#pragma once
#include"../../Client/Client/ISocketProvider.h"
using namespace std;
class TcpServer
{
private:
	bool winsockStarted;
	shared_ptr< SOCKET > sock;
public:
	TcpServer();
	virtual ~TcpServer();
	bool Start(const char *port);
};

TcpServer::TcpServer()
	: sock(), winsockStarted(false)
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
}

bool TcpServer::Start(const char *port)
{
	WSADATA WSAData; //Данные 
	SOCKET *server, *client; //Сокеты сервера и клиента
	SOCKADDR_IN serverAddr, clientAddr; //Адреса сокетов
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

	if (listen(*server, 0) == SOCKET_ERROR) { //Если не удалось получить запрос
		cout << "Listen function failed with error:" << WSAGetLastError() << endl;
		return -1;
	}
	cout << "Listening for incoming connections...." << endl;
	char buffer[1024]; //Создать буфер для данных
	int clientAddrSize = sizeof(clientAddr); //Инициализировать адерс клиента
	if (( accept(*server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
		return true;
	}
	return false;
}
