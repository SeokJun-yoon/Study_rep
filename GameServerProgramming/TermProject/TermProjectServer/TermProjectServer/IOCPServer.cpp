#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")

#include <vector>
#include <mutex>
#include <unordered_set>
#include <atomic>
using namespace std;

#include "protocol.h"
constexpr auto MAX_PACKET_SIZE = 255; // 최대 패킷 사이즈
constexpr auto MAX_BUF_SIZE = 1024;	// 최대 버퍼 사이즈
constexpr auto MAX_USER = 10000;	// 최대 접속 유저 수
constexpr auto VIEW_RADIOS = 5; // 시야 범위

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT};
enum C_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE };

struct EXOVER {
	WSAOVERLAPPED	over;
	ENUMOP			op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF		wsabuf;
		SOCKET		c_socket;
	};
};

struct CLIENT {
	mutex		m_cl;
	SOCKET		m_s;
	int			m_id;
	EXOVER		m_recv_over;
	int			m_prev_size;
	char		m_packt_buf[MAX_PACKET_SIZE];
	atomic <C_STATUS> m_status;

	short x, y;
	char m_name[MAX_ID_LEN + 1];
	unsigned m_move_time;

	unordered_set <int> m_view_list;
};