#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	// Initialize Winsock
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		std::cout << "Error in WSAStartUp: " << WSAGetLastError() << std::endl;
		return 1;
	}

	// Create TCP Socket
	SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpSocket == INVALID_SOCKET)
	{
		std::cout << "Error in socket creation: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// Create remote address
	addrinfo addrHints;
	addrinfo* addrResult;
	memset(&addrHints, 0, sizeof(addrHints));
	addrHints.ai_family = AF_INET;
	addrHints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo("cs260.meancat.com", "8888", &addrHints, &addrResult) != 0)
	{
		std::cout << "Error in connecting: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		return 1;
	}


	// Connect to remote server
	int result = connect(tcpSocket, addrResult->ai_addr, static_cast<int>(addrResult->ai_addrlen));
	if (result == SOCKET_ERROR) 
	{
		std::cout << "Error in connecting: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		freeaddrinfo(addrResult);
		return 1;
	}

	// Send data
	char* programTitle = argv[0];
	int bytesSent = send(tcpSocket, programTitle, static_cast<int>(strlen(programTitle)), 0);
	if (bytesSent == SOCKET_ERROR)
	{
		std::cout << "Error in sending: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		return 1;
	}

	// Shutdown socket
	shutdown(tcpSocket, SD_SEND);

	// Listen for data with non-blocking io
	char buffer[1600];
	memset(buffer, 0, sizeof(buffer));
	int bytesReceived = 0;
	int totalBytesReceived = 0;
	fd_set readSet;
	timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;  // 100 ms
	while (true)
	{
		std::cout << ". ";
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		FD_ZERO(&readSet);
		FD_SET(tcpSocket, &readSet);

		int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
		if (selectResult == SOCKET_ERROR)
		{
			std::cout << "Error in select: " << WSAGetLastError() << std::endl;
			break;
		}

		if (selectResult == 0)
		{
			// Timeout 
			continue;
		}

		// Data is available, receive it
		bytesReceived = recv(tcpSocket, buffer + totalBytesReceived, sizeof(buffer) - totalBytesReceived, 0);
		if (bytesReceived == 0)
		{
			break;
		}
		else if (bytesReceived == SOCKET_ERROR)
		{
			std::cout << "Error in receiving: " << WSAGetLastError() << std::endl;
			closesocket(tcpSocket);
			WSACleanup();
			return 1;
		}
		totalBytesReceived += bytesReceived;
	}


	std::cout << buffer << std::endl;

	// Close socket and shutdown Winsock
	closesocket(tcpSocket);
	WSACleanup();

	return 0;
}
