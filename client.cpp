#include <iostream>
#include <string>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

// client
void main()
{
	string ipAddress = "127.0.0.1";				//IP of the server
	int port = 54000;							// Listening port # on the server

	// initialize winsock
	WSADATA wsDATA;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &wsDATA);

	// validate winsock
	if (wsResult != 0)
	{
		cerr << "Cannot start the win sock" << wsResult << endl;
		return;										// IF NO WINSOCK, QUIT
	}

	// create a socket
	SOCKET sock = socket(
		AF_INET,									//intenet version 4  ----- if needed version 6 then ->AF_INET6
		SOCK_STREAM,
		0);

	// validate socket 
	if (sock == INVALID_SOCKET)
	{
		cerr << "Cannot create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// filling hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// connect to the server 
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	//validate connection
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect :" << WSAGetLastError << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do {
		//prompt to user for input
		cout << ">";
		getline(cin, userInput);

		if (userInput.size() > 0) 
		{

			//send th text
			int sendResult = send(
				sock, 
				userInput.c_str(), 
				userInput.size() + 1, 
				0
			);

			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				int bytesReceived_NO_OF_CHARS = recv(sock, buf, 4096, 0);
				if (bytesReceived_NO_OF_CHARS > 0) 
				{
					cout << "SERVER> " << string(
						buf, 
						0, 
						bytesReceived_NO_OF_CHARS
					) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// close client socket
	closesocket(sock);
	// cleanup winsock
	WSACleanup();

}