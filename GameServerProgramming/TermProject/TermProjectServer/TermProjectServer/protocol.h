#pragma once
#include <random>

#define SERVER_PORT		9000

#define WORLD_WIDTH		800	// 맵 너비
#define WORLD_HEIGHT	800	// 맵 높이

#define PERIODICALLY_SAVE_TIME 60000 // 유저 데이터 저장 주기

#define MAX_USER		3000	// 총 유저 수
#define NUM_NPC			9000	// NPC 수
#define NUMBER_OF_NPC_TYPES	3	// NPC 종류의 수
#define NUM_NPC1		NUM_NPC / NUMBER_OF_NPC_TYPES
#define NUM_NPC2		NUM_NPC / NUMBER_OF_NPC_TYPES
#define NUM_NPC3		NUM_NPC / NUMBER_OF_NPC_TYPES

#define NPC_ID_START	NUM_NPC / NUMBER_OF_NPC_TYPES	// NPC ID 구분
#define NPC2_ID_START	NPC_ID_START + NUM_NPC1
#define NPC3_ID_START	NPC2_ID_START + NUM_NPC2
#define QUEST_NPC_NUMBER	MAX_USER + NUM_NPC  // 현재 설정 값 : 12000

constexpr int MAX_ID_LEN = 10;		// 최대 ID 길이
constexpr int MAX_STR_LEN = 100;	// string 최대 길이
constexpr int VIEW_RADIUS = 8;		// 시야 범위

constexpr auto MAX_PACKET_SIZE = 255; // 최대 패킷 사이즈
constexpr auto MAX_BUF_SIZE = 1024;	// 최대 버퍼 사이즈


constexpr unsigned char O_PLAYER = 0;
constexpr unsigned char O_NPC1 = 1;
constexpr unsigned char O_NPC2 = 2;
constexpr unsigned char O_NPC3 = 3;

constexpr unsigned char D_UP = 0;
constexpr unsigned char D_DOWN = 1;
constexpr unsigned char D_LEFT = 2;
constexpr unsigned char D_RIGHT = 3;

#define PLAYER_MAX_HP 100	// 플레이어 최대 체력

#define S2C_LOGIN_OK		1
#define S2C_MOVE			2
#define S2C_ENTER			3
#define S2C_LEAVE			4
#define S2C_CHAT			5
#define S2C_LOGIN_FAIL		6
#define S2C_CHANGE_STATS	7
#define S2C_ATTACK			8

#define C2S_LOGIN	1
#define C2S_MOVE	2
#define C2S_ATTACK  3
#define C2S_CHAT	4
#define C2S_LOGOUT	5
#define C2S_REVIVE	6

char g_Map[WORLD_WIDTH][WORLD_HEIGHT];

std::default_random_engine dre{ 9999 };
std::uniform_int_distribution <> uid{ 0,5 };

#pragma pack(push ,1)

enum TILE_STATUS
{
	eBLANK,
	eBLOCKED
};

struct sc_packet_login_ok {
	char size;
	char type;
	char name[MAX_ID_LEN];
	int id;
	int x, y;
	int hp;
	int maxhp;
	int level;
	int maxexp;
	int	exp;
	int att;
};

struct sc_packet_login_fail {
	char  size;
	char  type;
	int	  id;
	char  message[MAX_STR_LEN];
};

struct sc_packet_move {
	char size;
	char type;
	int id;
	short x, y;
	unsigned move_time;
};

struct sc_packet_attack {
	char size;
	char type;
	int id;
	short x, y;
	short attackrange;
};

struct sc_packet_enter {
	char size;
	char type;
	int id;
	int x, y;
	int hp;
	int maxhp;
	int level;
	int maxexp;
	int	exp;
	int att;
	char name[MAX_ID_LEN];
	char o_type;
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
	int mess_type;
};

struct sc_packet_stat_change {
	char size;
	char type;

	int id;
	int level;
	int hp;
	int maxhp;
	int exp;
	int att;
	int maxexp;
	int attrange;
	int givenexp;

	bool isRevive;
	int x, y;
};

struct cs_packet_login {
	char	size;
	char	type;
	char	name[MAX_ID_LEN];
	int		loginType;
};

struct cs_packet_move {
	char	size;
	char	type;
	char	direction;
	unsigned move_time;
};

struct cs_packet_attack {
	char	size;
	char	type;
};

struct cs_packet_chat {
	char	size;
	char	type;
	char	message[MAX_STR_LEN];
};

struct cs_packet_logout {
	char	size;
	char	type;
};

struct cs_packet_revive {
	char	size;
	char	type;
};

#pragma pack (pop)