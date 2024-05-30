#include <iostream>
#include <map>
using namespace std;
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER        1024
#define SERVER_PORT        3500

struct SOCKETINFO // Ŭ���̾�Ʈ���� ���������� �������ϴ� �����͵� ����
{
	WSAOVERLAPPED overlapped;	// Ŭ���̾�Ʈ���� overlapped ����ü �ʿ�, recieve ���� overlapped ����ü�� ���� ���ȴ�
	WSABUF dataBuffer;	// WSABUF�� ���� �����Ͱ� ���� ���۰� �ƴ϶�, ���۵��� �����ϴ� ����(�ּ�, ���� ��) 
	SOCKET socket;
	char messageBuffer[MAX_BUFFER]; // ���� �����Ͱ� �� ����

	// ������ Send�� ���ۿ� Recieve�� ���۸� ���� ������������ �� ���α׷��� �ϳ��� �����ص� ������.
};

map <SOCKET, SOCKETINFO> clients;	// ���� ���Ӽ��������� key���� SOCKET���� �ִ� ���� �ٶ������� �ʴ�. �ֳ��ϸ� ���� ��ȣ�� �߱��������� ������ ����
// �Ϲ������δ� KEY���� ����ϴ� ID�� �־��ִ°��� �ٶ����ϴ�. ���� ���� ID�� ������, �� ������ �Ǹ� �ȵǴµ� ������ ID�� ������ �Ǳ� ������ ����� �ٶ������� ����.
void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);
void CALLBACK send_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags);

void CALLBACK recv_callback(DWORD Error, DWORD dataBytes, LPWSAOVERLAPPED overlapped, DWORD lnFlags)
{
	SOCKET client_s = reinterpret_cast<int>(overlapped->hEvent); // � ������ recv�� �������� Ȯ���ϱ� ���� hEvent���� ĳ�����ؼ� �̾Ƴ�

	if (dataBytes == 0)	// ������ ������ ���������� ���θ� Ȯ��, databytes�� 0���� Ȯ���ؼ�
	{
		closesocket(clients[client_s].socket);
		clients.erase(client_s);
		return;
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���
	clients[client_s].messageBuffer[dataBytes] = 0; // ���� ������ �տ� NULL���� ������༭ �ڿ� �ִ� ��� ���� ó�����ֱ� ���� �κ�
	cout << "From client : " << clients[client_s].messageBuffer << " (" << dataBytes << ") bytes)\n";
	clients[client_s].dataBuffer.len = dataBytes; // Ŭ���̾�Ʈ�� ������ �� �״�� ������ ���� ���̸� dataBytes�� ����. ���ۻ����� �״�� ������ �ȵ�
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED)); // �����ϱ����� 0���� �ʱ�ȭ
	clients[client_s].overlapped.hEvent = (HANDLE)client_s; // ���� ������ ���� hEvent���� client_s���� �־���� ��
	WSASend(client_s, &(clients[client_s].dataBuffer), 1, NULL, 0, &(clients[client_s].overlapped), send_callback); // ������ ���� - send�ϱ� sendcallback�� ��
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
	}  // Ŭ���̾�Ʈ�� closesocket�� ���� ���

	// WSASend(���信 ����)�� �ݹ��� ���
	clients[client_s].dataBuffer.len = MAX_BUFFER; // recv���� �پ�� ���ۻ���� �ٽ� �ִ�� �÷������

	cout << "TRACE - Send message : " << clients[client_s].messageBuffer << " (" << dataBytes << " bytes)\n";
	memset(&(clients[client_s].overlapped), 0, sizeof(WSAOVERLAPPED));
	clients[client_s].overlapped.hEvent = (HANDLE)client_s;
	WSARecv(client_s, &clients[client_s].dataBuffer, 1, 0, &flags, &(clients[client_s].overlapped), recv_callback);
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);	// 4��° ���� WSA_FLAG_OVERLAPPED �߿�, ���� ������ ERROR �߻���
	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	listen(listenSocket, 5); // ��뷮 ���������� 5�� �ƴ� SOMAXCONN�� ����ϴ� ���� ����
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&clientAddr, 0, addrLen);

	// �����Լ������� �ϳ��ϳ� �ְ�ް� ���� ����, �� CALLBACK���� ���ư� ���̴�
	while (true) {
		SOCKET clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		clients[clientSocket] = SOCKETINFO{};
		clients[clientSocket].socket = clientSocket;
		clients[clientSocket].dataBuffer.len = MAX_BUFFER;
		clients[clientSocket].dataBuffer.buf = clients[clientSocket].messageBuffer;
		memset(&clients[clientSocket].overlapped, 0, sizeof(WSAOVERLAPPED)); // ������� �ݵ�� 0���� �ʱ�ȭ �ؾ���
		clients[clientSocket].overlapped.hEvent = (HANDLE)clients[clientSocket].socket; // ������ �ļ�. � ������ recv���������� �˱� ���� ���. 
		// ���� ������ recv_callback �Լ��� �����ؼ� ���� ������ � ���������� �˱� ���� �����ϴ� ����̴�. 
		// �ü���� overlapped����ü ���� hEvent �ڵ��� ������ �����ϱ� ������ �̸� �̿��ؼ� ������ �����ϴ� ������� ����ϴ� ��.
		DWORD flags = 0;
		WSARecv(clients[clientSocket].socket, &clients[clientSocket].dataBuffer, 1, NULL,
			&flags, &(clients[clientSocket].overlapped), recv_callback);	// 4��°������ NULL�� Ȯ��
	}
	closesocket(listenSocket);
	WSACleanup();
}

