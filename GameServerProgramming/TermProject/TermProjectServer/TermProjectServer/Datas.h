#pragma once
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <MSWSock.h>

#include <iostream>
#include <vector>
#include <unordered_set>
#include <thread>
#include <atomic>
#include <math.h>
#include <mutex>
#include <chrono>
#include <queue>
#include <string>
#include <sqlext.h> 
#include <locale>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "protocol.h"
#include "EnumList.h"


struct EventType 
{
	int obj_id;
	int target_id;
	WORKER_OP event_id;
	std::chrono::high_resolution_clock::time_point wakeup_time;

	EventType(int objectID, int targetID, WORKER_OP type, std::chrono::high_resolution_clock::time_point dur)
	{
		obj_id = objectID;
		target_id = targetID;
		event_id = type;
		wakeup_time = dur;
	}

	constexpr bool operator < (const EventType & left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};

// Overlapped 구조체
struct ExOver {
	WSAOVERLAPPED	over;
	WORKER_OP			op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF		wsabuf;
		SOCKET		c_socket;
		int			p_id;
	};
};

// 클라이언트 구조체
struct Client {
	std::mutex	m_cl;
	SOCKET		m_s;
	int			m_id;
	ExOver		m_recv_over;
	int			m_prev_size;

	char		m_packet_buf[MAX_PACKET_SIZE];
	std::atomic <ObjectStatus> m_status;
	char m_name[MAX_ID_LEN + 1];
	short x, y;

	// Player
	int DBKey;
	int m_hp;
	int m_maxhp;
	int m_att;
	int m_exp;
	int m_maxexp;
	int m_level;
	int m_attackrange;
	// * for quest
	bool m_quest1_status = false;	// quest1 수령 상태 체크용. 수령 상태일 시 true
	bool m_quest1_complete = false; // quest1 완료 상태 확인용. 완료 시 true로 변경
	int m_quest1_count = 0;		// level 1 monster 잡으면 ++

	bool m_quest2_status = false;	// quest2 수령 상태 체크용. 수령 상태일 시 true
	bool m_quest2_complete = false; // quest2 완료 상태 확인용. 완료 시 true로 변경
	int m_quest2_count = 0;		// level 2 monster 잡으면 ++

	bool m_quest3_status = false;	// quest3 수령 상태 체크용. 수령 상태일 시 true
	bool m_quest3_complete = false; // quest3 완료 상태 확인용. 완료 시 true로 변경
	int m_quest3_count = 0;		// level 3 monster(boss monster) 잡으면 ++


	// NPC
	int given_exp;
	bool is_active;
	bool is_alive;


	unsigned m_move_time;
	std::chrono::high_resolution_clock::time_point m_last_move_time;

	// View List
	std::unordered_set <int> m_view_list;

	// LUA
	lua_State* L;
	std::mutex lua_l;

	std::chrono::high_resolution_clock::time_point m_last_connect_time;

	ObjectType objectType;
};