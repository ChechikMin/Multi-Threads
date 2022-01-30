#pragma once
#include "Info.h"

class ISocketProvider
{
public:
	static SOCKET* getSocketProvider();
};

SOCKET* ISocketProvider::getSocketProvider() {
	return new SOCKET(socket(AF_INET, SOCK_STREAM, 0));
}
