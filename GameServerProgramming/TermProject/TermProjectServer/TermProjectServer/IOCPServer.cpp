#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")
#pragma comment (lib, "lua53.lib")
#include "protocol.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

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

using namespace std;
using namespace chrono;

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT, OP_RANDOM_MOVE, OP_PLAYER_MOVE, OP_CHECK_CONNECT_TIME};
enum PLAYER_MAX_EXP { LEVEL1_MAX_EXP = 20, LEVEL2_MAX_EXP = 40, LEVEL3_MAX_EXP = 60};
enum PLAYER_ATTACK_DATA { LEVEL1_ATT = 10, LEVEL2_ATT = 15, LEVEL3_ATT = 20 };
enum MONSTER_MAX_HP { MON_LEVEL1_HP = 50, MON_LEVEL2_HP = 100, MON_LEVEL3_HP = 150 };
enum MONSTER_ATTACK_DATA { MON_LEVEL1_ATT = 5, MON_LEVEL2_ATT = 10, MON_LEVEL3_ATT = 15 };
enum MONSTER_GIVEN_EXP { MON_LEVEL1_EXP = 5, MON_LEVEL2_EXP = 8, MON_LEVEL3_EXP = 10 };

struct event_type {
	int obj_id;
	ENUMOP event_id;
	high_resolution_clock::time_point wakeup_time;
	int target_id;

	constexpr bool operator < (const event_type & left) const
	{
		return (wakeup_time > left.wakeup_time);
	}
};

priority_queue<event_type> timer_queue;
mutex timer_lock;

enum C_STATUS { ST_FREE, ST_ALLOC, ST_ACTIVE, ST_SLEEP };

// 차후 DB연동용 (주석해제하고 사용)
//void Load_Database();
//void Update_Database(int id, int x, int y, int hp, int level, int exp);

struct DATABASE {	// 데이터 베이스에 저장할 정보 : id, name, level, 현재 hp, 현재 exp, x 좌표, y 좌표, quest(1,2,3) 수령 상태, quest(1,2,3) 완료 상태, quest(1,2,3) 진행 정도, 
	int db_user_id;
	char db_user_name[10];
	int db_user_level;
	int db_hp;
	int db_exp;
	int db_x_pos;
	int db_y_pos;
	bool db_quest1_status;
	int db_quest1_count;
	bool db_quest1_complete;
	bool db_quest2_status;
	int db_quest2_count;
	bool db_quest2_complete;
	bool db_quest3_status;
	int db_quest3_count;
	bool db_quest3_complete;
};

// Overlapped 구조체
struct EXOVER {
	WSAOVERLAPPED	over;
	ENUMOP			op;
	char			io_buf[MAX_BUF_SIZE];
	union {
		WSABUF		wsabuf;
		SOCKET		c_socket;
		int			p_id;
	};
};

// 클라이언트 구조체
struct CLIENT {
	mutex		m_cl;
	SOCKET		m_s;
	int			m_id;
	EXOVER		m_recv_over;
	int			m_prev_size;

	char		m_packet_buf[MAX_PACKET_SIZE];
	atomic <C_STATUS> m_status;
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
	high_resolution_clock::time_point m_last_move_time;

	// View List
	unordered_set <int> m_view_list;

	// LUA
	lua_State* L;
	mutex lua_l;

	high_resolution_clock::time_point m_last_connect_time;
};

// [ 전역 변수 ]
vector<DATABASE> vec_database;

CLIENT g_clients[MAX_USER + NUM_NPC + 1];
HANDLE g_iocp;
SOCKET l_socket;
atomic_int UserCount = 0;

// 데이터베이스
const wchar_t* DSN_NAME = L"game_db_odbc";
const int QUERY_BUFFER_SIZE = 1024;
const int NAME_LEN = 50;

// 진단 레코드 처리 함수
void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	WCHAR wszMessage[1000];
	WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// 데이터가 잘린 경우는 숨김
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

// 데이터베이스 연결 함수
SQLRETURN ConnectDatabase(SQLHENV& henv, SQLHDBC& hdbc, SQLHSTMT& hstmt)
{
	SQLRETURN retcode;
	// 환경 핸들 할당
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		// ODBC 버전 설정
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// 연결 핸들 할당
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				// 로그인 타임아웃 설정
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
				// 데이터 소스에 연결
				retcode = SQLConnect(hdbc, (SQLWCHAR*)DSN_NAME, SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					// 스테이트먼트 핸들 할당
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
				}
			}
		}
	}
	return retcode;
}

