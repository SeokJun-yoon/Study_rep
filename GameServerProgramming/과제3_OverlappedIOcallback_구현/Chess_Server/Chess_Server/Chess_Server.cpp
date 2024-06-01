#include <iostream>
#include <WS2tcpip.h>
#include <vector>
#include <map>
using namespace std;

#pragma comment(lib, "Ws2_32.lib") // Ws2_32 라이브러리 추가

#define PORT 3500
#define BUFFER_SIZE 1024

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// 체스말 이동 키값
#define KEY_LEFT	'4'
#define KEY_RIGHT	'6'
#define KEY_UP		'8'
#define KEY_DOWN	'2'

void error_display(const char* msg, int err_no)
{
    WCHAR* lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, err_no,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    std::cout << msg;
    std::wcout << L"< ERROR ! > " << lpMsgBuf << std::endl;
    while (true);
    LocalFree(lpMsgBuf);
}

struct ClientInfo {
    SOCKET socket;

    OVERLAPPED recvOverlapped;
    char recvBuffer[BUFFER_SIZE];
    WSABUF recvWsabuf;

    OVERLAPPED sendOverlapped;
    char sendBuffer[BUFFER_SIZE];
    WSABUF sendWsabuf;

    short x, y;
    char command;
};

void CALLBACK RecvCompletionRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD flags);

map<SOCKET, ClientInfo*> clientMap;
const int interval = 80; // 간격 값

int main()
{
    cout << "< SERVER ON >" << endl;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);

    // 소켓 생성 단계 (IPv4, TCP)
    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    // 주소 설정 단계 (아이피 주소 / 포트 번호 설정)
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 누구나 접속 가능하도록

    // bind 단계 (소켓과 주소를 묶는다.)
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error_display("bind() error", WSAGetLastError());
        return 1;
    }

    // listen 단계 (클라이언트가 접속 가능한 대기 상태)
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        error_display("listen() error", WSAGetLastError());
        return 1;
    }

    SOCKET client_socket;
    SOCKADDR_IN client_addr;
    int addr_size = sizeof(client_addr);

    while (true) 
    {
        client_socket = accept(listenSocket, (sockaddr*)&client_addr, &addr_size);
        if (client_socket == INVALID_SOCKET) 
        {
            cout << "INVALID SOCKET" << endl;
            continue;
        }

        cout << "CONNECT !" << endl;

        ClientInfo* clientInfo = new ClientInfo();
        clientInfo->socket = client_socket;
        clientInfo->x = 0;
        clientInfo->y = 0;
        clientMap[client_socket] = clientInfo;

        memset(&clientInfo->recvOverlapped, 0, sizeof(OVERLAPPED));
        clientInfo->recvWsabuf.buf = clientInfo->recvBuffer;
        clientInfo->recvWsabuf.len = BUFFER_SIZE;

        DWORD recvFlags = 0;
        if (WSARecv(client_socket, &clientInfo->recvWsabuf, 1, NULL, &recvFlags, 
            &clientInfo->recvOverlapped, RecvCompletionRoutine) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING) 
            {
                std::cerr << "WSARecv failed: " << WSAGetLastError() << "\n";
                closesocket(client_socket);
                clientMap.erase(client_socket);
            }
        }
        closesocket(client_socket);
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}

void CALLBACK RecvCompletionRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD flags) 
{
    ClientInfo* clientInfo = (ClientInfo*)overlapped;

    if (bytesTransferred > 0 && error == 0) 
    {
        int input = atoi(clientInfo->recvBuffer);
        
        // 현재 위치 갱신
        if (input == KEY_DOWN) clientInfo->y--;
        else if (input == KEY_UP) clientInfo->y++;
        else if (input == KEY_RIGHT) clientInfo->x--;
        else if (input == KEY_LEFT) clientInfo->x++;

        std::cout << "Client " << clientInfo->socket << " moved to (" << clientInfo->x << ", " << clientInfo->y << ")\n";

        // 위치를 모든 클라이언트에게 전송
        //BroadcastPosition(clientInfo, input);

        // 다시 recv 요청
        memset(&clientInfo->recvOverlapped, 0, sizeof(OVERLAPPED));
        clientInfo->recvWsabuf.buf = clientInfo->recvBuffer;
        clientInfo->recvWsabuf.len = BUFFER_SIZE;
        DWORD recvFlags = 0;
        if (WSARecv(clientInfo->socket, &clientInfo->recvWsabuf, 1, NULL, &recvFlags, &clientInfo->recvOverlapped, RecvCompletionRoutine) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                std::cerr << "WSARecv failed: " << WSAGetLastError() << "\n";
                closesocket(clientInfo->socket);
                delete clientInfo;
                clientMap.erase(clientInfo->socket);
            }
        }
    }
    else {
        std::cout << "Client " << clientInfo->socket << " disconnected\n";
        closesocket(clientInfo->socket);
        delete clientInfo;
        clientMap.erase(clientInfo->socket);
    }
}