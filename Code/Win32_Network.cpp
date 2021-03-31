#include <windows.h>
#include <winsock.h>
#include <winsock2.h>
using namespace std;

bool serverChoice();//PROTOTYP OF serverChoice
bool SERVER = true;

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	WORD wsaVersion = MAKEWORD(1, 1);
	if (WSAStartup(wsaVersion, &wsaData) != 0) {
		return -1;
	}
	SERVER = serverChoice();

	if (SERVER) {
		SOCKET serverPC = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		hostent *serverInfo;
		//TYPE IN AN ADDRESS OF SERVER
		char serverAddrStr[256] = { '\0' };
		if (serverAddrStr[0] == '\0') {
			char standardAddr[4] = { '\127','\0','\0','\1' };
			serverInfo = gethostbyaddr(standardAddr, 4, AF_INET);
		}
		else {
			unsigned int serverAddrBuf = inet_addr(serverAddrStr);
			serverInfo = gethostbyaddr((char*)&serverAddrBuf , 4, AF_INET);
		}

		sockaddr_in serverAddr;
		sockaddr_in clientAddr;
		serverAddr.sin_family = serverInfo->h_addrtype;
		serverAddr.sin_port = htons(12345);
		serverAddr.sin_addr = *((in_addr*)serverInfo->h_addr_list[0]);

		if (bind(serverPC, (sockaddr*)&serverAddr , sizeof(serverAddr)) != 0) {
			WSACleanup();
			return -1;
		}
		if (listen(serverPC, 32) != 0) {
			WSACleanup();
			return -1;
		}

		int addrlen = sizeof(clientAddr);
		SOCKET clientPC = accept(serverPC, (sockaddr*)&clientAddr , &addrlen);
		if (clientPC == INVALID_SOCKET) {
			WSACleanup();
			return -1;
		}

		//COMMUNICATION START
		int sendInt = 0;
		int recvInt = 0;
		recv(clientPC, (char*)&recvInt , 4, 0);
		send(clientPC, (char*)&sendInt , 4, 0);
		//COMMUNICATION END

		closesocket(clientPC);
		closesocket(serverPC);

	}
	else {
		SOCKET serverPC = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		hostent *serverInfo;
		char serverAddrBuf[256] = { '\0' };
		if (serverAddrBuf[0] == '\0') {
			char serverAddress[4] = { '\127','\0','\0','\1' };
			serverInfo = gethostbyaddr(serverAddress, 4, AF_INET);
		}
		else {
			unsigned int serverAddrBin = inet_addr(serverAddrBuf);
			serverInfo = gethostbyaddr((char*)&serverAddrBin , 4, AF_INET);
		}
	
		sockaddr_in serverAddr;
		serverAddr.sin_family = serverInfo->h_addrtype;
		serverAddr.sin_port = htons(12345);
		serverAddr.sin_addr = *((in_addr*)serverInfo->h_addr_list[0]);

		if (connect(serverPC, (sockaddr*)&serverAddr , sizeof(serverAddr)) != 0) {
			WSACleanup();
			return -1;
		}

		//COMMUNICATION START
		int sendInt = 0;
		int recvInt = 0;
		send(serverPC, (char*)&sendInt , 4, 0);
		recv(serverPC, (char*)&recvInt , 4, 0);

		//COMMUNICATION END

		closesocket(serverPC);
	}
	WSACleanup();
	return 0;
}

bool serverChoice() {//IMPLEMENTATION OF serverChoice
choice:
	char srvChoice = '\0';
	switch (srvChoice) {
	case 'C': case 'c':
		return true;
		break;
	case 'J': case 'j':
		return false;
		break;
	default:
		goto choice;
	}
}