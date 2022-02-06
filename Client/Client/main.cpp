// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Client.h"


int main()
{
	try
	{
		TcpClient client;
		client.exec();
		std::thread t1(&TcpClient::write, std::ref(client)),
					t2(&TcpClient::read, std::ref(client));
		t1.join();
		t2.detach();
	}

	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}


	return 0;
    
}