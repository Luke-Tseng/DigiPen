#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <chrono>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI HandleClient(SOCKET clientSocket)
{
	// Receive HTTP request
	char buffer[2048];
	memset(buffer, 0, sizeof(buffer));

	int totalBytesReceived = 0;
	int bytes = 0;

	while ((bytes = recv(clientSocket, buffer + totalBytesReceived, sizeof(buffer) - totalBytesReceived, 0)) > 0)
	{
		totalBytesReceived += bytes;
	}

	if (bytes < 0)
	{
		std::cout << "Error in receiving data from client: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		return 1;
	}

	// Shutdown socket for receiving
	shutdown(clientSocket, SD_RECEIVE);
	
	// Parse HTTP request
	const char* hostStart = strstr(buffer, "Host: ") + 6;
	const char* hostEnd = strstr(hostStart, "\r\n");
	char hostname[256];
	strncpy_s(hostname, hostStart, hostEnd - hostStart);

	// Resolve IP address of hostname
	struct sockaddr_in serverAddress;
	if (inet_pton(AF_INET, hostname, &serverAddress.sin_addr) != 1)
	{
		// inet_pton failed, use getaddrinfo instead
		addrinfo addrHints;
		addrinfo *addrResult;
		memset(&addrHints, 0, sizeof(addrHints));
		addrHints.ai_family = AF_INET;
		addrHints.ai_socktype = SOCK_STREAM;
		if (getaddrinfo(hostname, NULL, &addrHints, &addrResult) != 0)
		{
			std::cout << "Error in resolving hostname: " << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			return 1;
		}
		serverAddress = *(struct sockaddr_in *)addrResult->ai_addr;
		freeaddrinfo(addrResult);
	}

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(80);

	// Build a socket and connect to server
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error in socket creation: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		return 1;
	}

	if (connect(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		std::cout << "Error in connecting to server: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		return 1;
	}

	// Send HTTP request to server
	if (send(serverSocket, buffer, totalBytesReceived, 0) == SOCKET_ERROR)
	{
		std::cout << "Error in sending to server: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		return 1;
	}

	// Receive HTTP response from server and send to client
	do
	{
		memset(buffer, 0, sizeof(buffer));
		totalBytesReceived = 0;
		bytes = 0;

		while (totalBytesReceived < sizeof(buffer) - 1 && (bytes = recv(serverSocket, buffer + totalBytesReceived, sizeof(buffer) - totalBytesReceived - 1, 0)) > 0)
		{
			totalBytesReceived += bytes;
		}

		if (bytes < 0)
		{
			std::cout << "Error in receiving data from server: " << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			return 1;
		}

		if (send(clientSocket, buffer, totalBytesReceived, 0) == SOCKET_ERROR)
		{
			std::cout << "Error in sending to client: " << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			return 1;
		}
	} while (bytes > 0);

	// Shutdown sockets
	shutdown(clientSocket, SD_SEND);
	shutdown(serverSocket, SD_BOTH);

	// Close sockets
	closesocket(clientSocket);
	closesocket(serverSocket);

	return 0;
}

int main(int argc, char *argv[])
{
	// Parsing argument argv[1] to get port to bind to
	int port = atoi(argv[1]);

	// Check port is valid
	if (port < 0 || port > 65535)
	{
		std::cout << "Invalid port number: " << port << " Port number must be in range of 0-65535" << std::endl;
		exit(1);
	}

	// Initialize winsock
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0)
	{
		std::cout << "Error in WSAStartUp: " << WSAGetLastError() << std::endl;
		return 1;
	}

	// Construct listening address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	// Create TCP socket
	SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (tcpSocket == INVALID_SOCKET)
	{
		std::cout << "Error in socket creation: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// Set listening socket to non-blocking
	u_long mode = 1; // non-blocking mode
	if (ioctlsocket(tcpSocket, FIONBIO, &mode) != 0)
	{
		std::cout << "Error in setting listening socket to non-blocking: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		return 1;
	}

	// Bind listening socket to listening address
	if (bind(tcpSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cout << "Error in binding: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		return 1;
	}

	// Listen on TCP socket
	if (listen(tcpSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Error in listening: " << WSAGetLastError() << std::endl;
		closesocket(tcpSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Listening on port " << port << std::endl;

	// Accept and handle incoming connections
	while (true)
	{
		struct sockaddr_in clientAddress;
		int clientAddressSize = sizeof(clientAddress);
		SOCKET clientSocket = accept(tcpSocket, (sockaddr *)&clientAddress, &clientAddressSize);

		// Call handle client on its own thread
		if (clientSocket != INVALID_SOCKET)
		{
			std::thread(HandleClient, clientSocket).detach();
		}

		// wait 100 ms on current thread
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	closesocket(tcpSocket);
	WSACleanup();
	return 0;
}
