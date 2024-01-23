#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	// Initialize Winsock
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != 0)
	{
		std::cout << "Error in WSAStartUp: " << WSAGetLastError() << std::endl;
		return 1;
	}

	// Create UDP Socket
	SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET)
	{
		std::cout << "Error in socket creation: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// Create remote address
	sockaddr_in myAddr;
	memset(myAddr.sin_zero, 0, sizeof(myAddr.sin_zero));
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "34.213.232.150", &myAddr.sin_addr);

	// Send data over socket
	int result = sendto(udpSocket, argv[0], strlen(argv[0]), 0, (struct sockaddr*)&myAddr, sizeof(myAddr));
	if (result == SOCKET_ERROR)
	{
		std::cout << "Error in sendto: " << WSAGetLastError() << std::endl;
		closesocket(udpSocket);
		WSACleanup();
		return 1;
	}

	// Listen for response
	char buffer[1600];
	memset(buffer, 0, sizeof(buffer));
	int bytesReceived = recv(udpSocket, buffer, sizeof(buffer), 0);
	if (bytesReceived < 0)
	{
		std::cout << "Error in recv: " << WSAGetLastError() << std::endl;
	}
	else
	{
		std::cout << buffer << std::endl;
	}

	// Close socket and shutdown Winsock
	closesocket(udpSocket);
	WSACleanup();

	return 0;
}
