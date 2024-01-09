#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	WSADATA wsaData;
	int Result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (Result != 0)
	{
		cout << "Winsock dll Error : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID_SOCKET : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	//ZeroMemory(&ListenSockAddr, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY; // 보통은 지정 아이피를 해주지만 테스트기 때문에, INADDR_ANY로
	ListenSockAddr.sin_port = htons(40211);

	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "Can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	Result = listen(ListenSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		cout << "Can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
	if (ClientSocket == SOCKET_ERROR)
	{
		cout << "Accept failed : " << GetLastError() << endl;
		exit(-1);
	}

	const char Buffer[] = { "Hello World" };
	int SentByte = send(ClientSocket, Buffer, strlen(Buffer), 0);
	if (SentByte < 0)
	{
		cout << "Accept failed :" << GetLastError() << endl;
		exit(-1);
	}
	else if (SentByte == 0)
	{
		cout << "Disconnected :" << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Sent Byte : " << SentByte << ", " << Buffer << endl;
	}

	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
	if (RecvByte < 0)
	{
		cout << "Accept failed :" << GetLastError() << endl;
		exit(-1);
	}
	else if (RecvByte == 0)
	{
		cout << "Disconnected :" << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Recieved Byte : " << RecvByte << ", " << Buffer << endl;
	}

	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}