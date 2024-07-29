#include "Datas.h"
#include "PacketManager.h"
#include "DBConstants.h"
#include "DatabaseManager.h"

#pragma comment (lib, "WS2_32.lib")
#pragma comment (lib, "mswsock.lib")
#pragma comment (lib, "lua53.lib")

#include "protocol.h"
#include "EnumList.h"


using namespace std;
using namespace chrono;

priority_queue<EventType> timer_queue;
mutex timer_lock;

Client g_clients[MAX_USER + NUM_NPC + 1];
HANDLE g_iocp;
SOCKET l_socket;
atomic_int UserCount = 0;

// 타일맵 구성을 위한 이차원 배열 선언
char g_Map[WORLD_WIDTH][WORLD_HEIGHT];

// 랜덤 엔진 및 분포 정의
std::default_random_engine dre{ 9999 };
std::uniform_int_distribution<> uid{ 0, 5 };

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
void AddTimer(int obj_id, ENUMOP op_type, int duration)
{
	timer_lock.lock();
	EventType ev{ obj_id, 0, op_type, high_resolution_clock::now() + milliseconds(duration)};
	timer_queue.push(ev);
	timer_lock.unlock();
}

bool IsPlayer(int id)
{
	return id < NPC_ID_START;
}

bool IsNear(int a, int b)	// 시야 확인
{
	if (abs(g_clients[a].x - g_clients[b].x) > VIEW_RADIUS) return false;	// 시야를 벗어나게 되면 false 리턴 (x)
	if (abs(g_clients[a].y - g_clients[b].y) > VIEW_RADIUS) return false;	// 시야를 벗어나게 되면 false 리턴 (y)
	return true;
}

void ActivateNPC(int id)
{
	g_clients[id].is_active = true;
	ObjectStatus old_state = ObjectStatus::OS_Sleep;
	if (true == atomic_compare_exchange_strong(&g_clients[id].m_status, &old_state, ObjectStatus::OS_Active))
		AddTimer(id, ENUMOP::OP_RANDOM_MOVE, 1000);
}

void PlayerLevelUp(int user_id)
{
	if (g_clients[user_id].m_level == 1)	// 플레이어 LV1 -> LV2
	{
		if (g_clients[user_id].m_exp >= static_cast<int>(PlayerMaxExp::PME_Level1))
		{
			g_clients[user_id].m_level = 2;
			g_clients[user_id].m_exp = 0;
			g_clients[user_id].m_maxexp = static_cast<int>(PlayerMaxExp::PME_Level2);
			g_clients[user_id].m_attackrange = 2;
			g_clients[user_id].m_hp = PLAYER_MAX_HP;
			g_clients[user_id].m_maxhp = PLAYER_MAX_HP;
			g_clients[user_id].m_att = static_cast<int>(PlayerAttackData::PAD_Level2);
			cout << "Player Level Up!" << endl;
		}
	}

	if (g_clients[user_id].m_level == 2)	// 플레이어 LV2 -> LV3
	{
		if (g_clients[user_id].m_exp >= static_cast<int>(PlayerMaxExp::PME_Level2))
		{
			g_clients[user_id].m_level = 3;
			g_clients[user_id].m_exp = 0;
			g_clients[user_id].m_maxexp = static_cast<int>(PlayerMaxExp::PME_Level3);
			g_clients[user_id].m_attackrange = 3;
			g_clients[user_id].m_hp = PLAYER_MAX_HP;
			g_clients[user_id].m_maxhp = PLAYER_MAX_HP;
			g_clients[user_id].m_att = static_cast<int>(PlayerAttackData::PAD_Level3);
			cout << "Player Level Up!" << endl;
		}
	}

	std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
	DB::DB_SaveCharacter(g_clients, g_clients[user_id].m_id, wideName, DSN_NAME);

	PacketManager::SendStatChangePacket(g_clients, user_id, g_clients[user_id].m_id);

	char mess[100];
	sprintf_s(mess, "[ %s ] Level [ %d ] -> Level [ %d ] !", g_clients[user_id].m_name, g_clients[user_id].m_level - 1, g_clients[user_id].m_level);	// 킬 메시지 

	for (int i = 0; i < UserCount; ++i)
	{
		PacketManager::SendChatPacket(g_clients, i, user_id, mess, 3);
	}
}


