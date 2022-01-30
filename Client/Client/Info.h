#pragma once
#include <iostream>
#include <sstream>
#include <string>

#define _WIN32_WINNT 0x501

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma warning(disable: 4996)
#pragma comment(lib, "Ws2_32.lib")

using std::cerr;
using std::cout;
using std::endl;