// 데이터베이스 연결 해제 함수
void DisconnectDatabase(SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt)
{
	if (hstmt) {
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
	}
	if (hdbc) {
		SQLDisconnect(hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	}
	if (henv) {
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
	}
}

// 캐릭터 정보를 로드하는 함수
void DB_LoadCharacter(int user_id, const wstring& inputName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;

	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
	{
		wcout << L"ODBC Connect Ok!" << endl;

		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.LoadCharacter '%s'", inputName.c_str());

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			wcout << L"SELECT OK!" << endl;

			SQLWCHAR Name[NAME_LEN];
			SQLINTEGER x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp;
			SQLLEN cbName = 0, cbx = 0, cby = 0, cbLevel = 0, cbAttackDamage = 0, cbAttackRange = 0,
				cbCurrExp = 0, cbMaxExp = 0, cbCurrHp = 0, cbMaxHp = 0;

			// 컬럼 바인딩
			SQLBindCol(hstmt, 1, SQL_C_WCHAR, &Name, NAME_LEN, &cbName);
			SQLBindCol(hstmt, 2, SQL_C_LONG, &x, 0, &cbx);
			SQLBindCol(hstmt, 3, SQL_C_LONG, &y, 0, &cby);
			SQLBindCol(hstmt, 4, SQL_C_LONG, &Level, 0, &cbLevel);
			SQLBindCol(hstmt, 5, SQL_C_LONG, &AttackDamage, 0, &cbAttackDamage);
			SQLBindCol(hstmt, 6, SQL_C_LONG, &AttackRange, 0, &cbAttackRange);
			SQLBindCol(hstmt, 7, SQL_C_LONG, &CurrExp, 0, &cbCurrExp);
			SQLBindCol(hstmt, 8, SQL_C_LONG, &MaxExp, 0, &cbMaxExp);
			SQLBindCol(hstmt, 9, SQL_C_LONG, &CurrHp, 0, &cbCurrHp);
			SQLBindCol(hstmt, 10, SQL_C_LONG, &MaxHp, 0, &cbMaxHp);

			// 데이터 가져오기
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_NO_DATA)
			{
				wcout << L"No data found." << endl;
			}
			else if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
			{
				g_clients[user_id].x = x;
				g_clients[user_id].y = y;
				g_clients[user_id].m_level = Level;
				g_clients[user_id].m_att = AttackDamage;
				g_clients[user_id].m_attackrange = AttackRange;
				g_clients[user_id].m_maxexp = MaxExp;
				g_clients[user_id].m_maxhp = MaxHp;
				g_clients[user_id].m_hp = CurrHp;

				// 데이터 확인 출력 (선택 사항)
				wprintf(L"Name: %s, Level: %d, Exp: %d, HP: %d/%d\n",
					Name, Level, CurrExp, CurrHp, MaxHp);
			}
			else {
				wcout << L"Error fetching data." << endl;
				HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
			}
		}
		else {
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else {
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}

void DB_CreateCharacter(const wstring& inputName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;

	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		wcout << L"ODBC Connect Ok!" << endl;

		int x = ((rand() % WORLD_WIDTH) % 6) + 11;
		int y = ((rand() % WORLD_HEIGHT) % 6) + 16;

		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.CreateCharacter '%s', %d, %d", inputName.c_str(), x, y);

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) 
		{
			wcout << L"INSERT OK!" << endl;
		}
		else 
		{
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else {
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}

void DB_SaveCharacter(int user_id, const wstring& inputName)
{
	SQLHENV henv = NULL;
	SQLHDBC hdbc = NULL;
	SQLHSTMT hstmt = NULL;
	SQLRETURN retcode;
	SQLINTEGER x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp;
	setlocale(LC_ALL, "korean");

	retcode = ConnectDatabase(henv, hdbc, hstmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		wcout << L"ODBC Connect Ok!" << endl;

		x = g_clients[user_id].x;
		y = g_clients[user_id].y;
		Level = g_clients[user_id].m_level;
		AttackDamage = g_clients[user_id].m_att;
		AttackRange = g_clients[user_id].m_attackrange;
		MaxExp = g_clients[user_id].m_maxexp;
		CurrExp = g_clients[user_id].m_exp;
		MaxHp = g_clients[user_id].m_maxhp;
		CurrHp = g_clients[user_id].m_hp;

		// 데이터 확인 출력 (선택 사항)
		wprintf(L"Name: %s, Level: %d, Exp: %d / %d , HP: %d / %d \n",
			inputName.c_str(), Level, CurrExp, MaxExp, CurrHp, MaxHp);


		// SQLWCHAR 배열에 SQL 쿼리 저장
		SQLWCHAR query[1024];
		swprintf(query, 1024, L"EXEC GAME.dbo.SaveCharacter '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d", 
			inputName.c_str(), x, y, Level, AttackDamage, AttackRange, CurrExp, MaxExp, CurrHp, MaxHp);

		// SQL 쿼리 실행
		retcode = SQLExecDirect(hstmt, query, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			wcout << L"SAVE OK!" << endl;
		}
		else
		{
			wcout << L"WRONG SQL" << endl;
			HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
		}

		DisconnectDatabase(henv, hdbc, hstmt);
	}
	else {
		wcout << L"Failed to connect to the database." << endl;
		HandleDiagnosticRecord(hdbc, SQL_HANDLE_DBC, retcode);
	}
}


std::wstring CharArrayToWString(const char* charArray) 
{
	// char 배열의 길이를 계산
	int length = static_cast<int>(strlen(charArray));

	// 필요한 버퍼 크기를 계산 (最大 2배 길이)
	int bufferSize = MultiByteToWideChar(CP_ACP, 0, charArray, length, NULL, 0);

	// wstring을 담을 버퍼
	std::wstring wstr(bufferSize, L'\0');

	// 변환 수행
	MultiByteToWideChar(CP_ACP, 0, charArray, length, &wstr[0], bufferSize);

	return wstr;
}


// Timer
void add_timer(int obj_id, ENUMOP op_type, int duration)
{
	timer_lock.lock();
	event_type ev{ obj_id, op_type, high_resolution_clock::now() + milliseconds(duration), 0 };
	timer_queue.push(ev);
	timer_lock.unlock();
}

bool is_player(int id)
{
	return id < NPC_ID_START;
}

bool is_near(int a, int b)	// 시야 확인
{
	if (abs(g_clients[a].x - g_clients[b].x) > VIEW_RADIUS) return false;	// 시야를 벗어나게 되면 false 리턴 (x)
	if (abs(g_clients[a].y - g_clients[b].y) > VIEW_RADIUS) return false;	// 시야를 벗어나게 되면 false 리턴 (y)
	return true;
}

// send 함수

void send_packet(int user_id, void* p)
{
	char* buf = reinterpret_cast<char *>(p);

	CLIENT& u = g_clients[user_id];

	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);
	WSASend(u.m_s, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void send_login_ok_packet(int user_id)
{
	sc_packet_login_ok p;
	p.id = user_id;
	p.level = g_clients[user_id].m_level;
	p.exp = g_clients[user_id].m_exp;
	p.maxexp = g_clients[user_id].m_maxexp;
	p.hp = g_clients[user_id].m_hp;
	p.maxhp = g_clients[user_id].m_maxhp;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_OK;
	p.x = g_clients[user_id].x;
	p.y = g_clients[user_id].y;

	send_packet(user_id, &p);
}

void send_login_fail(int user_id)
{
	sc_packet_login_fail p;
	p.size = sizeof(p);
	p.type = S2C_LOGIN_FAIL;

	char error[MAX_STR_LEN];
	sprintf_s(error, "Login Error - Invalid ID !");
	strcpy_s(p.message, error);

	send_packet(user_id, &p);
}

void send_enter_packet(int user_id, int o_id)
{
	sc_packet_enter p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_ENTER;
	p.x = g_clients[o_id].x; 
	p.y = g_clients[o_id].y;
	p.level = g_clients[o_id].m_level;
	p.hp = g_clients[o_id].m_hp;
	p.maxhp = g_clients[o_id].m_maxhp;
	p.exp = g_clients[o_id].m_exp;
	p.maxexp = g_clients[o_id].m_maxexp;

	strcpy_s(p.name, g_clients[o_id].m_name);
	p.o_type = O_PLAYER;

	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_view_list.insert(o_id);
	g_clients[user_id].m_cl.unlock();

	send_packet(user_id, &p);
}

void send_leave_packet(int user_id, int o_id)
{
	sc_packet_leave p;
	p.id = o_id;
	p.size = sizeof(p);
	p.type = S2C_LEAVE;

	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_view_list.erase(o_id);
	g_clients[user_id].m_cl.unlock();

	send_packet(user_id, &p);
}

void send_stat_change_packet(int get_id, int set_id, bool isRevive = false)
{
	// 몬스터 및 플레이어 스탯 정보 변경

	sc_packet_stat_change p;
	// 플레이어 / NPC 공동 사용
	p.id = set_id;
	p.size = sizeof(p);
	p.type = S2C_CHANGE_STATS;
	p.level = g_clients[set_id].m_level;
	p.hp = g_clients[set_id].m_hp;
	p.maxhp = g_clients[set_id].m_maxhp;
	p.exp = g_clients[set_id].m_exp;
	p.att = g_clients[set_id].m_att;
	p.isRevive = isRevive;
	p.x = g_clients[set_id].x;
	p.y = g_clients[set_id].y;

	// Player만 사용
	p.maxexp = g_clients[set_id].m_maxexp;
	p.attrange = g_clients[set_id].m_attackrange;

	// NPC만 사용
	p.givenexp = g_clients[set_id].given_exp;

	send_packet(get_id, &p);
}

void send_move_packet(int user_id, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = S2C_MOVE;
	p.x = g_clients[mover].x;
	p.y = g_clients[mover].y;
	p.move_time = g_clients[mover].m_move_time;

	g_clients[mover].m_cl.lock();
	//strcpy_s(p.name, g_clients[mover].name);
	g_clients[mover].m_cl.unlock();
	//p.o_type = 0;

	send_packet(user_id, &p);
}

void send_attack_packet(int user_id, int attacker_id, int attacker_attackrange, int attacker_x, int attacker_y)
{
	sc_packet_attack p;
	p.id = attacker_id;
	p.size = sizeof(p);
	p.type = S2C_ATTACK;
	p.x = g_clients[attacker_id].x;
	p.y = g_clients[attacker_id].y;
	p.attackrange = g_clients[attacker_id].m_attackrange;

	send_packet(user_id, &p);
}

void send_chat_packet(int user_id, int chatter, char* mess, int mess_type)
{
	sc_packet_chat p;
	p.id = chatter;
	p.size = sizeof(p);
	p.type = S2C_CHAT;
	strcpy_s(p.mess, mess);
	p.mess_type = mess_type;

	send_packet(user_id, &p);
}


void activate_npc(int id)
{
	g_clients[id].is_active = true;
	C_STATUS old_state = ST_SLEEP;
	if (true == atomic_compare_exchange_strong(&g_clients[id].m_status, &old_state, ST_ACTIVE))
		add_timer(id, OP_RANDOM_MOVE, 1000);
}

void is_player_level_up(int user_id)
{
	if (g_clients[user_id].m_level == 1)	// 플레이어 LV1 -> LV2
	{
		if (g_clients[user_id].m_exp >= LEVEL1_MAX_EXP)
		{
			g_clients[user_id].m_level = 2;
			g_clients[user_id].m_exp = 0;
			g_clients[user_id].m_maxexp = LEVEL2_MAX_EXP;
			g_clients[user_id].m_attackrange = 2;
			g_clients[user_id].m_hp = PLAYER_MAX_HP;
			g_clients[user_id].m_maxhp = PLAYER_MAX_HP;
			g_clients[user_id].m_att = LEVEL2_ATT;
			cout << "Player Level Up!" << endl;
		}
	}

	if (g_clients[user_id].m_level == 2)	// 플레이어 LV2 -> LV3
	{
		if (g_clients[user_id].m_exp >= LEVEL2_MAX_EXP)
		{
			g_clients[user_id].m_level = 3;
			g_clients[user_id].m_exp = 0;
			g_clients[user_id].m_maxexp = LEVEL3_MAX_EXP;
			g_clients[user_id].m_attackrange = 3;
			g_clients[user_id].m_hp = PLAYER_MAX_HP;
			g_clients[user_id].m_maxhp = PLAYER_MAX_HP;
			g_clients[user_id].m_att = LEVEL3_ATT;
			cout << "Player Level Up!" << endl;
		}
	}

	std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
	DB_SaveCharacter(g_clients[user_id].m_id, wideName);

	send_stat_change_packet(user_id, g_clients[user_id].m_id);
	char mess[100];
	sprintf_s(mess, "[ %s ] Level [ %d ] -> Level [ %d ] !", g_clients[user_id].m_name, g_clients[user_id].m_level -1, g_clients[user_id].m_level);	// 킬 메시지 
	
	for (int i = 0; i < UserCount; ++i)
	{
		send_chat_packet(i, user_id, mess, 3);
	}
}


void is_npc_die(int user_id, int npc_id)
{
	if ((g_clients[npc_id].m_id >= NPC_ID_START) && (g_clients[npc_id].m_id < NPC2_ID_START))	// monster1 을 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += MON_LEVEL1_EXP;
				cout << "Get Level1 EXP(5) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					is_player_level_up(user_id);
				}

				if (g_clients[user_id].m_quest1_status == true)	// quest1 수령 상태일 경우
				{
					if (g_clients[user_id].m_quest1_count < 5)	// quest1 완료 조건에 미치지 못한 경우
					{
						g_clients[user_id].m_quest1_count++;	// monster1 잡은 카운트만 증가
					}
					else    // 5마리 잡은 경우
					{
						g_clients[user_id].m_quest1_complete = true;	// quest1 완료 상태로 변경
					}
				}
			}
			g_clients[npc_id].is_alive = false;
			g_clients[npc_id].is_active = false;
			cout << "Monster 1 die! " << endl;

			char mess[100];
			sprintf_s(mess, "[ ID : %s ] killed [ Monster 1 ]", g_clients[user_id].m_name);	// 킬 메시지 
			for (int i = 0; i < UserCount; ++i)
			{
				send_chat_packet(i, user_id, mess, 0);
			}
		}

	}

	if ((g_clients[npc_id].m_id >= NPC2_ID_START) && (g_clients[npc_id].m_id < NPC3_ID_START))	// monster 2를 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += MON_LEVEL2_EXP;
				cout << "Get Level2 EXP(8) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					is_player_level_up(user_id);
				}

				if (g_clients[user_id].m_quest2_status == true)	// quest2 수령 상태일 경우
				{
					if (g_clients[user_id].m_quest2_count < 5)	// quest2 완료 조건에 미치지 못한 경우
					{
						g_clients[user_id].m_quest2_count++;	// monster2 잡은 카운트만 증가
					}
					else    // 5마리 잡은 경우
					{
						g_clients[user_id].m_quest2_complete = true;	// quest2 완료 상태로 변경
					}
				}
			}
			g_clients[npc_id].is_alive = false;
			g_clients[npc_id].is_active = false;

			char mess[100];
			sprintf_s(mess, "[ ID : %s ] killed [ Monster 2]", g_clients[user_id].m_name);	// 킬 메시지 
			for (int i = 0; i < UserCount; ++i)
			{
				send_chat_packet(i, user_id, mess, 0);
			}
		}
	}

	if ((g_clients[npc_id].m_id >= NPC3_ID_START) && (g_clients[npc_id].m_id < (NUM_NPC + MAX_USER)))	// monster 3를 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += MON_LEVEL3_EXP;
				cout << "Get Level3 EXP(10) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					is_player_level_up(user_id);
				}

				if (g_clients[user_id].m_quest3_status == true)	// quest1 수령 상태일 경우
				{
					if (g_clients[user_id].m_quest3_count < 5)	// quest1 완료 조건에 미치지 못한 경우
					{
						g_clients[user_id].m_quest3_count++;	// monster1 잡은 카운트만 증가
					}
					else    // 5마리 잡은 경우
					{
						g_clients[user_id].m_quest3_complete = true;	// quest1 완료 상태로 변경
					}
				}
			}
			g_clients[npc_id].is_alive = false;
			g_clients[npc_id].is_active = false;

			char mess[100];
			sprintf_s(mess, "[ ID : %s ] killed [ Boss Monster]", g_clients[user_id].m_name);	// 킬 메시지 
			for (int i = 0; i < UserCount; ++i)
			{
				send_chat_packet(i, user_id, mess, 0);
			}
		}
	}

	send_stat_change_packet(user_id, g_clients[user_id].m_id);
}

