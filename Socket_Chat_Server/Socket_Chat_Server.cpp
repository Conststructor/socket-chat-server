#include <iostream>
#include <WS2tcpip.h>
#include <winsock.h>

#include <fstream>
#include <sstream>

#include<map>
#include "User.h"
//#include<vector>

//#include "functions.cpp"

using namespace std;

int setSocket();
void clientHandler(int index);
void writeToFile(const string& data);
bool LoginDataComparsion(string buffer);
bool LoginDataComparsion(string buffer, vector<User> &active_user_list);
bool LoginDataComparsion(string buffer, SOCKET Socket);
bool RegistrationDataComparsion(string buffer);

SOCKET serverSocket, Connections[100];
int counter = 0;
int port = 15000;
WSADATA wsaData;
int wsaerr;
WORD wVersionRequested = MAKEWORD(2, 2);

map<SOCKET, string> active_user_list;
map<SOCKET, User> Active_User_List;
User user;

string filename = "Users.txt";

int main(int argc, char* argv[])
{
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	//// Initialize WSA / load DLL

	setSocket();
}


int setSocket() {
	// Initialize WSA / load DLL *************************************************
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

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else
		cout << "Socket is OK\n";

	// Bind socket ****************************************************************

	sockaddr_in service;
	int sizeOfService = sizeof(service);
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
	else {
		cout << "Bind is OK\n";

	}

	// Listen socket ***************************************************************

	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
		cout << "Listen : Error listening on socket";
	else {
		cout << "Listen is OK. Waiting connections...\n";

		//------------------------------------------------------- test 1 --------------------------
		// define set of sockets
		
		fd_set master;
		FD_ZERO(&master);
		FD_SET(serverSocket, &master);
		char buf[300];
		
		while (true)
		{
			fd_set masterCopy = master;
			int socketCount = select(0, &masterCopy, nullptr, nullptr, nullptr);
			for (int i = 0; i < socketCount; i++)
			{
				SOCKET sock = masterCopy.fd_array[i];
				if (sock == serverSocket)
				{
					cout << "Incoming new connection...\n";
					//accapt new connection
					SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
					cout << "Client socket # " << clientSocket << " accepted...\n";

					//add new connection to client list
					FD_SET(clientSocket, &master);

					// ================================== checking client login data ===================================
					ZeroMemory(buf, 300);
					int bytesIn = recv(clientSocket, buf, sizeof(buf), 0);
					cout << "Authorization message : \"" << buf << "\"\n";

					istringstream b((string)buf);
					string connection_request;
					getline(b, connection_request, ':');
					//----------------------------------- client try to login ------------------------------------------
					if (connection_request == "connetion_request")
					{
						cout << "Login check enabled...\n";
						if (bytesIn == 0)
						{
							closesocket(sock);
							FD_CLR(sock, &master);
							cout << "Client socket # " << clientSocket << " disconnected\n";
							continue;
						}
						else
						{
							string strBuf = string(buf);
							if (LoginDataComparsion(strBuf, clientSocket) == true)// char arr [300] == string?
							{
								// welcome messsage
								string welconeMsg = "You are connected to the server !\n";
								cout << "Client socket # " << clientSocket << " connected\n";
								send(clientSocket, welconeMsg.c_str(), welconeMsg.size() + 1, 0);
								cout << "Login check success\n";
							}
							else
							{
								closesocket(sock);
								FD_CLR(sock, &master);
								cout << "Socket # " << clientSocket << " wrong login -> disconnected\n";
							}
						}
					}
					//----------------------------------- client try to register ---------------------------------------
					if (connection_request == "registration_request")
					{
						cout << "Registration enabled...\n";

						if (bytesIn == 0)
						{
							closesocket(sock);
							FD_CLR(sock, &master);
							cout << "Client (registration) socket # " << clientSocket << " disconnected\n";
							continue;
						}
						else
						{
							string strBuf = string(buf);
							if (RegistrationDataComparsion(strBuf) == true)// char arr [300] == string?
							{
								// welcome messsage
								string welconeMsg = "You are successfully register !\n";
								cout << "Client socket # " << clientSocket << " connected\n";
								send(clientSocket, welconeMsg.c_str(), welconeMsg.size() + 1, 0);
								cout << "User register success\n";
								continue;
							}
							else
							{
								string denyMsg = "Username alredy exsist";
								send(clientSocket, denyMsg.c_str(), denyMsg.size() + 1, 0);

								closesocket(sock);
								FD_CLR(sock, &master);
								cout << "Socket # " << clientSocket << " wrong register data -> disconnected\n";
							}
						}

					}
					if (connection_request != "connetion_request" || connection_request == "registration_request")
					{
						cout << "Login check skip\n";
						continue;
					}
					//##########################################################################
				}
				// ======================== retranslate messages to connected clients =============================
				else
				{
					cout << "Retranclate messages...\n";
					//char buf[300];
					ZeroMemory(buf, 300);
					//accept new message *********************************************************
					int bytesIn = recv(sock, buf, sizeof(buf), 0);

					if (bytesIn <= 0)
					{
						closesocket(sock);
						FD_CLR(sock, &master);
						cout << "Error recieving from socket # " << sock << " server error\n";
					}
					else
					{
						// Send messages to other clients ****************************************
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (/*outSock != sock &&*/ outSock != serverSocket)
							{
								ostringstream ss;
								auto IT = Active_User_List.find(sock);

								//------------------------------------------------- map variant with User obj ---------------
								
								/*if (IT->second.get_name() == user.get_name())
								{
									ss << "Socket #" << IT->first << " Name: " << IT->second.get_name() << " ---> " << buf << "\r\n";
								}*/

								if (IT->second.get_name() == user.get_name())
								{
									ss << "SOCKET_#" << IT->first << "ID_#"<< IT->second.get_id()<< "GROUP_#"<<IT->second.get_room()->first <<"GROUP_NAME: "<<IT->second.get_room()->second << "Name: " << IT->second.get_name() << " ---> " << buf << "\r\n";
								}

								//###################################################################
								
								//WORKING WARIANT

								//auto it = active_user_list.find(sock);
								//ss << "Socket #" << it->first << " Name: " << it->second << " ---> " << buf << "\r\n";

								cout << ss.str();
								string strOut = ss.str();
								send(outSock, strOut.c_str(), strOut.size() + 1, 0);
								cout << "Retranslate from socket # " << sock << " to ---> # " << outSock << endl;
							}
						}
					}
				}
			}
		}
		//----------------------------------------------------------------------------------------
	}

	// Accept connections
	//SOCKET acceptSocket;
	//for (int i = 0; i < 100; i++)
	//{
	//	acceptSocket = accept(serverSocket, (SOCKADDR*)&service, &sizeOfService);
	//	if (acceptSocket == INVALID_SOCKET)
	//	{
	//		cout << "Accept failed : " << WSAGetLastError() << endl;
	//		WSACleanup();
	//		return -1;
	//	}
	//	else {
	//		cout << "Accepted connection\n";
	//		Connections[i];
	//		counter++;
	//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientHandler, (LPVOID)(i), NULL, NULL);
	//	}
	//}
	// Recive message from client and sending confirmation

	/*bool exitTrigger = false;
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
	}*/

	system("pause");
	WSACleanup();

	return 0;
}

