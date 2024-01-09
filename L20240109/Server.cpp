#include <iostream>
#include <WinSock2.h> //Sock 사용을 위한 친구.

#pragma comment(lib, "ws2_32")
using namespace std;

//GetLastError -> 에러 코드 확인용

int main()
{
	WSAData wsaData; // 여기서부터
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	} // 여기까지 윈도우 코드
	//소켓 생성
	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID_SOCKET : " << GetLastError() << endl;
		exit(-1);
	}
	//tcp용 연결 소켓 생성
	SOCKADDR_IN ListenSockAddr; 
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	//ZeroMemory(&ListenSockAddr, sizeof(ListenSockAddr));
	ListenSockAddr.sin_family = AF_INET;
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(40211);

	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr)); //바인드를 통해 소켓에 구멍을 뚫고 소켓이 실행할 때마다 바인드된 함수를 실행
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	Result = listen(ListenSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ListenSockAddr, &ClientSocketAddrLength);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept fail : " << GetLastError() << endl;
		exit(-1);
	}



	const char Buffer[] = {"Hello"};

	int SentByte = send(ClientSocket, Buffer, strlen(Buffer), 0);
	if (SentByte < 0)
	{
		cout << "Error fail : " << GetLastError() << endl;
		exit(-1);
	}
	else if (SentByte == 0)
	{
		cout << "Disonnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Sent byte : " << SentByte << ", " << Buffer <<  endl;
		exit(-1);
	}

	

	char RecvBuffer[1024] = { 0, };	
	int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
	if (RecvByte < 0)
	{
		cout << "Error fail : " << GetLastError() << endl;
		exit(-1);
	}
	else if (RecvByte == 0)
	{
		cout << "Disonnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Recv byte : " << RecvByte << ", " << RecvBuffer << endl;
		exit(-1);
	}

	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}