void check_user_hit(int id)	// 유저가 움직여서 부딪히는 경우
{
	g_clients[id].m_cl.lock();
	auto vl = g_clients[id].m_view_list;
	g_clients[id].m_cl.unlock();

	for (auto npc : vl)		//viewlist 안의 npc 전체
	{
		if (g_clients[npc].m_id >= NPC_ID_START && g_clients[npc].m_id < QUEST_NPC_NUMBER)	// monster일 경우
		{
			if (g_clients[id].x == g_clients[npc].x && g_clients[id].y == g_clients[npc].y &&
				g_clients[id].m_hp > 0 && g_clients[npc].m_hp > 0)
			{
				g_clients[id].m_hp -= g_clients[npc].m_att;

				if (g_clients[id].m_hp <= 0)
				{
					g_clients[id].m_hp = 0;

					// 본인에게 부활 여부 확인용 메시지 보내기
					char mess[100];
					sprintf_s(mess, "You died! Press 'R' to respawn. (Quit : Q)");

					send_chat_packet(g_clients[id].m_id, id, mess, 4);

					// 전체 클라이언트에게 Player die 전송
					char mess2[100];
					sprintf_s(mess2, "[ %s ] Die !", g_clients[id].m_name);

					for (int j = 0; j < UserCount; ++j)
					{
						send_chat_packet(j, g_clients[id].m_id, mess2, 5);
					}
				}

				g_clients[id].m_cl.lock();
				auto vl = g_clients[id].m_view_list;
				g_clients[id].m_cl.unlock();

				// 내 주변에게 알림
				for (auto user : vl)
				{
					if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
					{
						send_stat_change_packet(user, g_clients[id].m_id);
					}
				}
				// 나에게 알림
				send_stat_change_packet(id, g_clients[id].m_id);
			}
		}
	}
}