void clientHandler(int index) {
	char message[300];
	while (true)
	{
		recv(Connections[index], message, sizeof(message), NULL);
		for (int i = 0; i < counter; i++)
		{
			/*if (i == index) {
				continue;
			}*/
			send(Connections[i], message, sizeof(message), NULL);
		}
	}
}

void writeToFile(const string& data) {
	ofstream file(filename, ofstream::app);
	if (file.is_open()) {
		file << data;
		file << "\n";
		file.close();
		cout << "User added successfully." << endl;
	}
	else {
		cerr << "Unable to open file for writing." << endl;
	}
}

bool LoginDataComparsion(string buffer) {

	string  not_used, client_username, client_password;
	istringstream client_login_data(buffer);
	getline(client_login_data, not_used, ':');
	getline(client_login_data, client_username, ':');
	getline(client_login_data, client_password);


	ifstream file(filename);
	string data;
	if (file.is_open()) {
		//getline(file, data);

		//---------------------- matching user data ---------------------
		while (getline(file, data))
		{
			istringstream line(data);
			string username, password;
			if (getline(line, username, ':') && getline(line, password))
			{
				cout << "User name : " << username << " | pasword : " << password << endl;
				if (username == client_username && password == client_password)
				{
					cout << "Match ! Welcome " << username << endl;
					return true;
				}
			}
		}

		//###############################################################

		file.close();
		return false;
	}
	else {
		cerr << "Unable to open file for reading." << endl;
		return false;
	}
}

