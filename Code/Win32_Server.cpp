/* ======================================================
SFile: S
SDate: S
SRevision: S
SCreator: Gergely Zara S
SNotice: S
======================================================*/
#pragma comment(lib, "ws2_32.lib")
#include <WinSock.h>
#include <iostream>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

SOCKET Connections[32];
int ConnectionCounter = 0;
int Counter;

char buffer[6];	//6 volt
char bufferWas[6];
char buffer2[6];//lhet a bufferbe nem �gy f�r bele...
/*******************************/
/*******************************/
///
///A lagol�s furcsa, mert egyszer van lag m�skor meg nincs.....!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///
void ClientHandlerThread(int index)//sent to clients all the time.
//EBB�L ANNYI FUT AH�NY KLIENS VAN....
{//index is you who will be sending and receiving
	//char buffer[6];	//6 volt
	//char buffer2[6];//lhet a bufferbe nem �gy f�r bele...
	// ebbe a bufferbe pont blef�r egy j�t�kos adata
	//real64 GlobalLocalPlayerParts;

	while (true)
	{
	//a kliens oldalon az egyes SOCKET Connectionje
	//nincs meg a memoriaban............
	if (index == 0)
	{
		recv(Connections[0], buffer, sizeof(buffer), NULL);
		if(buffer > 0)
		{
		send(Connections[1], buffer, sizeof(buffer), NULL);
		}
		//!!!!
 		//closesocket(ClientSocket);
            	//WSACleanup();
		for(int n = 0; n < 6; n++)
		{
			bufferWas[n] = buffer[n];
		}
	}
		
	}

	//while (true)
	//{
	//	//az a baj h az indexet amit elk�ld az valamit elbasz
	//	//az index hazn�lata n�lk�l csak a CLIENT2 tud k�ldeni
	//	for (int i = 0; i < ConnectionCounter; i++)
	//	{
	//		if (i != index)//!=
	//			continue;
	//		recv(Connections[i], buffer, sizeof(buffer), NULL);
	//		//if (buffer == buffer2)
	//		//{
	//		//	break;
	//		//}
	//		////nem szabad sz�tspammolni az agy�t mert nem ceresse
	//		////de ez ne mbizti h megoldotta a bugot, mert ha mindkett� mozog
	//		////lehet visszaj�n majd a bug, 
	//		//buffer2[0] = buffer[0];
	//		//buffer2[1] = buffer[1];
	//		//buffer2[2] = buffer[2];
	//		//buffer2[3] = buffer[3];
	//		//buffer2[4] = buffer[4];
	//		//buffer2[5] = buffer[5];
	//		//buffer2[6] = buffer[6];
	//		for (int i = 0; i < ConnectionCounter; i++)
	//		{
	//			if (i == index)//ezt gondold �t ==
	//				continue;
	//			//the server should send some kind of player identifier
	//			send(Connections[i], buffer, sizeof(buffer), NULL);

	//			//std::cout << buffer << std::endl;
	//		}
	//	}
	//}
}

int main()
{
	/*******************************************************************************************/
	WSAData WsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &WsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
	/*******************************************************************************************/
	SOCKADDR_IN address;
	int addresslength = sizeof(address);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");//broadcast locally
	address.sin_port = htons(1111);
	address.sin_family = AF_INET;//use ip

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&address, sizeof(address));
	listen(sListen, SOMAXCONN);
	/******************************************************ON NEW CLIENT CONNECTION*/
	SOCKET NewConnection;
	//a connectionket memoriaban lehetne t�rolni
	for (int i = 0; i < 3; i++)
		//ez annyit csin�l h nyolcszor tudunk visszamenni a while loopba
	{
		NewConnection = accept(sListen, (SOCKADDR*)&address, &addresslength);
		if (NewConnection == 0)
		{
			std::cout << "Client not Connected!" << std::endl;
		}
		else
		{
			std::cout << "Client Connected!" << std::endl;
			Connections[i] = NewConnection;
			ConnectionCounter += 1;
			CreateThread
			(0, 0, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), 0, 0);
		}

	}	
	return 0;
}

