#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 실습

struct StatInfo
{
	int hp; // +0
	int attack; // +4
	int defence; // +8
};

void EnterLobby();
StatInfo CreatePlayer();
void CreateMonster(StatInfo* info);
// 플레이어 승리 시 true, 아니면 false
bool StartBattle(StatInfo* player, StatInfo* monster);

int main()
{
	EnterLobby();


	return 0;
}

void EnterLobby()
{
	cout << "로비에 입장했습니다" << endl;

	StatInfo player;
	// 값 변화를 관찰하기 위한 임의 초기값 설정
	player.hp = 0xbbbbbbbb;
	player.attack = 0xbbbbbbbb;
	player.defence = 0xbbbbbbbb;

	// [매개변수][RET][지역변수(temp(100,20,2)player(100,10,2)] [매개변수(&temp)][RET][지역변수(ret(100,20,2))]
	player=CreatePlayer();
	// 위와 같이 만들게 되면 결국 복사(사본 만들기)를 통해 이루어지므로 성능적인 부하를 야기시킬 수 있다

	StatInfo monster;
	// 값 변화를 관찰하기 위한 임의 초기값 설정
	monster.hp = 0xbbbbbbbb;
	monster.attack = 0xbbbbbbbb;
	monster.defence = 0xbbbbbbbb;

	// [매개변수][RET][지역변수(monster(40,8,1)player(b,b,b)] [매개변수(&monster)][RET][지역변수())]
	CreateMonster(&monster);
	// 결국 위의 포인터를 활용한 방식이 훨씬 효율이 좋다

	// 번외편1)
	// 구조체끼리 복사할 때 무슨일이 벌어질까?
	// 
	player = monster;
	// 위에서 일어나는 과정의 풀이가 아래
	player.hp = monster.hp;
	player.attack = monster.attack;
	player.defence = monster.defence;

	bool victory = StartBattle(&player, &monster);

	if (victory)
		cout << "승리!" << endl;
	else
		cout << "패배!" << endl;
}

StatInfo CreatePlayer()
{
	StatInfo ret;

	cout << "플레이어 생성" << endl;

	ret.hp = 100;
	ret.attack = 10;
	ret.defence = 2;

	return ret;
}

void CreateMonster(StatInfo* info)
{
	cout << "몬스터 생성" << endl;
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

		cout << "몬스터 HP : " << monster->hp << endl;

		if (monster->hp == 0)
			return true;

		damage = monster->attack - player->defence;
		if (damage < 0)
			damage = 0;

		cout << "플레이어 HP : " << player->hp << endl;

		player->hp -= damage;
		if (player->hp < 0)
			player->hp = 0;

		if (player->hp == 0)
			return false;
	}
}
