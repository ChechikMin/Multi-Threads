#pragma once
#include "ISocketProvider.h"
#include "FileWorker.h"
#include "Msg.h"
#include <condition_variable>
#include <thread>
#include <chrono>

std::condition_variable cv;
std::mutex cv_m;

class TcpClient {
public:
	TcpClient();
	void read();
	void write();
	void exec() {
		
		WSADATA WSAData;
		SOCKADDR_IN addr;
		WSAStartup(MAKEWORD(2, 0), &WSAData);
		if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
			return ;
		}

		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //коннект к серверу
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8000); //порт
		while (connect(m_socket, (SOCKADDR*)&addr, sizeof(addr)) < 0) {
			cout << "Connecting.." << endl;
			Sleep(3000);
		}

		cout << "Connected to server!" << endl;

	};
private:
	SOCKET m_socket;
	SOCKADDR_IN *m_wData = nullptr;
	struct sockaddr_in m_addr; // структура с адресом
	struct hostent* m_hostinfo;

};

TcpClient::TcpClient() {


	//m_socket = ISocketProvider::getSocketProvider();

	m_addr.sin_family = AF_INET; // домены Internet
	m_addr.sin_port = htons(8000); // или любой другой порт...
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}

void TcpClient::read() {
	for (;;) {
		std::string a;
		std::unique_lock<std::mutex> lk(cv_m);
		cv.wait(lk);
		FileWorker fileWorker;
		std::getline(file, a);
		Msg msg(a);
		msg.fromStringToDigits();
		int sum = msg.sum();
		std::cerr << "Waiting... \n";
		std::cout << a <<" "<<sum<< "\n";
		std::cerr << "...Sent.\n";

		

			if (send(m_socket, a.c_str(), a.size(), 0) == SOCKET_ERROR)
			{
				cout << "Cant send msg\n";
			}

			char  recvbuf[8];
			if (recv(m_socket, recvbuf, 8, 0) == SOCKET_ERROR)
			{
				cout << "Cant get msg\n" << endl;
			}
			else
				cout << "Get msg\n " << recvbuf << endl;

		}
	
}

void TcpClient::write()
{
	for (;;) {
		std::string a;
		std::getline(std::cin, a);
		Msg msg(a);
		if (msg.check_size() && msg.is_digits())
		{
			msg.fromStringToDigits();
			msg.sorting_str();
			a = msg.prepareForSend();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			{
				file.open("SomeText.txt");
				std::lock_guard<std::mutex> lk(cv_m);
				std::cout << "Write to file ...\n";
				file << a << "\n";
				file.close();
			}
			std::cout << "FileReady ...\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
			cv.notify_one();
		}
	}
}