bool LoginDataComparsion(string buffer, vector <User> &active_user_list) {

	string  not_used, client_username, client_password;
	istringstream client_login_data(buffer);
	getline(client_login_data, not_used, ':');
	getline(client_login_data, client_username, ':');
	getline(client_login_data, client_password);


	ifstream file(filename);
	string data;
	if (file.is_open()) {
		//---------------------- matching user data ---------------------
		int i = 0;
		while (getline(file, data))
		{
			istringstream line(data);
			string username, password;
			if (getline(line, username, ':') && getline(line, password))
			{
				cout << "User name : " << username << " | pasword : " << password << endl;
				if (username == client_username && password == client_password)
				{
					cout << "Match ! Welcome " << username << endl;
					
					active_user_list.push_back(User(i, username, password));

					//active_user_list[i]->set_id(i);
					//active_user_list[i]->set_name(username);
					//active_user_list[i]->set_room_num(0);

					return true;
				}
				i++;
			}
		}

		//###############################################################

		file.close();
		return false;
	}
	else {
		cerr << "Unable to open file for reading." << endl;
		return false;
	}
}

bool LoginDataComparsion(string buffer, SOCKET Socket) {

	string  not_used, client_username, client_password;
	istringstream client_login_data(buffer);
	getline(client_login_data, not_used, ':');
	getline(client_login_data, client_username, ':');
	getline(client_login_data, client_password);


	ifstream file(filename);
	string data;
	if (file.is_open()) {
		//---------------------- matching user data ---------------------
		int i = 0;
		while (getline(file, data))
		{
			istringstream line(data);
			string username, password;
			if (getline(line, username, ':') && getline(line, password))
			{
				cout << "User name : " << username << " | pasword : " << password << endl;
				if (username == client_username && password == client_password)
				{
					cout << "Match ! Welcome " << username << endl;

					//active_user_list.emplace(Socket, username);
					user.set_id(i);
					user.set_name(username);
					user.set_pass(password);
					user.set_room(0, "Main");
					Active_User_List.emplace(Socket, user);

					return true;
				}
				i++;
			}
		}
		//###############################################################
		file.close();
		return false;
	}
	else {
		cerr << "Unable to open file for reading." << endl;
		return false;
	}
}

bool RegistrationDataComparsion(string buffer) {

	string  not_used, client_username, client_password;
	istringstream client_login_data(buffer);
	getline(client_login_data, not_used, ':');
	getline(client_login_data, client_username, ':');
	getline(client_login_data, client_password);


	ifstream file(filename);
	string data;
	if (file.is_open()) {
		//getline(file, data);

		//---------------------- matching user data ---------------------

		//client_username = "test", client_password = "pass";

		int i = 0;

		while (getline(file, data))
		{
			istringstream line(data);
			string username, password;
			if (getline(line, username, ':'))
			{
				cout << "User name : " << username << endl;
				if (username == client_username)
				{
					cout << username << "Match ! Username already exsist\n";
					return false;
				}
				i++;
			}
		}
		file.close();

		// ------------------------ write new user data -------------------
		ofstream writeInFile(filename, ofstream::app);
		if (writeInFile.is_open())
		{
			writeInFile << client_username << ":" << client_password << endl;
			writeInFile.close();
			cout << "User " << client_username << " added successfully." << endl;

			User user(i,client_username,client_password);

			return true;
		}
		//###############################################################
	}
	else {
		cerr << "Unable to open file for reading." << endl;
		return false;
	}
}