void check_monster_hit(int id) // Monster가 이동해서 부딪힐 경우
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (g_clients[i].m_status == ST_ACTIVE)
		{
			if (g_clients[id].x == g_clients[i].x && g_clients[id].y == g_clients[i].y &&
				g_clients[i].m_hp > 0 && g_clients[id].m_hp > 0)
			{
				g_clients[i].m_hp -= g_clients[id].m_att;

				if (g_clients[i].m_hp <= 0)
				{
					g_clients[i].m_hp = 0;

					// 본인에게 부활 여부 확인용 메시지 보내기
					char mess[100];
					sprintf_s(mess, "You died! Press 'R' to respawn. (Quit : Q)");

					send_chat_packet(g_clients[i].m_id, i, mess, 4);

					// 전체 클라이언트에게 Player die 전송
					char mess2[100];
					sprintf_s(mess2, "[ %s ] Die !", g_clients[i].m_name);

					for (int j = 0; j < UserCount; ++j)
					{
						send_chat_packet(j, id, mess2, 5);
					}
				}

				g_clients[i].m_cl.lock();
				auto vl = g_clients[i].m_view_list;
				g_clients[i].m_cl.unlock();

				// 내 주변에게 알림
				for (auto user : vl)
				{
					if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
					{
						send_stat_change_packet(user, g_clients[i].m_id);
					}
				}
				
				// 나에게 알림
				send_stat_change_packet(i, g_clients[i].m_id);
			}
		}
	}
}



