#include <iostream>
#include <map>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_PORT        3500

struct SOCKETINFO // 클라이언트들이 독자적으로 가져야하는 데이터들 포함
{
	WSAOVERLAPPED overlapped;	// 클라이언트마다 overlapped 구조체 필요, recieve 전용 overlapped 구조체로 많이 사용된다
	WSABUF dataBuffer;	// WSABUF는 실제 데이터가 들어가는 버퍼가 아니라, 버퍼들을 관리하는 버퍼(주소, 정보 등) 
	SOCKET socket;
	char messageBuffer[MAX_BUFFER]; // 실제 데이터가 들어갈 버퍼

	// 원래는 Send용 버퍼와 Recieve용 버퍼를 따로 만들어야하지만 이 프로그램은 하나로 재사용해도 괜찮다.
};

map <SOCKET, SOCKETINFO> clients;	// 실제 게임서버에서는 key값을 SOCKET으로 주는 것은 바람직하지 않다. 왜냐하면 소켓 번호가 중구난방으로 들어오기 때문
// 일반적으로는 KEY값에 사용하는 ID를 넣어주는것이 바람직하다. 또한 원래 ID는 고유값, 즉 재사용이 되면 안되는데 소켓의 ID는 재사용이 되기 때문에 사용이 바람직하지 않음.
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent); // 어떤 소켓이 recv를 보낸건지 확인하기 위해 hEvent에서 캐스팅해서 뽑아냄

	if (dataBytes == 0)	// 소켓이 연결이 끊어졌는지 여부를 확인, databytes가 0인지 확인해서
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // 클라이언트가 closesocket을 했을 경우
	clients[client_s].messageBuffer[dataBytes] = 0; // 실제 데이터 앞에 NULL값을 만들어줘서 뒤에 있는 찌꺼기 값을 처리해주기 위한 부분
	cout << "From client : " << clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	clients[client_s].dataBuffer.len = dataBytes; // 클라이언트가 보내온 양 그대로 보내기 위해 길이를 dataBytes로 셋팅. 버퍼사이즈 그대로 보내면 안됨
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED)); // 재사용하기위해 0으로 초기화
	clients[client_s].overlapped.hEvent = (HANDLE)client_s; // 소켓 구분을 위한 hEvent값은 client_s값을 넣어줘야 함
	WSASend(client_s, &(clients[client_s].dataBuffer), 1, NULL, 0, &(clients[client_s].overlapped), send_callback); // 마지막 인자 - send니까 sendcallback이 들어감
}

void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	DWORD receiveBytes = 0;
	DWORD flags = 0;

	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent);

	if (dataBytes == 0) {
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // 클라이언트가 closesocket을 했을 경우

	// WSASend(응답에 대한)의 콜백일 경우
	clients[client_s].dataBuffer.len = MAX_BUFFER; // recv에서 줄어든 버퍼사이즈를 다시 최대로 늘려줘야함

	cout << "TRACE - Send message : " << clients[client_s].messageBuffer << " (" << dataBytes << " bytes)\n";
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &clients[client_s].dataBuffer, 1, 0, &flags, &(clients[client_s].overlapped), recv_callback);
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);	// 4번째 항의 WSA_FLAG_OVERLAPPED 중요, 하지 않으면 ERROR 발생함
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(listenSocket, 5); // 대용량 서버에서는 5가 아닌 SOMAXCONN을 사용하는 것이 맞음
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);

	// 메인함수에서는 하나하나 주고받고를 하지 않음, 즉 CALLBACK으로 돌아갈 것이다
	while (true) {
		SOCKET clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		memset(&clients[clientSocket].overlapped, 0, sizeof(WSAOVERLAPPED)); // 사용전에 반드시 0으로 초기화 해야함
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket; // 일종의 꼼수. 어떤 소켓의 recv정보인지를 알기 위한 방법. 
		// 여러 소켓이 recv_callback 함수를 공유해서 쓰기 때문에 어떤 소켓인지를 알기 위해 설정하는 방법이다. 
		// 운영체제가 overlapped구조체 안의 hEvent 핸들을 어차피 무시하기 때문에 이를 이용해서 소켓을 구분하는 방법으로 사용하는 것.
		DWORD flags = 0;
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, NULL,
			&flags, &(clients[clientSocket].overlapped), recv_callback);	// 4번째인자의 NULL값 확인
	}
	closesocket(listenSocket);
	WSACleanup();
}