void IsNPCDead(int user_id, int npc_id)
{
	if (g_clients[npc_id].objectType==ObjectType::OT_Monster1)	// monster1 을 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += static_cast<int>(MonsterGivenExp::MGE_Level1);
				cout << "Get Level1 EXP(5) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					PlayerLevelUp(user_id);
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
				PacketManager::SendChatPacket(g_clients, i, user_id, mess, 0);
			}
		}

	}

	if ((g_clients[npc_id].m_id >= NPC2_ID_START) && (g_clients[npc_id].m_id < NPC3_ID_START))	// monster 2를 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += static_cast<int>(MonsterGivenExp::MGE_Level2);
				cout << "Get Level2 EXP(8) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					PlayerLevelUp(user_id);
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
				PacketManager::SendChatPacket(g_clients, i, user_id, mess, 0);
			}
		}
	}

	if ((g_clients[npc_id].m_id >= NPC3_ID_START) && (g_clients[npc_id].m_id < (NUM_NPC + MAX_USER)))	// monster 3를 잡았을 때,
	{
		if ((g_clients[npc_id].m_hp <= 0) && (g_clients[npc_id].is_active == true))
		{
			if (g_clients[npc_id].is_alive == true)
			{
				g_clients[user_id].m_exp += static_cast<int>(MonsterGivenExp::MGE_Level3);
				cout << "Get Level3 EXP(10) !" << endl;

				if (g_clients[user_id].m_exp >= g_clients[user_id].m_maxexp)
				{
					PlayerLevelUp(user_id);
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
				PacketManager::SendChatPacket(g_clients, i, user_id, mess, 0);
			}
		}
	}
	PacketManager::SendStatChangePacket(g_clients, user_id, g_clients[user_id].m_id);
}

void CheckUserHit(int id)	// 유저가 움직여서 부딪히는 경우
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

					PacketManager::SendChatPacket(g_clients, g_clients[id].m_id, id, mess, 4);

					// 전체 클라이언트에게 Player die 전송
					char mess2[100];
					sprintf_s(mess2, "[ %s ] Die !", g_clients[id].m_name);

					for (int j = 0; j < UserCount; ++j)
					{
						PacketManager::SendChatPacket(g_clients, j, g_clients[id].m_id, mess2, 5);
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
						PacketManager::SendStatChangePacket(g_clients, user, g_clients[id].m_id);
					}
				}
				// 나에게 알림
				PacketManager::SendStatChangePacket(g_clients, id, g_clients[id].m_id);
			}
		}
	}
}

void CheckMonsterHit(int id) // Monster가 이동해서 부딪힐 경우
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (g_clients[i].m_status == ObjectStatus::OS_Active)
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

					PacketManager::SendChatPacket(g_clients, g_clients[i].m_id, i, mess, 4);

					// 전체 클라이언트에게 Player die 전송
					char mess2[100];
					sprintf_s(mess2, "[ %s ] Die !", g_clients[i].m_name);

					for (int j = 0; j < UserCount; ++j)
					{
						PacketManager::SendChatPacket(g_clients, j, id, mess2, 5);
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
						PacketManager::SendStatChangePacket(g_clients, user, g_clients[i].m_id);
					}
				}

				// 나에게 알림
				PacketManager::SendStatChangePacket(g_clients, i, g_clients[i].m_id);
			}
		}
	}
}



