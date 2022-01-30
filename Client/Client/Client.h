#pragma once
#include "ISocketProvader.h"

class Client {
public:
	Client();
	void exec() {
		
		WSADATA WSAData;
		SOCKET server;
		SOCKADDR_IN addr;
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
			return ;
		}

		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //коннект к серверу
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8000); //порт
		while (connect(server, (SOCKADDR*)&addr, sizeof(addr)) < 0) {
			cout << "Connecting.." << endl;
			Sleep(3000);
		}

		cout << "Connected to server!" << endl;
		cout << "Now you can use our live chat application. " << " Enter \"exit\" to disconnect" << endl;

	};
private:
	SOCKET *m_socket = nullptr;
	SOCKADDR_IN *m_wData = nullptr;
	struct sockaddr_in m_addr; // структура с адресом
	struct hostent* m_hostinfo;

};

Client::Client() {

	m_socket = ISocketProvider::getSocketProvider();

	m_addr.sin_family = AF_INET; // домены Internet
	m_addr.sin_port = htons(8000); // или любой другой порт...
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}