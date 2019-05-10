#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;
// server
void main()
{
	// initialize winsock
	WSADATA wsDATA;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsDATA);

	// validate winsock
	if (wsok != 0)
	{
		cerr << "Cannot initialize the winsock "<<endl;
		return;													// IF NO WINDSOCK, QUIT

	}

	// create a socket - SOCK_STREAM for TCP, SOCK_DGRAM for udp
	SOCKET listening_socket = socket(
		AF_INET,												// version 4
		SOCK_STREAM,
		0
	);

	// validate socket
	if (listening_socket == INVALID_SOCKET)
	{
		cerr << "can't create a socket" << endl;
		return;
	}


	// bind IP and port to a socket - WE USE GENERALLY STRUCTURE CALLED " HINT STRUCTURE "
	sockaddr_in hint_addr;
	hint_addr.sin_family = AF_INET; 
	
	// (There conecepts called little endian and big endian...etc. Follow them. BAsics PC-Little endian, Networking- Bigendian)
	hint_addr.sin_port = htons(							// (HTONS - Host To Network short
		54000											// port number to bind with socket
	); 

	hint_addr.sin_addr.S_un.S_addr = INADDR_ANY;		// we need to bind with any address
	// could also use inet_pton ...

	// now need to bind that to our port or socket (probably listening socket)

	bind(
		listening_socket,								// socket to bind
		(sockaddr*)&hint_addr,							// structure sockaddr_in binds with referencing
		sizeof(hint_addr)
	);													
	// we have socket, socket bind to the netwrking code, -> now socket can use to reference the networking code
	// can send and receive things all inside that socket -(Simply uses the socket instead of the networking)

	// tell the winsock the socket is for listening - THEN HOST WINDOWS WILL DEDICATE THE PARTICULAR SOCKET FOR ITS OWN TASK
	listen(listening_socket, SOMAXCONN);


	// wait for connection (Meaning starts to listening)
	sockaddr_in hint_client;
	int clientSize = sizeof(hint_client);

	SOCKET client_socket = accept(
		listening_socket, 
		(sockaddr*)&hint_client, 
		&clientSize
	);

	char host[NI_MAXHOST];								// clients' remote name (if available)
	char service[NI_MAXSERV];							// service (i.e. port value) that the client connected on 

	ZeroMemory(host, NI_MAXHOST);						// same as memset (host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo(
		(sockaddr*)&hint_client, 
		sizeof(hint_client), 
		host, 
		NI_MAXHOST, 
		service, 
		NI_MAXSERV, 0) == 0){
		cout << host << "connected on port " << service << endl;
	}else{
		inet_ntop(AF_INET, &hint_client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" << 
			ntohs(hint_client.sin_port) << endl;
	}

	// ONCE HAVE A CONNECTION LAAUNDRY THINGS HAVE TO DO :) . CAN SAY 10 THINGS HAVE TO DO

		/*  1. LISTENING - WAITING TO THE CONNECTION. THEN GETS THE CONNECTION. THEN,
			2. CLOSE LISTENING SOCKET 
			3. WHILE LOOP : ACCEPT & ECHO MESSAGE BACK TO CLIENT
		*/

	// close the socket
	closesocket(listening_socket);


	// while loop
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// wait for client to send data
		int bytesReceived = recv(client_socket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << " Error in recv()." << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client Disconnected." << endl;
			break;
		}

		// echo message back to client
		send(client_socket, buf, bytesReceived + 1, 0);

	}

	// close client socket
	closesocket(client_socket);

	// cleanup winsock
	WSACleanup();
}