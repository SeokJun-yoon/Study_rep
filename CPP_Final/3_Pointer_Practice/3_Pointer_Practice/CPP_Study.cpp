#include <iostream>
using namespace std;

// 주제 : 포인터 실습

struct StatInfo
{
	int hp;	// +0
	int attack;	// +4
	int defence; // +8
};

void EnterLobby();
StatInfo CreatePlayer();
void CreateMonster(StatInfo* info);

int main()
{
	EnterLobby();


	return 0;
}

void EnterLobby()
{
	cout << "로비에 입장했습니다." << endl;

	StatInfo player;	// 포인터를 사용할 수 있다면, 전역 변수 사용을 지양하기
	CreatePlayer();
}
