#include <iostream>
#include <TCHAR.H>
#include <WS2tcpip.h>
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
constexpr int PORT = 50000; // 포트 번호 50000

char SERVER_ADDR[] = "127.0.0.1";   // 서버 주소, 127.0.0.1 은 자신을 가리키는 IP

struct ChessPiece {
    short x, y;
    char command;
};

ChessPiece chesspiece;

// 전역 변수:
int WindowPosition_X = 400; // 창의 시작 될 때의 X좌표
int WindowPosition_Y = 50; //  창이 시작 될 때의 Y좌표
int WindowSize_X = 660; // 창의 X 길이
int WindowSize_Y = 680; // 창의 Y 길이
int interval = 80; // 간격 값
bool is_connected = false;

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
    LocalFree(lpMsgBuf);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    HWND hwnd;
    MSG msg;
    WNDCLASS WndClass;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = _T("Chess");
    RegisterClass(&WndClass);
    hwnd = CreateWindow(_T("Chess"),
        _T("Chess"),
        WS_OVERLAPPEDWINDOW,
        WindowPosition_X,
        WindowPosition_Y,
        WindowSize_X,
        WindowSize_Y,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
    WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH hBrush;
    HBRUSH oldBrush;

    static RECT ChessBoard[8][8];

    static WSADATA WSAData;
    static SOCKET s_socket;
    static SOCKADDR_IN server_addr;

    switch (iMsg)
    {
    case WM_CREATE:
        memset(&server_addr, 0, sizeof(server_addr));
        if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
        {
            error_display("WSAStartup failed", WSAGetLastError());
            return -1;
        }

        while (true)
        {
            s_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(PORT);

            cout << "서버 IP 주소 입력 (기본값: 127.0.0.1): ";
            cin >> SERVER_ADDR;

            if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) <= 0)
            {
                error_display("inet_pton failed", WSAGetLastError());
                closesocket(s_socket);
                continue;
            }

            if (connect(s_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
            {
                error_display("connect failed", WSAGetLastError());
                closesocket(s_socket);
                cout << "연결 실패. 주소를 다시 입력하세요!" << endl;
            }
            else
            {
                cout << "서버 연결 성공!" << endl;
                is_connected = true;
                break;
            }
        }

        chesspiece.x = interval * 3;
        chesspiece.y = interval * 3;

        break;

    case WM_KEYDOWN:
        if (!is_connected) break;

        switch (wParam)
        {
        case VK_LEFT:
            chesspiece.command = 'L';
            break;

        case VK_RIGHT:
            chesspiece.command = 'R';
            break;
        case VK_DOWN:
            chesspiece.command = 'D';
            break;

        case VK_UP:
            chesspiece.command = 'U';
            break;

        default:
            return DefWindowProc(hwnd, iMsg, wParam, lParam);
        }

        if (send(s_socket, (char*)&chesspiece, sizeof(chesspiece), 0) == SOCKET_ERROR)
        {
            error_display("send failed", WSAGetLastError());
            break;
        }

        if (recv(s_socket, (char*)&chesspiece, sizeof(ChessPiece), 0) == SOCKET_ERROR)
        {
            error_display("recv failed", WSAGetLastError());
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // 체스 판 출력 부분
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                ChessBoard[i][j].left = i * interval;
                ChessBoard[i][j].right = interval + i * interval;
                ChessBoard[i][j].top = j * interval;
                ChessBoard[i][j].bottom = interval + j * interval;
                if ((i + j) % 2 == 1)
                {
                    hBrush = CreateSolidBrush(RGB(0, 0, 0));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                    Rectangle(hdc, ChessBoard[i][j].left, ChessBoard[i][j].top, ChessBoard[i][j].right, ChessBoard[i][j].bottom);
                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }
                else
                {
                    Rectangle(hdc, ChessBoard[i][j].left, ChessBoard[i][j].top, ChessBoard[i][j].right, ChessBoard[i][j].bottom);
                }
            }
        }

        // 체스 말 출력 부분
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Ellipse(hdc, chesspiece.x, chesspiece.y, chesspiece.x + interval, chesspiece.y + interval);
        SelectObject(hdc, oldBrush);
        EndPaint(hwnd, &ps);
        break;

    case WM_DESTROY:
        if (is_connected && closesocket(s_socket) == SOCKET_ERROR)
        {
            error_display("closesocket failed", WSAGetLastError());
        }
        WSACleanup();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, iMsg, wParam, lParam);
    }
    return 0;
}
