#include <iostream>
using namespace std;

// ���� : ������ �ǽ�

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
	cout << "�κ� �����߽��ϴ�." << endl;

	StatInfo player;	// �����͸� ����� �� �ִٸ�, ���� ���� ����� �����ϱ�
	CreatePlayer();
}
