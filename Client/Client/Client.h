#pragma once
#include "ISocketProvider.h"
#include "FileWorker.h"
#include "Msg.h"
#include <condition_variable>
#include <thread>
#include <chrono>

class TcpClient {
public:
	TcpClient();
	void read();
	void write();
	void exec();

private:
	SOCKET m_socket;
	WSADATA WSAData;
	struct sockaddr_in m_addr;
	std::condition_variable cv;
	std::mutex cv_m;

};

TcpClient::TcpClient() {


	m_addr.sin_family = AF_INET; 
	m_addr.sin_port = htons(8000); 
	m_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}

void TcpClient::read() {
	for (;;) {
		std::string userMsg;
		std::string sum;
		{
			std::unique_lock<std::mutex> lk(cv_m);
			cv.wait(lk);

			FileWorker fileWorker;
			std::getline(file, userMsg);
			Msg msg(userMsg);
			msg.fromStringToDigits();
			sum = std::to_string(msg.sum());
			
			std::cerr << "Waiting... \n";
			std::cout << userMsg << " " << sum << "\n";
			std::cerr << "...Sent.\n";
		}
		

			if (send(m_socket, sum.c_str(), sum.size(), 0) == SOCKET_ERROR)
				cout << "Cant send msg\n";

			char  recvbuf[16];
			if (recv(m_socket, recvbuf, 16, 0) == SOCKET_ERROR)
				cout << "Cant get msg\n" << endl;
			else
				cout << "Get msg\n " << recvbuf << endl;

		}
	
}

void TcpClient::write()
{

	for (;;) {
		std::string userMsg;
		std::getline(std::cin, userMsg);
		Msg myMsg(userMsg);
		if (myMsg.check_size() && myMsg.is_digits())
		{
			myMsg.fromStringToDigits();
			myMsg.sorting_str();
			userMsg = myMsg.prepareForSend();

			std::this_thread::sleep_for(std::chrono::seconds(1));
			{
				file.open("SomeText.txt");
				std::lock_guard<std::mutex> lk(cv_m);

				std::cout << "Write to file ...\n";
				file << userMsg << "\n";
				file.close();
			}

			std::cout << "FileReady ...\n";
			std::this_thread::sleep_for(std::chrono::seconds(1));
			cv.notify_one();
		}
		else
			std::cout << "String contains not only digits ...\n";
	}
	
}

void TcpClient::exec() {

	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Socket creation failed with error: " << WSAGetLastError() << endl;
		return;
	}

	while (connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)) < 0) {
		cout << "Connecting.." << endl;
		Sleep(3000);
	}

	cout << "Connected to server!" << endl;
}