#pragma once

#define SERVER_PORT		9000

#define WORLD_WIDTH		800	// 맵 너비
#define WORLD_HEIGHT	800	// 맵 높이

#define NPC_ID_START	20000	// NPC ID 구분
#define NUM_NPC			2000	// NPC 수

constexpr int MAX_ID_LEN = 10;	// 최대 ID 길이
constexpr int MAX_STR_LEN = 100;  // string 최대 길이
constexpr int VIEW_RADIUS = 8; // 시야 범위

constexpr int ON_TILE_MAP = 0;
constexpr int NO_TILE_MAP = 1;

#define S2C_LOGIN_OK		1
#define S2C_MOVE			2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_CHAT			5
#define S2C_LOGIN_FAIL		6
#define S2C_CHANGE_STATS	7

#define C2S_LOGIN	1
#define C2S_MOVE	2

char g_Map[WORLD_WIDTH][WORLD_HEIGHT];
#include <random>

default_random_engine dre{ 9999 };
uniform_int_distribution <> uid{ 0,5 };

#pragma pack(push ,1)

enum TILE_STATUS
{
	eBLANK,
	eBLOCKED
};

struct sc_packet_login_ok {
	char size;
	char type;
	int id;
	short x, y;
	short hp;
	short level;
	int	exp;
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	short x, y;
	unsigned move_time;
};

constexpr unsigned char O_HUMAN = 0;
constexpr unsigned char O_ELF = 1;
constexpr unsigned char O_ORC = 2;

struct sc_packet_enter {
	char size;
	char type;
	int id;
	char name[MAX_ID_LEN];
	char o_type;
	short x, y;
};

struct sc_packet_leave {
	char size;
	char type;
	int id;
};

struct sc_packet_chat {
	char size;
	char type;
	int	id;
	char mess[MAX_STR_LEN];
};

struct cs_packet_login {
	char	size;
	char	type;
	char	name[MAX_ID_LEN];
};

constexpr unsigned char D_UP = 0;
constexpr unsigned char D_DOWN = 1;
constexpr unsigned char D_LEFT = 2;
constexpr unsigned char D_RIGHT = 3;

struct cs_packet_move {
	char	size;
	char	type;
	char	direction;
	unsigned move_time;
};

#pragma pack (pop)