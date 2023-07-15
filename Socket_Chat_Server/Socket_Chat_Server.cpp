#include <iostream>
#include <WS2tcpip.h>
#include <winsock.h>


using namespace std;

int main(int argc, char* argv[])
{
	// Initialize WSA / load DLL

	SOCKET serverSocket, acceptSocket;
	int port = 15000;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		cout << "WinSock dll not found\n";
		return 0;
	}
	else
	{
		cout << "WinSock dll found\n";
		cout << "Status : " << wsaData.szSystemStatus << endl;
	}

	// Set up server socket

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else
		cout << "Socket is OK\n";

	// Bind socket

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, (PCSTR)("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "Bind failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else
		cout << "Bind is OK\n";

	// Listen socket

	if (listen(serverSocket, 1) == SOCKET_ERROR)
		cout << "Listen : Error listening on socket";
	else
		cout << "Listen is OK. Waiting connections...\n";

	// Accept connections

	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		cout << "Accept failed : " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	else
		cout << "Accepted connection\n";

	// Recive message from client and sending confirmation
	bool exitTrigger = false;
	while (exitTrigger == false)
	{
		char buffer[300];
		int byteCount = recv(acceptSocket, buffer, 300, 0);
		if (byteCount > 0)
			cout << "Message recived: " << buffer << endl;
		else
			WSACleanup();
		if (strcmp(buffer, "quit") == 0)
			exitTrigger = true;
		char confirmation[100] = "Message recived";
		byteCount = send(acceptSocket, confirmation, 100, 0);
	}


	system("pause");
	WSACleanup();

	return 0;

}
