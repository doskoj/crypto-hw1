#define _WIN32_WINNT 0x501

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include "toydes.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 1
#define DEFAULT_PORT "2700"
#define DEFAULT_HOST "129.161.250.221"

int main (int argc, char* argv[])
{
	WSADATA wsaData;
    SOCKET connectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char sendbuf[32];
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    unsigned int key;
    char* fname;
    char* server;

	if (argc < 5)
	{
		std::cerr << "usage: " << argv[0] << " server-address port file-name key" << std::endl;
		exit(1);
	}

	char* port = argv[2];
	if (atoi(port) < 0 || atoi(port) > 65535)
	{
		std::cerr << "ERROR: Invalid port number" << std::endl;
		exit(1);
	}

	server = argv[1];
	fname = argv[3];
	key = atoi(argv[4])&0x3ff;

	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(server, port, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    ptr = result;
    connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (connectSocket == INVALID_SOCKET)
    {
    	std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
    	freeaddrinfo(result);
    	WSACleanup();
    	return 1;
    }

    iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
   	{
   		closesocket(connectSocket);
   		connectSocket = INVALID_SOCKET;
   	}

   	if (connectSocket == INVALID_SOCKET)
   	{
   		std::cerr << "Unable to connect to server" << std::endl;
   		WSACleanup();
   		return 1;
   	}

   	std::ifstream infile;
   	infile.open(fname, std::ios::binary | std::ios::in);
   	if (!infile.is_open())
   	{
   		std::cerr << "Unable to open file" << std::endl;
   		WSACleanup();
   		return 1;
   	}

   	Toydes t = Toydes();
   	char tmp;
   	while(!infile.eof())
	{
		infile.read(&tmp, 1);
		if (infile.eof()) break;
		sendbuf[0] = t.encryptByte(tmp, key);
		iResult = send(connectSocket, sendbuf, 1, 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "send failed with error " << WSAGetLastError() << std::endl;
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}
	}

	infile.close();

	iResult = shutdown(connectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		std::cerr << "shutdown failed with error " << WSAGetLastError() << std::endl;
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	closesocket(connectSocket);
	WSACleanup();
	return 0;
}