void do_move(int user_id, int direction)
{
	CLIENT& u = g_clients[user_id];
	int x = u.x;
	int y = u.y;
	switch (direction) {
	case D_UP:
		//if (y > 0) y--; break;
		if (g_Map[x][y - 1] == eBLANK)
		{
			if (y > 0) y--; break;
		}
		break;
	case D_DOWN:
		//if (y < (WORLD_HEIGHT - 1)) y++; break;
		if (g_Map[x][y + 1] == eBLANK)
		{
			if (y < (WORLD_HEIGHT - 1)) y++; break;
		}
		break;
	case D_LEFT:
		//if (x > 0) x--; break;
		if (g_Map[x - 1][y] == eBLANK)
		{
			if (x > 0) x--; break;
		}
		break;
	case D_RIGHT:
		//if (x < (WORLD_WIDTH - 1)) x++; break;
		if (g_Map[x + 1][y] == eBLANK)
		{
			if (x < (WORLD_WIDTH - 1)) x++; break;
		}
		break;
	default:
		cout << "Unknown Direction from Client move packet!\n";
		DebugBreak();
		exit(-1);
	}
	u.x = x;
	u.y = y;

	g_clients[user_id].m_cl.lock();
	unordered_set<int> old_vl = g_clients[user_id].m_view_list;
	g_clients[user_id].m_cl.unlock();
	unordered_set<int> new_vl;	// 시야처리를 위한 viewlist를 set컨테이너로 선언
	for (auto &cl : g_clients)	// 모든 클라이언트에 대해
	{
		if (false == is_near(cl.m_id, user_id)) continue;
		if (ST_SLEEP == cl.m_status) 	activate_npc(cl.m_id);
		if (ST_ACTIVE != cl.m_status) continue;
		if (cl.m_id == user_id) continue;
		if (false == is_player(cl.m_id))
		{
			EXOVER* over = new EXOVER;
			over->op = OP_PLAYER_MOVE;
			over->p_id = user_id;
			PostQueuedCompletionStatus(g_iocp, 1, cl.m_id, &over->over);
		}
		new_vl.insert(cl.m_id);
		send_stat_change_packet(cl.m_id, g_clients[user_id].m_id);
	}
	send_move_packet(user_id, user_id);	// 나에게 내가 이동했다고 알려주어야 함. (밑에서는 알려주지 않음)

	for (auto np : new_vl)	// np : new player
	{
		if (0 == old_vl.count(np))	// 이동하며 새로 보게 된 플레이어에 대한 처리 (Object가 새로 시야에 들어왔을 때)
		{
			send_enter_packet(user_id, np);
			if (false == is_player(np))	continue;
			g_clients[np].m_cl.lock();
			if (0 == g_clients[np].m_view_list.count(user_id))	// 상대 viewlist에 내가 없을때
			{
				g_clients[np].m_cl.unlock();
				send_enter_packet(np, user_id);
			}
			else    // 상대 viewlist에 내가 있을 때
			{
				g_clients[np].m_cl.unlock();
				send_move_packet(np, user_id);
			}
		}
		else    // 이동하며 계속 보고 있었던 플레이어에 대한 처리 (계속 시야에 존재하고 있을 때)
		{
			if (false == is_player(np)) continue;
			g_clients[np].m_cl.lock();
			if (0 != g_clients[np].m_view_list.count(user_id))
			{
				g_clients[np].m_cl.unlock();
				send_move_packet(np, user_id);
			}
			else  // 상대방의 시야에 내가 있는게 아니라면
			{
				g_clients[np].m_cl.unlock();
				send_enter_packet(np, user_id);
			}
		}
	}

	for (auto old_p : old_vl)		// Object가 시야에서 벗어났을 때
	{
		if (0 == new_vl.count(old_p))
		{
			send_leave_packet(user_id, old_p);
			if (false == is_player(old_p)) continue;
			g_clients[old_p].m_cl.lock();
			if (0 != g_clients[old_p].m_view_list.count(user_id))
			{
				g_clients[old_p].m_cl.unlock();
				send_leave_packet(old_p, user_id);
			}
			else
			{
				g_clients[old_p].m_cl.unlock();
			}
		}
	}

	check_user_hit(user_id);
}

void random_move_npc(int id)
{
	int x = g_clients[id].x;
	int y = g_clients[id].y;
	switch (rand() % 4)
	{
	case D_UP:
		//if (y > 0) y--; break;
		if (g_Map[x][y - 1] == eBLANK)
		{
			if (y > 0) y--; break;
		}
		break;
	case D_DOWN:
		//if (y < (WORLD_HEIGHT - 1)) y++; break;
		if (g_Map[x][y + 1] == eBLANK)
		{
			if (y < (WORLD_HEIGHT - 1)) y++; break;
		}
		break;
	case D_LEFT:
		//if (x > 0) x--; break;
		if (g_Map[x - 1][y] == eBLANK)
		{
			if (x > 0) x--; break;
		}
		break;
	case D_RIGHT:
		//if (x < (WORLD_WIDTH - 1)) x++; break;
		if (g_Map[x + 1][y] == eBLANK)
		{
			if (x < (WORLD_WIDTH - 1)) x++; break;
		}
		break;
	}
	g_clients[id].x = x;
	g_clients[id].y = y;

	for (int i = 0; i < NPC_ID_START; ++i)
	{
		if (ST_ACTIVE != g_clients[i].m_status) continue;
		if (true == is_near(i, id))
		{
			g_clients[i].m_cl.lock();
			if (0 != g_clients[i].m_view_list.count(id))
			{
				g_clients[i].m_cl.unlock();
				send_move_packet(i, id);
			}
			else
			{
				g_clients[i].m_cl.unlock();
				send_enter_packet(i, id);
			}
		}
		else
		{
			g_clients[i].m_cl.lock();
			if (0 != g_clients[i].m_view_list.count(id))
			{
				g_clients[i].m_cl.unlock();
				send_leave_packet(i, id);
			}
			else
				g_clients[i].m_cl.unlock();
		}
	}

	check_monster_hit(id);
}

