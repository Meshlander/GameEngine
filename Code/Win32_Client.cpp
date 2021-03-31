/* ======================================================
SFile: S
SDate: S
SRevision: S
SCreator: Gergely Zara S
SNotice: S
======================================================*/
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <iostream>

SOCKET Connection;

void ClientThread()
{
	char buffer[256];
	while (true)
	{
		recv(Connection, buffer, sizeof(buffer), 0);
		std::cout << buffer << std::endl;
	}
}

int main()
{
	WSAData WsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &WsaData) != 0)
	{
		//MessageBoxA(NULL, "Winsock startup failed", "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN address;
	int addresslength = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//local host
	address.sin_port = htons(1111);
	address.sin_family = AF_INET;//use ip
	//not listen
	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&address, addresslength) != 0)
	{
		MessageBoxA(NULL, "connection failed", "ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}
	std::cout << "Connected!" << std::endl;
	/**********************************************************Variables received*/
	CreateThread
	(0, 0, (LPTHREAD_START_ROUTINE)ClientThread, 0, 0, 0);
	/*********************************************************Variables sent*/
	char buffer[256];
	while (true)
	{
		std::cin.getline(buffer, sizeof(buffer));
		send(Connection, buffer, sizeof(buffer), 0);
		Sleep(10);
//lehet ez a sleep lenne fontos
	}
	/************************************************************************************/
	//system("pause");
	return 0;
}
