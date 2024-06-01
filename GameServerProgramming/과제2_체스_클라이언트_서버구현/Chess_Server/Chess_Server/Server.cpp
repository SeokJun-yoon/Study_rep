#include <iostream>
#include <WS2tcpip.h>
#include <vector>
using namespace std;

#pragma comment(lib, "Ws2_32.lib") // Ws2_32 ���̺귯�� �߰�
#define PORT 3500
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

struct ChessPiece {
    short x, y;
    char command;
};

int interval = 80; // ���� ��
ChessPiece chesspiece;

int main()
{
    cout << "< SERVER ON >" << endl;
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);

    // ���� ���� �ܰ� (IPv4, TCP)
    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);

    // �ּ� ���� �ܰ� (������ �ּ� / ��Ʈ ��ȣ ����)
    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET; // IPv4
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; // ������ ���� �����ϵ���

    // bind �ܰ� (���ϰ� �ּҸ� ���´�.)
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error_display("bind() error", WSAGetLastError());
        return 1;
    }

    // listen �ܰ� (Ŭ���̾�Ʈ�� ���� ������ ��� ����)
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        error_display("listen() error", WSAGetLastError());
        return 1;
    }

    while (true) {
        SOCKET client_socket;
        SOCKADDR_IN client_addr;
        int addr_size = sizeof(client_addr);
        client_socket = accept(listenSocket, (sockaddr*)&client_addr, &addr_size);

        if (client_socket == INVALID_SOCKET) {
            cout << "INVALID SOCKET" << endl;
            continue;
        }

        cout << "CONNECT !" << endl;

        while (true) {
            int recv_size = recv(client_socket, (char*)&chesspiece, sizeof(chesspiece), 0);
            if (recv_size == 0 || recv_size == SOCKET_ERROR) {
                cout << "Client disconnected." << endl;
                break;
            }

            // Ŭ���̾�Ʈ�κ��� ���� ��ɿ� ���� ü�� ���� ��ġ ����
            if (chesspiece.command == 'L') // �� ����Ű �Է� ��
            {
                chesspiece.x -= interval;
                if (chesspiece.x < interval * 0)
                    chesspiece.x = interval * 0;
            }

            if (chesspiece.command == 'R') // �� ����Ű �Է� ��
            {
                chesspiece.x += interval;
                if (chesspiece.x > interval * 7)
                    chesspiece.x = interval * 7;
            }

            if (chesspiece.command == 'D') // �ϴ� ����Ű �Է� ��
            {
                chesspiece.y += interval;
                if (chesspiece.y > interval * 7)
                    chesspiece.y = interval * 7;
            }

            if (chesspiece.command == 'U') // ��� ����Ű �Է� ��
            {
                chesspiece.y -= interval;
                if (chesspiece.y < interval * 0)
                    chesspiece.y = interval * 0;
            }

            // Ŭ���̾�Ʈ���� ������ ü�� ���� ��ġ ����
            send(client_socket, (char*)&chesspiece, sizeof(chesspiece), 0);
        }

        closesocket(client_socket);
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