void enter_game(int user_id, char name[])
{
	g_clients[user_id].m_cl.lock();
	strcpy_s(g_clients[user_id].m_name, name);
	g_clients[user_id].m_name[MAX_ID_LEN] = NULL;
	
	///////////////////////////////////////////////////////
	// DB 로드 없이 실행 할 때 (주석 처리)
	std::wstring wideName = CharArrayToWString(name);
	DB_LoadCharacter(user_id, wideName);
	///////////////////////////////////////////////////////
	send_login_ok_packet(user_id);
	UserCount++;

	// 접속시간 저장 및 타이머 등록
	g_clients[user_id].m_last_connect_time = high_resolution_clock::now();
	add_timer(user_id, OP_CHECK_CONNECT_TIME, PERIODICALLY_SAVE_TIME);

	g_clients[user_id].m_status = ST_ACTIVE;
	g_clients[user_id].m_cl.unlock();


	for (auto &cl : g_clients)
	{
		int i = cl.m_id;
		if (user_id == i) continue;
		if (true == is_near(user_id, i))
		{
			//g_clients[i].m_cl.lock();
			if (ST_SLEEP == g_clients[i].m_status)
			{
				activate_npc(i);
			}
			if (ST_ACTIVE == g_clients[i].m_status)
			{
				send_enter_packet(user_id, i);
				if (true == is_player(i))
					send_enter_packet(i, user_id);
			}
			//g_clients[i].m_cl.unlock();
			send_stat_change_packet(i, g_clients[user_id].m_id);
		}
	}
}

void do_attack(int id)
{
	g_clients[id].m_cl.lock();
	auto vl = g_clients[id].m_view_list;
	g_clients[id].m_cl.unlock();

	// 내가 공격했음을 내 주변 유저에게 알림
	for (auto user : vl)
	{
		if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
		{
			send_attack_packet(g_clients[user].m_id, g_clients[id].m_id, g_clients[id].m_attackrange, g_clients[id].x, g_clients[id].y);
		}
	}

	// 나에게 알림
	send_attack_packet(g_clients[id].m_id, g_clients[id].m_id, g_clients[id].m_attackrange, g_clients[id].x, g_clients[id].y);

	char mess[100];

	for (auto npc : vl) {

		if (g_clients[npc].m_id >= NPC_ID_START && g_clients[npc].m_id < QUEST_NPC_NUMBER)	// monster일 경우
		{
			if ((g_clients[npc].x == g_clients[id].x && g_clients[npc].y == g_clients[id].y - 1) ||
				(g_clients[npc].x == g_clients[id].x && g_clients[npc].y == g_clients[id].y + 1) ||
				(g_clients[npc].x == g_clients[id].x - 1 && g_clients[npc].y == g_clients[id].y) ||
				(g_clients[npc].x == g_clients[id].x + 1 && g_clients[npc].y == g_clients[id].y))		// 시야 안의 npc 중 1칸 이내 범위 판정
			{
				g_clients[npc].m_hp -= g_clients[id].m_att;
				cout << "Attack !" << endl;
				is_npc_die(id, npc);

				char mess[100];
				sprintf_s(mess, "[ %s ] -> Attack -> [ %s ] ( %d Damage ).", g_clients[id].m_name, g_clients[npc].m_name, g_clients[id].m_att);


				// 내 주변 유저에게 BroadCast
				for (auto user : vl) 
				{
					if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
					{
						send_chat_packet(g_clients[user].m_id, id, mess, 1);
						send_stat_change_packet(g_clients[user].m_id, npc);
					}
				}

				// 나에게 알림
				send_chat_packet(g_clients[id].m_id, id, mess, 1);
				send_stat_change_packet(g_clients[id].m_id, npc);
			}
		}
	}

}

void do_revive(int id)
{
	g_clients[id].m_hp = g_clients[id].m_maxhp;
	g_clients[id].m_exp /= 2;
	g_clients[id].x = ((rand() % WORLD_WIDTH) % 6) + 11;
	g_clients[id].y = ((rand() % WORLD_HEIGHT) % 6) + 16;

	g_clients[id].m_cl.lock();
	auto vl = g_clients[id].m_view_list;
	g_clients[id].m_cl.unlock();

	char mess[100];

	// 나한테 주변 클라이언트 정보를 알려줌
	for (auto &cl : g_clients)
	{
		int i = cl.m_id;
		if (id == i) continue;
		if (true == is_near(id, i))
		{
			send_enter_packet(id, i);
			send_enter_packet(i, id);
		}
	}

	// 내 주변 유저에게 BroadCast
	for (auto user : vl)
	{
		if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
		{
			//send_chat_packet(g_clients[user].m_id, id, mess, 1);
			send_stat_change_packet(g_clients[user].m_id, id, true);
		}
	}
	
	// 나에게 알림
	//send_chat_packet(g_clients[id].m_id, id, mess, 4);
	send_stat_change_packet(g_clients[id].m_id, id, true);
}

void process_packet(int user_id, char* buf)
{
	switch (buf[1]) {
	case C2S_LOGIN: {
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
		switch (packet->loginType)
		{
		case 1:
		{
			std::wstring wideName = CharArrayToWString(packet->name);
			DB_CreateCharacter(wideName);
		}
			break;

		case 2:
			enter_game(user_id, packet->name);
			break;
		}
	}
					break;
	case C2S_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		g_clients[user_id].m_move_time = packet->move_time;
		do_move(user_id, packet->direction);
	}
				   break;

	case C2S_ATTACK: {
		cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);
		do_attack(user_id);
	}
					 break;

	case C2S_REVIVE: {
		cs_packet_revive* packet = reinterpret_cast<cs_packet_revive*>(buf);
		do_revive(user_id);
	}
					 break;

	default:
		cout << "Unknown Packet Type Error!\n";
		DebugBreak();
		exit(-1);
	}
}


void disconnect(int user_id)
{
	send_leave_packet(user_id, user_id);

	// TODO. SaveCharacter DB
	// g_clients[user_id] > DB 테이블안에 들어가면 댐
	// EXEC Game.dbo.SaveCharacter '%s', g_client[user_id].name
	std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
	DB_SaveCharacter(g_clients[user_id].m_id, wideName);

	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_status = ST_ALLOC;
	closesocket(g_clients[user_id].m_s);
	for (int i = 0; i < NPC_ID_START; ++i)
	{
		CLIENT &cl = g_clients[i];
		if (user_id == cl.m_id) continue;
		//cl.m_cl.lock();
		if (ST_ACTIVE == cl.m_status)
			send_leave_packet(cl.m_id, user_id);
		//cl.m_cl.unlock();
	}
	g_clients[user_id].m_status = ST_FREE;
	g_clients[user_id].m_cl.unlock();
}