void DoMove(int user_id, int direction)
{
	Client& u = g_clients[user_id];
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
		if (false == IsNear(cl.m_id, user_id)) continue;
		if (ObjectStatus::OS_Sleep == cl.m_status) 	ActivateNPC(cl.m_id);
		if (ObjectStatus::OS_Active != cl.m_status) continue;
		if (cl.m_id == user_id) continue;
		if (false == IsPlayer(cl.m_id))
		{
			ExOver* over = new ExOver;
			over->op = ENUMOP::OP_PLAYER_MOVE;
			over->p_id = user_id;
			PostQueuedCompletionStatus(g_iocp, 1, cl.m_id, &over->over);
		}
		new_vl.insert(cl.m_id);

		PacketManager::SendStatChangePacket(g_clients, cl.m_id, g_clients[user_id].m_id);
	}

	PacketManager::SendMovePacket(g_clients, user_id, user_id);

	for (auto np : new_vl)	// np : new player
	{
		if (0 == old_vl.count(np))	// 시야에 새로 들어온 오브젝트에 대한 처리
		{
			PacketManager::SendEnterPacket(g_clients, user_id, np);

			if (false == IsPlayer(np))	continue;
			g_clients[np].m_cl.lock();
			if (0 == g_clients[np].m_view_list.count(user_id))	// 상대방 viewlist에 존재하지 않으면
			{
				g_clients[np].m_cl.unlock();
				PacketManager::SendEnterPacket(g_clients, np, user_id);
			}
			else    // 상대방 viewlist에 존재할 때
			{
				g_clients[np].m_cl.unlock();

				PacketManager::SendMovePacket(g_clients, np, user_id);
			}
		}
		else    // 시야에 머물고 있는 오브젝트에 대한 처리
		{
			if (false == IsPlayer(np)) continue;
			g_clients[np].m_cl.lock();
			if (0 != g_clients[np].m_view_list.count(user_id))
			{
				g_clients[np].m_cl.unlock();

				PacketManager::SendMovePacket(g_clients, np, user_id);
			}
			else  
			{
				g_clients[np].m_cl.unlock();
				PacketManager::SendEnterPacket(g_clients, np, user_id);
			}
		}
	}

	for (auto old_p : old_vl)		// 시야에서 벗어난 오브젝트에 대한 처리
	{
		if (0 == new_vl.count(old_p))
		{
			PacketManager::SendLeavePacket(g_clients, user_id, old_p);
			if (false == IsPlayer(old_p)) continue;
			g_clients[old_p].m_cl.lock();
			if (0 != g_clients[old_p].m_view_list.count(user_id))
			{
				g_clients[old_p].m_cl.unlock();
				PacketManager::SendLeavePacket(g_clients, old_p, user_id);
			}
			else
			{
				g_clients[old_p].m_cl.unlock();
			}
		}
	}

	CheckUserHit(user_id);
}

void RandomMoveNPC(int id)
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
		if (ObjectStatus::OS_Active != g_clients[i].m_status) continue;
		if (true == IsNear(i, id))
		{
			g_clients[i].m_cl.lock();
			if (0 != g_clients[i].m_view_list.count(id))
			{
				g_clients[i].m_cl.unlock();

				PacketManager::SendMovePacket(g_clients, i, id);
			}
			else
			{
				g_clients[i].m_cl.unlock();
				PacketManager::SendEnterPacket(g_clients, i, id);
			}
		}
		else
		{
			g_clients[i].m_cl.lock();
			if (0 != g_clients[i].m_view_list.count(id))
			{
				g_clients[i].m_cl.unlock();
				PacketManager::SendLeavePacket(g_clients, i, id);
			}
			else
				g_clients[i].m_cl.unlock();
		}
	}

	CheckMonsterHit(id);
}

void EnterGame(int user_id, char name[])
{
	g_clients[user_id].m_cl.lock();
	strcpy_s(g_clients[user_id].m_name, name);
	g_clients[user_id].m_name[MAX_ID_LEN] = NULL;

	///////////////////////////////////////////////////////
	// DB 로드 없이 실행 할 때 (주석 처리)
	std::wstring wideName = CharArrayToWString(name);
	DB::DB_LoadCharacter(g_clients, user_id, wideName, NAME_LEN, DSN_NAME);
	///////////////////////////////////////////////////////
	PacketManager::SendLoginSuccessPacket(g_clients, user_id);
	UserCount++;

	// 접속시간 저장 및 타이머 등록
	g_clients[user_id].m_last_connect_time = high_resolution_clock::now();
	AddTimer(user_id, ENUMOP::OP_CHECK_CONNECT_TIME, PERIODICALLY_SAVE_TIME);

	g_clients[user_id].m_status = ObjectStatus::OS_Active;
	g_clients[user_id].m_cl.unlock();


	for (auto &cl : g_clients)
	{
		int i = cl.m_id;
		if (user_id == i) continue;
		if (true == IsNear(user_id, i))
		{
			if (ObjectStatus::OS_Sleep == g_clients[i].m_status)
			{
				ActivateNPC(i);
			}
			if (ObjectStatus::OS_Active == g_clients[i].m_status)
			{
				PacketManager::SendEnterPacket(g_clients, user_id, i);
				if (true == IsPlayer(i))
					PacketManager::SendEnterPacket(g_clients, i, user_id);
			}

			PacketManager::SendStatChangePacket(g_clients, i, g_clients[user_id].m_id);
		}
	}
}

