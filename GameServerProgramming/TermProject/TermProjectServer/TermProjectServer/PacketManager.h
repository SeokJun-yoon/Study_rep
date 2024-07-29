#pragma once
#include "Datas.h"

struct Client;

namespace PacketManager
{
	void SendPacket(Client* clients, int userID, void* p);

	void SendLoginSuccessPacket(Client* clients, int userID, bool IsLoginOK);
	void SendLoginFailPacket(Client* clients, int userID);

	void SendEnterPacket(Client* clients, int userID, int otherID);
	void SendLeavePacket(Client* clients, int userID, int otherID);

	void SendStatChangePacket(Client* clients, int getID, int setID, bool isRevive = false);

	void SendMovePacket(Client* clients, int userID, int mover);
	void SendAttackPacket(Client* clients, int userID, int attackerID, int attackerRanger, int attackerX, int attackerY);

	void SendChatPacket(Client* clients, int userID, int chatter, char* msg, int msgType);
}