void recv_packet_construct(int user_id, int io_byte)
{
	CLIENT& cu = g_clients[user_id];
	EXOVER& r_o = cu.m_recv_over;

	int rest_byte = io_byte;
	char* p = r_o.io_buf;
	int packet_size = 0;
	if (0 != cu.m_prev_size) packet_size = cu.m_packet_buf[0];
	while (rest_byte > 0) {
		if (0 == packet_size) packet_size = *p;
		if (packet_size <= rest_byte + cu.m_prev_size) {
			memcpy(cu.m_packet_buf + cu.m_prev_size, p, packet_size - cu.m_prev_size);
			p += packet_size - cu.m_prev_size;
			rest_byte -= packet_size - cu.m_prev_size;
			packet_size = 0;
			process_packet(user_id, cu.m_packet_buf);
			cu.m_prev_size = 0;
		}
		else {
			memcpy(cu.m_packet_buf + cu.m_prev_size, p, rest_byte);
			cu.m_prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void worker_thread()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		CLIENT& cl = g_clients[user_id];

		switch (exover->op) {
		case OP_RECV:
			if (0 == io_byte) disconnect(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_s, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			if (0 == io_byte) disconnect(user_id);
			delete exover;
			break;
		case OP_ACCEPT: {
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i) {
				lock_guard<mutex> gl{ g_clients[i].m_cl };
				if (ST_FREE == g_clients[i].m_status) {
					g_clients[i].m_status = ST_ALLOC;
					user_id = i;
					break;
				}
			}

			SOCKET c_socket = exover->c_socket;
			if (-1 == user_id)
				closesocket(c_socket);
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, user_id, 0);
				CLIENT& nc = g_clients[user_id];
				nc.m_prev_size = 0;
				nc.m_recv_over.op = OP_RECV;
				ZeroMemory(&nc.m_recv_over.over, sizeof(nc.m_recv_over.over));
				nc.m_recv_over.wsabuf.buf = nc.m_recv_over.io_buf;
				nc.m_recv_over.wsabuf.len = MAX_BUF_SIZE;
				nc.m_s = c_socket;
				nc.x = ((rand() % WORLD_WIDTH) % 6) + 11;
				nc.y = ((rand() % WORLD_HEIGHT) % 6) + 16;
				nc.m_view_list.clear();
				DWORD flags = 0;
				WSARecv(c_socket, &nc.m_recv_over.wsabuf, 1, NULL, &flags, &nc.m_recv_over.over, NULL);
			}
			c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_socket = c_socket;
			ZeroMemory(&exover->over, sizeof(exover->over));
			AcceptEx(l_socket, c_socket, exover->io_buf, NULL,
				sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}
						break;
		case OP_RANDOM_MOVE: {
			random_move_npc(user_id);
			bool keep_alive = false;
			for (int i = 0; i < NPC_ID_START; ++i)
				if (true == is_near(user_id, i))
					if (ST_ACTIVE == g_clients[i].m_status)
					{
						keep_alive = true;
						break;
					}
			if (true == keep_alive) add_timer(user_id, OP_RANDOM_MOVE, 1000);
			else g_clients[user_id].m_status = ST_SLEEP;
			delete exover;
		}
							 break;
		case OP_PLAYER_MOVE: {
			g_clients[user_id].lua_l.lock();
			lua_State *L = g_clients[user_id].L;
			if (user_id == QUEST_NPC_NUMBER)
			{
				lua_getglobal(L, "quest_accept");
				lua_pushnumber(L, exover->p_id);
			}
			else
			{
				lua_getglobal(L, "event_player_move");
				lua_pushnumber(L, exover->p_id);
			}
			int error = lua_pcall(L, 1, 0, 0);
			if (error) cout << lua_tostring(L, -1);
			g_clients[user_id].lua_l.unlock();
			delete exover;
		}
							 break;
		default:
			cout << "Unknown Operation in worker_thread!\n";
			while (true);

		}
	}
}

int API_SendMessage(lua_State* L)
{
	int my_id = (int)lua_tointeger(L, -3);
	int user_id = (int)lua_tointeger(L, -2);
	char *mess = (char*)lua_tostring(L, -1);

	send_chat_packet(user_id, my_id, mess, 2);
	lua_pop(L, 3);
	return 0;
}

