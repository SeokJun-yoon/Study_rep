#include <iostream>
using namespace std;

// ������ ���� : ������ �ǽ�

struct StatInfo 
{
	int hp; // +0
	int attack; // +4
	int defence; // +8
};

void EnterLobby();
StatInfo CreatePlayer();
void CreateMonster(StatInfo* info);
// �÷��̾� �¸� �� true, �ƴϸ� false
bool StartBattle(StatInfo* player, StatInfo* monster);

int main()
{

	EnterLobby();
	return 0;
}

void EnterLobby()
{
	cout << "�κ� �����߽��ϴ�" << endl;

	StatInfo player;
	player.hp = 0xbbbbbbbb;
	player.attack = 0xbbbbbbbb;
	player.defence = 0xbbbbbbbb;

	// [�Ű�����][RET][��������(temp(100, 10, 2), player(100, 10, 2))] [�Ű�����(&temp)[RET][��������(ret(100, 10, 2)
	player = CreatePlayer();

	StatInfo monster;
	monster.hp = 0xbbbbbbbb;
	monster.attack = 0xbbbbbbbb;
	monster.defence = 0xbbbbbbbb;
	// [�Ű�����][RET][��������(monster(40, 8, 1), player(100, 10, 2))] [�Ű�����(&monster)[RET][��������()]
	CreateMonster(&monster);

	// ������1)
	// ����ü���� ������ �� ���� ���� ��������?
	//
	//player = monster;
	bool victory=StartBattle(&player, &monster);

	if (victory)
		cout << "�¸�!" << endl;
	else
		cout << "�й�!" << endl;

}

StatInfo CreatePlayer()
{
	StatInfo ret;

	cout << "�÷��̾� ����" << endl;
	ret.hp = 100;
	ret.attack = 10;
	ret.defence = 2;
	
	return ret;
}

void CreateMonster(StatInfo* info)
{
	cout << "���� ����" << endl;

	info->hp = 40;
	info->attack = 8;
	info->defence = 1;
}


bool StartBattle(StatInfo* player, StatInfo* monster)
{
	while (true)
	{
		int damage = player->attack - monster->defence;
		if (damage < 0)
			damage = 0;

		monster->hp -= damage;
		if (monster->hp < 0)
			monster->hp = 0;

		cout << "���� HP : " << monster->hp << endl;

		if (monster->hp == 0)
			return true;

		damage = monster->attack - player->defence;
		if (damage < 0)
			damage = 0;

		cout << "�÷��̾� HP : " << player->hp << endl;

		player->hp -= damage;
		if (player->hp < 0)
			player->hp=0;

		if (player->hp == 0)
			return false;
	}
}