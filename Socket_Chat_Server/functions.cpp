//#include <iostream>
//#include <WS2tcpip.h>
//#include <winsock.h>
//
//#include <fstream>
//#include <sstream>
//
//using namespace std;
//
//SOCKET serverSocket, Connections[100];
//int counter = 0;
//int port = 15000;
//WSADATA wsaData;
//int wsaerr;
//WORD wVersionRequested = MAKEWORD(2, 2);
//
//
//int setSocket() {
//	// Initialize WSA / load DLL
//	if (wsaerr != 0)
//	{
//		cout << "WinSock dll not found\n";
//		return 0;
//	}
//	else
//	{
//		cout << "WinSock dll found\n";
//		cout << "Status : " << wsaData.szSystemStatus << endl;
//	}
//
//	serverSocket = INVALID_SOCKET;
//	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (serverSocket == INVALID_SOCKET) {
//		cout << "Error at socket(): " << WSAGetLastError() << endl;
//		WSACleanup();
//		return 0;
//	}
//	else
//		cout << "Socket is OK\n";
//
//	// Bind socket
//
//	sockaddr_in service;
//	int sizeOfService = sizeof(service);
//	service.sin_family = AF_INET;
//	InetPton(AF_INET, (PCSTR)("127.0.0.1"), &service.sin_addr.s_addr);
//	service.sin_port = htons(port);
//
//	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
//	{
//		cout << "Bind failed: " << WSAGetLastError() << endl;
//		closesocket(serverSocket);
//		WSACleanup();
//		return 0;
//	}
//	else {
//		cout << "Bind is OK\n";
//
//	}
//	// Listen socket
//
//	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
//		cout << "Listen : Error listening on socket";
//	else {
//		cout << "Listen is OK. Waiting connections...\n";
//		//------------------------------------------------------- test 1 --------------------------
//		fd_set master;
//		FD_ZERO(&master);
//		FD_SET(serverSocket, &master);
//		while (true)
//		{
//			fd_set masterCopy = master;
//			int socketCount = select(0, &masterCopy, nullptr, nullptr, nullptr);
//
//			for (int i = 0; i < socketCount; i++)
//			{
//				SOCKET sock = masterCopy.fd_array[i];
//				if (sock == serverSocket)
//				{
//					//accapt new connection
//					SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
//					//add new connection to client list
//					FD_SET(clientSocket, &master);
//					// welcome messsage
//					string welconeMsg = "You are connected to the server !\n";
//					cout << "Client socket # " << clientSocket << " connected\n";
//					send(clientSocket, welconeMsg.c_str(), welconeMsg.size() + 1, 0);
//				}
//				else
//				{
//					char buf[300];
//					ZeroMemory(buf, 300);
//					//accept new message
//					int bytesIn = recv(sock, buf, 300, 0);
//					if (bytesIn <= 0)
//					{
//						closesocket(sock);
//						FD_CLR(sock, &master);
//					}
//					else
//					{
//						// Send messages to other clients
//						for (int i = 0; i < master.fd_count; i++)
//						{
//							SOCKET outSock = master.fd_array[i];
//							if (outSock != sock && outSock != serverSocket)
//							{
//								ostringstream ss;
//								ss << "Socket #" << sock << " : " << buf << "\r\n";
//								cout << ss.str();
//								string strOut = ss.str();
//								send(outSock, strOut.c_str(), strOut.size() + 1, 0);
//							}
//						}
//					}
//				}
//			}
//		}
//		//----------------------------------------------------------------------------------------
//	}
//
//	// Accept connections
//	//SOCKET acceptSocket;
//	//for (int i = 0; i < 100; i++)
//	//{
//	//	acceptSocket = accept(serverSocket, (SOCKADDR*)&service, &sizeOfService);
//	//	if (acceptSocket == INVALID_SOCKET)
//	//	{
//	//		cout << "Accept failed : " << WSAGetLastError() << endl;
//	//		WSACleanup();
//	//		return -1;
//	//	}
//	//	else {
//	//		cout << "Accepted connection\n";
//	//		Connections[i];
//	//		counter++;
//	//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandler, (LPVOID)(i), NULL, NULL);
//	//	}
//	//}
//	// Recive message from client and sending confirmation
//
//	/*bool exitTrigger = false;
//	while (exitTrigger == false)
//	{
//		char buffer[300];
//		int byteCount = recv(acceptSocket, buffer, 300, 0);
//		if (byteCount > 0)
//			cout << "Message recived: " << buffer << endl;
//		else
//			WSACleanup();
//		if (strcmp(buffer, "quit") == 0)
//			exitTrigger = true;
//		char confirmation[100] = "Message recived";
//		byteCount = send(acceptSocket, confirmation, 100, 0);
//	}*/
//
//	system("pause");
//	WSACleanup();
//
//	return 0;
//}
//
//void reciveMessage() {
//	/*bool exitTrigger = false;
//	while (exitTrigger == false)
//	{
//		char buffer[300];
//		int byteCount = recv(acceptSocket, buffer, 300, 0);
//		if (byteCount > 0)
//			cout << "Message recived: " << buffer << endl;
//		else
//			WSACleanup();
//		if (strcmp(buffer, "quit") == 0)
//			exitTrigger = true;
//		char confirmation[100] = "Message recived";
//		byteCount = send(acceptSocket, confirmation, 100, 0);
//	}*/
//}
//
//void clientHandler(int index) {
//	char message[300];
//	while (true)
//	{
//		recv(Connections[index], message, sizeof(message), NULL);
//		for (int i = 0; i < counter; i++)
//		{
//			/*if (i == index) {
//				continue;
//			}*/
//			send(Connections[i], message, sizeof(message), NULL);
//		}
//	}
//}
//
//void writeToFile(const string& filename, const string& data) {
//	ofstream file(filename);
//	if (file.is_open()) {
//		file << data;
//		file.close();
//		cout << "User added successfully." << endl;
//	}
//	else {
//		cerr << "Unable to open file for writing." << endl;
//	}
//}
//
//string ReadFromFile(const string& filename) {
//	ifstream file(filename);
//	string data;
//	if (file.is_open()) {
//		getline(file, data);
//		file.close();
//		return data;
//	}
//	else {
//		cerr << "Unable to open file for reading." << endl;
//		return "";
//	}
//}