void DoAttack(int id)
{
	g_clients[id].m_cl.lock();
	auto vl = g_clients[id].m_view_list;
	g_clients[id].m_cl.unlock();

	// 내가 공격했음을 내 주변 유저에게 알림
	for (auto user : vl)
	{
		if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
		{
			PacketManager::SendAttackPacket(g_clients, g_clients[user].m_id, g_clients[id].m_id, g_clients[id].m_attackrange, g_clients[id].x, g_clients[id].y);
		}
	}

	// 나에게 알림
	PacketManager::SendAttackPacket(g_clients, g_clients[id].m_id, g_clients[id].m_id, g_clients[id].m_attackrange, g_clients[id].x, g_clients[id].y);

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
				IsNPCDead(id, npc);

				char mess[100];
				sprintf_s(mess, "[ %s ] -> Attack -> [ %s ] ( %d Damage ).", g_clients[id].m_name, g_clients[npc].m_name, g_clients[id].m_att);


				// 내 주변 유저에게 BroadCast
				for (auto user : vl)
				{
					if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
					{
						PacketManager::SendChatPacket(g_clients, g_clients[user].m_id, id, mess, 1);
						PacketManager::SendStatChangePacket(g_clients, g_clients[user].m_id, npc);
					}
				}

				// 나에게 알림
				PacketManager::SendChatPacket(g_clients, g_clients[id].m_id, id, mess, 1);
				PacketManager::SendStatChangePacket(g_clients, g_clients[id].m_id, npc);
			}
		}
	}

}

void DoRevive(int id)
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
		if (true == IsNear(id, i))
		{
			PacketManager::SendEnterPacket(g_clients, id, i);
			PacketManager::SendEnterPacket(g_clients, i, id);
		}
	}

	// 내 주변 유저에게 BroadCast
	for (auto user : vl)
	{
		if (g_clients[user].m_id >= 0 && g_clients[user].m_id < NPC_ID_START)
		{
			PacketManager::SendStatChangePacket(g_clients, g_clients[user].m_id, id, true);
		}
	}

	PacketManager::SendStatChangePacket(g_clients, g_clients[id].m_id, id, true);
}

void ProcessPacket(int user_id, char* buf)
{
	// buf[1]의 값을 C2S_Packet 타입으로 변환
	C2S_Packet packetType = static_cast<C2S_Packet>(buf[1]);

	switch (packetType) {
	case C2S_Packet::C2S_LOGIN: {
		cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(buf);
		switch (packet->loginType)
		{
		case 1:
		{
			std::wstring wideName = CharArrayToWString(packet->name);
			DB::DB_CreateCharacter(wideName, DSN_NAME);
		}
		break;

		case 2:
			EnterGame(user_id, packet->name);
			break;
		}
	}
					break;
	case C2S_Packet::C2S_MOVE: {
		cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(buf);
		g_clients[user_id].m_move_time = packet->move_time;
		DoMove(user_id, packet->direction);
	}
				   break;

	case C2S_Packet::C2S_ATTACK: {
		cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(buf);
		DoAttack(user_id);
	}
					 break;

	case C2S_Packet::C2S_REVIVE: {
		cs_packet_revive* packet = reinterpret_cast<cs_packet_revive*>(buf);
		DoRevive(user_id);
	}
					 break;

	default:
		cout << "Unknown Packet Type Error!\n";
		DebugBreak();
		exit(-1);
	}
}

void Disconnect(int user_id)
{
	PacketManager::SendLeavePacket(g_clients, user_id, user_id);

	// TODO. SaveCharacter DB
	// g_clients[user_id] > DB 테이블안에 들어가면 댐
	// EXEC Game.dbo.SaveCharacter '%s', g_client[user_id].name
	std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
	DB::DB_SaveCharacter(g_clients, g_clients[user_id].m_id, wideName, DSN_NAME);

	g_clients[user_id].m_cl.lock();
	g_clients[user_id].m_status = ObjectStatus::OS_Alloc;
	closesocket(g_clients[user_id].m_s);
	for (int i = 0; i < NPC_ID_START; ++i)
	{
		Client&cl = g_clients[i];
		if (user_id == cl.m_id) continue;
		if (ObjectStatus::OS_Active == cl.m_status)
			PacketManager::SendLeavePacket(g_clients, cl.m_id, user_id);
	}
	g_clients[user_id].m_status = ObjectStatus::OS_Free;
	g_clients[user_id].m_cl.unlock();
}