int API_get_x(lua_State* L)
{
	int obj_id = (int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	int x = g_clients[obj_id].x;
	lua_pushnumber(L, x);
	return 1;
}

int API_get_y(lua_State* L)
{
	int obj_id = (int)lua_tointeger(L, -1);
	lua_pop(L, 2);
	int y = g_clients[obj_id].y;
	lua_pushnumber(L, y);
	return 1;
}

void initialize_clients()
{
	for (int i = 0; i < NPC_ID_START; ++i)	// player 초기 값 설정
	{
		g_clients[i].m_id = i;
		g_clients[i].m_level = 1;
		g_clients[i].m_status = ST_FREE;
		g_clients[i].m_att = LEVEL1_ATT;
		g_clients[i].m_attackrange = 1;
		g_clients[i].m_maxexp = LEVEL1_MAX_EXP;
		g_clients[i].m_maxhp = PLAYER_MAX_HP;
		g_clients[i].m_hp = PLAYER_MAX_HP;
	}

	for (int i = NPC_ID_START; i < NPC2_ID_START; ++i)	// monster1 초기 값 설정
	{
		g_clients[i].m_id = i;
		g_clients[i].m_level = 1;
		g_clients[i].m_status = ST_SLEEP;
		g_clients[i].m_hp = MON_LEVEL1_HP;
		g_clients[i].m_maxhp = MON_LEVEL1_HP;
		g_clients[i].m_att = MON_LEVEL1_ATT;
		g_clients[i].given_exp = MON_LEVEL1_EXP;
	}

	for (int i = NPC2_ID_START; i < NPC3_ID_START; ++i)	// monster2 초기 값 설정
	{
		g_clients[i].m_id = i;
		g_clients[i].m_level = 2;
		g_clients[i].m_status = ST_SLEEP;
		g_clients[i].m_hp = MON_LEVEL2_HP;
		g_clients[i].m_maxhp = MON_LEVEL2_HP;
		g_clients[i].m_att = MON_LEVEL2_ATT;
		g_clients[i].given_exp = MON_LEVEL2_EXP;
	}

	for (int i = NPC3_ID_START; i < NUM_NPC + MAX_USER; ++i)	// monster3(bossmonster) 초기 값 설정
	{
		g_clients[i].m_id = i;
		g_clients[i].m_level = 3;
		g_clients[i].m_status = ST_SLEEP;
		g_clients[i].m_hp = MON_LEVEL3_HP;
		g_clients[i].m_maxhp = MON_LEVEL3_HP;
		g_clients[i].m_att = MON_LEVEL3_ATT;
		g_clients[i].given_exp = MON_LEVEL3_EXP;
	}

	// Quest NPC
	g_clients[QUEST_NPC_NUMBER].m_id = QUEST_NPC_NUMBER;
	g_clients[QUEST_NPC_NUMBER].m_status = ST_ACTIVE;
	g_clients[QUEST_NPC_NUMBER].m_hp = MON_LEVEL3_HP;
	g_clients[QUEST_NPC_NUMBER].m_maxhp = MON_LEVEL3_HP;
	g_clients[QUEST_NPC_NUMBER].m_att = MON_LEVEL3_ATT;
	g_clients[QUEST_NPC_NUMBER].given_exp = MON_LEVEL3_EXP;

	//for (int i = 0; i < MAX_USER; ++i) {
	//	g_clients[i].m_id = i;

	//	g_clients[i].m_status = ST_FREE;
	//}
}

void init_map()
{
	for (int i = 0; i < WORLD_WIDTH; ++i) {
		for (int j = 0; j < WORLD_HEIGHT; ++j) {
			if (uid(dre) == 0)
				g_Map[i][j] = eBLOCKED;
			else
				g_Map[i][j] = eBLANK;
		}
	}
}

void init_npc()
{
	for (int i = NPC_ID_START; i < NPC_ID_START + NUM_NPC; ++i)
	{
		g_clients[i].m_s = 0;
		g_clients[i].m_id = i;

		g_clients[i].is_active = true;
		g_clients[i].is_alive = true;
		g_clients[i].m_status = ST_SLEEP;
		g_clients[i].x = rand() % WORLD_WIDTH;
		g_clients[i].y = rand() % WORLD_HEIGHT;

		if (i >= NPC_ID_START && i < NPC2_ID_START)
		{
			sprintf_s(g_clients[i].m_name, "MONSTER 1");
		}

		else if (i >= NPC2_ID_START && i < NPC3_ID_START)
		{
			sprintf_s(g_clients[i].m_name, "MONSTER 2");
		}

		else
		{
			sprintf_s(g_clients[i].m_name, "MONSTER 3");
		}

		////g_clients[i].m_last_move_time = high_resolution_clock::now();
		////add_timer(i, OP_RANDOM_MOVE, 1000);

		lua_State *L = g_clients[i].L = luaL_newstate();
		luaL_openlibs(L);
		luaL_loadfile(L, "NPC.LUA");
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, "set_uid");
		lua_pushnumber(L, i);
		lua_pcall(L, 1, 0, 0);
		lua_pop(L, 1);

		lua_register(L, "API_send_message", API_SendMessage);
		lua_register(L, "API_get_x", API_get_x);
		lua_register(L, "API_get_y", API_get_y);
	}


	//// Quest NPC
	g_clients[QUEST_NPC_NUMBER].m_s = 0;
	g_clients[QUEST_NPC_NUMBER].m_id = QUEST_NPC_NUMBER;
	sprintf_s(g_clients[QUEST_NPC_NUMBER].m_name, "QUEST_NPC");
	g_clients[QUEST_NPC_NUMBER].m_status = ST_ACTIVE;
	g_clients[QUEST_NPC_NUMBER].x = 15;
	g_clients[QUEST_NPC_NUMBER].y = 15;

	lua_State *L = g_clients[QUEST_NPC_NUMBER].L = luaL_newstate();
	luaL_openlibs(L);
	luaL_loadfile(L, "NPC.LUA");
	lua_pcall(L, 0, 0, 0);
	lua_getglobal(L, "set_uid");
	lua_pushnumber(L, QUEST_NPC_NUMBER);
	lua_pcall(L, 1, 0, 0);
	lua_pop(L, 1);

	lua_register(L, "API_send_message", API_SendMessage);
	lua_register(L, "API_get_x", API_get_x);
	lua_register(L, "API_get_y", API_get_y);

}

void initialize_clients_DB(int id, int x, int y)
{
	g_clients[id].x = x;
	g_clients[id].y = y;
}

void Periodically_Save(int user_id) 
{
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - g_clients[user_id].m_last_connect_time);
	if (duration.count() >= PERIODICALLY_SAVE_TIME)
	{
		// 일정 주기 마다 호출할 함수
		std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
		DB_SaveCharacter(g_clients[user_id].m_id, wideName);

		// 마지막 호출 시간을 갱신
		g_clients[user_id].m_last_connect_time = now;
	}
}

void do_timer()
{
	while (true)
	{
		this_thread::sleep_for(1ms);
		while (true)
		{
			timer_lock.lock();
			if (true == timer_queue.empty())
			{
				timer_lock.unlock();
				break;
			}
			if (timer_queue.top().wakeup_time > high_resolution_clock::now())
			{
				timer_lock.unlock();
				break;
			}
			event_type ev = timer_queue.top();
			timer_queue.pop();
			timer_lock.unlock();
			switch (ev.event_id)
			{
			case OP_RANDOM_MOVE:
				{
					EXOVER *over = new EXOVER;
					over->op = ev.event_id;
					PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
					//random_move_npc(ev.obj_id);
					//add_timer(ev.obj_id, ev.event_id, 1000);
				}
				break;

			case OP_CHECK_CONNECT_TIME:
				{
					Periodically_Save(ev.obj_id);
					// 180초(3분) 후 다시 타이머 추가
					add_timer(ev.obj_id, OP_CHECK_CONNECT_TIME, PERIODICALLY_SAVE_TIME);
				}
				break;
			}
		}
	}
}

int main()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);

	cout << "NPC Initialization start.\n";
	init_npc();
	cout << "NPC Initialization finished.\n";
	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr *>(&s_address), sizeof(s_address));

	listen(l_socket, SOMAXCONN);

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	init_map();
	initialize_clients();

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over.over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_socket = c_socket;
	AcceptEx(l_socket, c_socket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);

	vector <thread> worker_threads;
	for (int i = 0; i < 4; ++i) worker_threads.emplace_back(worker_thread);

	thread timer_thread{ do_timer };
	for (auto& th : worker_threads) th.join();
}