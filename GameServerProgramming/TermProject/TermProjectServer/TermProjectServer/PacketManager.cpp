#include "PacketManager.h"

void PacketManager::SendPacket(Client* clients, int userID, void * p)
{
	char* buf = reinterpret_cast<char *>(p);

	Client& u = clients[userID];

	ExOver* exOver = new ExOver;
	exOver->op = ENUMOP::OP_SEND;
	ZeroMemory(&exOver->over, sizeof(exOver->over));
	exOver->wsabuf.buf = exOver->io_buf;
	exOver->wsabuf.len = buf[0];
	memcpy(exOver->io_buf, buf, buf[0]);
	WSASend(u.m_s, &exOver->wsabuf, 1, NULL, 0, &exOver->over, NULL);
}

void PacketManager::SendLoginSuccessPacket(Client* clients, int userID)
{
	sc_packet_login_ok p;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_LOGIN_OK);
	strcpy_s(p.name, clients[userID].m_name);
	p.id = userID;
	p.level = clients[userID].m_level;
	p.exp = clients[userID].m_exp;
	p.maxexp = clients[userID].m_maxexp;
	p.hp = clients[userID].m_hp;
	p.maxhp = clients[userID].m_maxhp;
	p.x = clients[userID].x;
	p.y = clients[userID].y;

	SendPacket(clients, userID, &p);
}

void PacketManager::SendLoginFailPacket(Client* clients, int userID)
{
	sc_packet_login_fail p;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_LOGIN_FAIL);

	char error[MAX_STR_LEN];
	sprintf_s(error, "Login Error - Invalid ID !");
	strcpy_s(p.message, error);

	SendPacket(clients, userID, &p);
}

void PacketManager::SendEnterPacket(Client* clients, int userID, int otherID)
{
	sc_packet_enter p;
	p.id = otherID;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_ENTER);
	p.x = clients[otherID].x;
	p.y = clients[otherID].y;
	p.level = clients[otherID].m_level;
	p.hp = clients[otherID].m_hp;
	p.maxhp = clients[otherID].m_maxhp;
	p.exp = clients[otherID].m_exp;
	p.maxexp = clients[otherID].m_maxexp;
	
	strcpy_s(p.name, clients[otherID].m_name);

	p.objectType = static_cast<int>(clients[otherID].objectType);

	clients[userID].m_cl.lock();
	clients[userID].m_view_list.insert(otherID);
	clients[userID].m_cl.unlock();

	SendPacket(clients, userID, &p);
}

void PacketManager::SendLeavePacket(Client* clients, int userID, int otherID)
{
	sc_packet_leave p;
	p.id = otherID;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_LEAVE);

	clients[userID].m_cl.lock();
	clients[userID].m_view_list.erase(otherID);
	clients[userID].m_cl.unlock();

	SendPacket(clients, userID, &p);
}

void PacketManager::SendStatChangePacket(Client* clients, int getID, int setID, bool isRevive)
{
	sc_packet_stat_change p;
	p.id = setID;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_CHANGE_STATS);
	p.level = clients[setID].m_level;
	p.hp = clients[setID].m_hp;
	p.maxhp = clients[setID].m_maxhp;
	p.exp = clients[setID].m_exp;
	p.att = clients[setID].m_att;
	p.isRevive = isRevive;
	p.x = clients[setID].x;
	p.y = clients[setID].y;

	p.maxexp = clients[setID].m_maxexp;
	p.attrange = clients[setID].m_attackrange;

	p.givenexp = clients[setID].given_exp;

	p.objectType = static_cast<int>(clients[setID].objectType);

	SendPacket(clients, getID, &p);
}

void PacketManager::SendMovePacket(Client* clients, int userID, int mover)
{
	sc_packet_move p;
	p.id = mover;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_MOVE);
	p.x = clients[mover].x;
	p.y = clients[mover].y;
	p.move_time = clients[mover].m_move_time;

	clients[mover].m_cl.lock();
	clients[mover].m_cl.unlock();

	SendPacket(clients, userID, &p);
}

void PacketManager::SendAttackPacket(Client* clients, int userID, int attackerID, int attackerRanger, int attackerX, int attackerY)
{
	sc_packet_attack p;
	p.id = attackerID;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_ATTACK);
	p.x = clients[attackerID].x;
	p.y = clients[attackerID].y;
	p.attackrange = clients[attackerID].m_attackrange;

	SendPacket(clients, userID, &p);
}

void PacketManager::SendChatPacket(Client* clients, int userID, int chatter, char* msg, int msgType)
{
	sc_packet_chat p;
	p.id = chatter;
	p.size = sizeof(p);
	p.type = static_cast<char>(S2C_Packet::S2C_CHAT);
	strcpy_s(p.mess, msg);
	p.mess_type = msgType;

	SendPacket(clients, userID, &p);
}