void RecvPacketConstruct(int user_id, int io_byte)
{
	Client& cu = g_clients[user_id];
	ExOver& r_o = cu.m_recv_over;

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
			ProcessPacket(user_id, cu.m_packet_buf);
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

void WorkerThread()
{
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);

		ExOver* exover = reinterpret_cast<ExOver*>(over);
		int user_id = static_cast<int>(key);
		Client& cl = g_clients[user_id];

		switch (exover->op) {
		case ENUMOP::OP_RECV:
			if (0 == io_byte) Disconnect(user_id);
			else {
				RecvPacketConstruct(user_id, io_byte);
				ZeroMemory(&cl.m_recv_over.over, sizeof(cl.m_recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.m_s, &cl.m_recv_over.wsabuf, 1, NULL, &flags, &cl.m_recv_over.over, NULL);
			}
			break;
		case ENUMOP::OP_SEND:
			if (0 == io_byte) Disconnect(user_id);
			delete exover;
			break;
		case ENUMOP::OP_ACCEPT: {
			int user_id = -1;
			for (int i = 0; i < MAX_USER; ++i) {
				lock_guard<mutex> gl{ g_clients[i].m_cl };
				if (ObjectStatus::OS_Free == g_clients[i].m_status) {
					g_clients[i].m_status = ObjectStatus::OS_Alloc;
					user_id = i;
					break;
				}
			}

			SOCKET c_socket = exover->c_socket;
			if (-1 == user_id)
				closesocket(c_socket);
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), g_iocp, user_id, 0);
				Client& nc = g_clients[user_id];
				nc.m_prev_size = 0;
				nc.m_recv_over.op = ENUMOP::OP_RECV;
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
		case ENUMOP::OP_RANDOM_MOVE: {
			RandomMoveNPC(user_id);
			bool keep_alive = false;
			for (int i = 0; i < NPC_ID_START; ++i)
				if (true == IsNear(user_id, i))
					if (ObjectStatus::OS_Active == g_clients[i].m_status)
					{
						keep_alive = true;
						break;
					}
			if (true == keep_alive) AddTimer(user_id, ENUMOP::OP_RANDOM_MOVE, 1000);
			else g_clients[user_id].m_status = ObjectStatus::OS_Sleep;
			delete exover;
		}
									 break;
		case ENUMOP::OP_PLAYER_MOVE: {
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
			cout << "Unknown Operation in WorkerThread!\n";
			while (true);

		}
	}
}

