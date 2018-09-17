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
#define DEFAULT_PORT "2017"

int __cdecl main(int argc, char* argv[])
{
	WSADATA wsaData;
	int iResult;

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;	

	unsigned int key;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	if (argc < 2)
	{
		std::cerr << "usage: " << argv[0] << " key" << std::endl;
		exit(1);
	}

	key = atoi(argv[1]);

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed with error: " <<  iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        std::cerr << "getaddrinfo failed with error: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
    	std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
    	freeaddrinfo(result);
    	closesocket(listenSocket);
    	WSACleanup();
    	return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
    	std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
    	closesocket(listenSocket);
    	WSACleanup();
    	return 1;
    }

    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
    	std::cerr << "accept failed with error " << WSAGetLastError() << std::endl;
    	closesocket(listenSocket);
    	WSACleanup();
    	return 1;
    }

    std::ofstream ofile;
    ofile.open("file.txt", std::ios::binary | std::ios::out);
    Toydes t = Toydes();
    unsigned char tmp;
    do
    {
    	iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
    	if (iResult > 0)
    	{
			tmp = t.decryptByte(recvbuf[0], key);
			ofile.write(reinterpret_cast<char*>(&tmp), 1);
    	}
    	else if (iResult < 0)
    	{
    		std::cerr << "recv failed with error " << WSAGetLastError() << std::endl;
    		closesocket(clientSocket);
    		WSACleanup();
    		return 1;
    	}
    }
    while (iResult > 0);

    ofile.close();

    iResult = shutdown(clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
    	std::cerr << "shutdown failed with error " << WSAGetLastError() << std::endl;
    	closesocket(clientSocket);
    	WSACleanup();
    	return 1;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}