int API_SendMessage(lua_State* L)
{
	int my_id = (int)lua_tointeger(L, -3);
	int user_id = (int)lua_tointeger(L, -2);
	char *mess = (char*)lua_tostring(L, -1);

	PacketManager::SendChatPacket(g_clients, user_id, my_id, mess, 2);
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

void InitializeClients()
{
	for (int i = 0; i < MAX_USER + NUM_NPC; ++i)
	{
		if (i >= 0 && i < NPC_ID_START)
		{
			g_clients[i].m_id = i;
			g_clients[i].m_level = 1;
			g_clients[i].m_status = ObjectStatus::OS_Free;
			g_clients[i].m_maxhp = PLAYER_MAX_HP;
			g_clients[i].m_hp = PLAYER_MAX_HP;
			g_clients[i].m_maxexp = static_cast<int>(PlayerMaxExp::PME_Level1);
			g_clients[i].m_att = static_cast<int>(PlayerAttackData::PAD_Level1);
			g_clients[i].m_attackrange = 1;
			g_clients[i].objectType = ObjectType::OT_Player;
		}

		else
		{
			g_clients[i].is_active = true;
			g_clients[i].is_alive = true;
			bool map_blocked = true;
			while (map_blocked)
			{
				g_clients[i].x = rand() % WORLD_WIDTH;
				g_clients[i].y = rand() % WORLD_HEIGHT;
				if (g_Map[g_clients[i].x][g_clients[i].y] == eBLANK) break;
			}

			if (i >= NPC_ID_START && i < NPC2_ID_START)
			{
				g_clients[i].m_id = i;

				g_clients[i].m_level = 1;
				g_clients[i].m_status = ObjectStatus::OS_Sleep;
				g_clients[i].m_hp = static_cast<int>(MonsterMaxHp::MMH_Level1);
				g_clients[i].m_maxhp = static_cast<int>(MonsterMaxHp::MMH_Level1);
				g_clients[i].given_exp = static_cast<int>(MonsterGivenExp::MGE_Level1);
				g_clients[i].m_att = static_cast<int>(MonsterAttackData::MAD_Level1);
				sprintf_s(g_clients[i].m_name, "MONSTER 1");
				g_clients[i].objectType = ObjectType::OT_Monster1;

			}

			if (i >= NPC2_ID_START && i < NPC3_ID_START)
			{
				g_clients[i].m_id = i;

				g_clients[i].m_level = 2;
				g_clients[i].m_status = ObjectStatus::OS_Sleep;
				g_clients[i].m_hp = static_cast<int>(MonsterMaxHp::MMH_Level2);
				g_clients[i].m_maxhp = static_cast<int>(MonsterMaxHp::MMH_Level2);
				g_clients[i].given_exp = static_cast<int>(MonsterGivenExp::MGE_Level2);
				g_clients[i].m_att = static_cast<int>(MonsterAttackData::MAD_Level2);
				sprintf_s(g_clients[i].m_name, "MONSTER 2");
				g_clients[i].objectType = ObjectType::OT_Monster2;
			}

			if (i >= NPC3_ID_START && i < QUEST_NPC_NUMBER)
			{
				g_clients[i].m_id = i;

				g_clients[i].m_level = 3;
				g_clients[i].m_status = ObjectStatus::OS_Sleep;
				g_clients[i].m_hp = static_cast<int>(MonsterMaxHp::MMH_Level3);
				g_clients[i].m_maxhp = static_cast<int>(MonsterMaxHp::MMH_Level3);
				g_clients[i].given_exp = static_cast<int>(MonsterGivenExp::MGE_Level3);
				g_clients[i].m_att = static_cast<int>(MonsterAttackData::MAD_Level3);
				sprintf_s(g_clients[i].m_name, "MONSTER 3");
				g_clients[i].objectType = ObjectType::OT_Monster3;
			}

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
	}
	g_clients[QUEST_NPC_NUMBER].m_id = QUEST_NPC_NUMBER;
	g_clients[QUEST_NPC_NUMBER].objectType = ObjectType::OT_NPC;
	g_clients[QUEST_NPC_NUMBER].x = 15;
	g_clients[QUEST_NPC_NUMBER].y = 15;
	g_clients[QUEST_NPC_NUMBER].m_hp = static_cast<int>(MonsterMaxHp::MMH_Level3);
	g_clients[QUEST_NPC_NUMBER].m_maxhp = static_cast<int>(MonsterMaxHp::MMH_Level3);
	sprintf_s(g_clients[QUEST_NPC_NUMBER].m_name, "QUEST_NPC");
	g_clients[QUEST_NPC_NUMBER].m_status = ObjectStatus::OS_Active;
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

void InitMap()
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

void Periodically_Save(int user_id)
{
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - g_clients[user_id].m_last_connect_time);
	if (duration.count() >= PERIODICALLY_SAVE_TIME)
	{
		// 일정 주기 마다 호출할 함수
		std::wstring wideName = CharArrayToWString(g_clients[user_id].m_name);
		DB::DB_SaveCharacter(g_clients, g_clients[user_id].m_id, wideName, DSN_NAME);

		// 마지막 호출 시간을 갱신
		g_clients[user_id].m_last_connect_time = now;
	}
}

void DoTimer()
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
			EventType ev = timer_queue.top();
			timer_queue.pop();
			timer_lock.unlock();
			switch (ev.event_id)
			{
			case ENUMOP::OP_RANDOM_MOVE:
			{
				ExOver *over = new ExOver;
				over->op = ev.event_id;
				PostQueuedCompletionStatus(g_iocp, 1, ev.obj_id, &over->over);
				//add_timer(ev.obj_id, ev.event_id, 1000);
			}
			break;

			case ENUMOP::OP_CHECK_CONNECT_TIME:
			{
				Periodically_Save(ev.obj_id);
				// 180초(3분) 후 다시 타이머 추가
				AddTimer(ev.obj_id, ENUMOP::OP_CHECK_CONNECT_TIME, PERIODICALLY_SAVE_TIME);
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

	l_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN s_address;
	memset(&s_address, 0, sizeof(s_address));
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(SERVER_PORT);
	s_address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(l_socket, reinterpret_cast<sockaddr *>(&s_address), sizeof(s_address));

	listen(l_socket, SOMAXCONN);

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	InitMap();
	cout << "Server Initialization start.\n";
	InitializeClients();
	cout << "Server Initialization finished.\n";

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(l_socket), g_iocp, 999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ExOver accept_over;
	ZeroMemory(&accept_over.over, sizeof(accept_over.over));
	accept_over.op = ENUMOP::OP_ACCEPT;
	accept_over.c_socket = c_socket;
	AcceptEx(l_socket, c_socket, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);

	vector <thread> worker_threads;
	for (int i = 0; i < 4; ++i) worker_threads.emplace_back(WorkerThread);

	thread timer_thread{ DoTimer };
	for (auto& th : worker_threads) th.join();
}