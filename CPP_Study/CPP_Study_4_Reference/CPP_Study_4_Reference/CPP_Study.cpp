#include <iostream>
using namespace std;

// ������ ���� : ����

struct StatInfo 
{
	int hp; // +0
	int attack; // +4
	int defence; // +8
};

// [�Ű�����][RET][��������(info)] [�Ű�����(&info)][RET][��������]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 8;
	info->defence = 5;
}

// [�Ű�����][RET][��������(info)] [�Ű�����(info)][RET][��������]
void CreateMonster(StatInfo info)
{
	info.hp = 100;
	info.attack = 8;
	info.defence = 5;
}

// ���� �������� �ʴ´ٸ�, ���� �� �ϴ� ���� ����

// 1) �� ���� ���
// [�Ű�����][RET][��������(info)] [�Ű�����(info)][RET][��������]
void PrintInfoByCopy(StatInfo info)
{
	cout << "---------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATTACK: " << info.attack << endl;
	cout << "DEFENCE: " << info.defence << endl;
	cout << "---------------------" << endl;
}

// 2) �ּ� ���� ���
// [�Ű�����][RET][��������(info)] [�Ű�����(&info)][RET][��������]
void PrintInfoByPtr(StatInfo* info)
{
	cout << "---------------------" << endl;
	cout << "HP: " << info->hp << endl;
	cout << "ATTACK: " << info->attack << endl;
	cout << "DEFENCE: " << info->defence << endl;
	cout << "---------------------" << endl;
}

// StatInfo ����ü�� 1000����Ʈ¥�� ���� ����ü���?
// - (�� ����) StatInfo�� �ѱ�� 1000����Ʈ�� ����Ǵ�
// - (�ּ� ����) StatInfo*�� 8����Ʈ
// - (���� ����) StatInfo&�� 8����Ʈ

// 3) ���� ���� ���
void PrintInfoByRef(StatInfo& info)
{
	cout << "---------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATTACK: " << info.attack << endl;
	cout << "DEFENCE: " << info.defence << endl;
	cout << "---------------------" << endl;
}


// �� ����ó�� ���ϰ� ����ϰ�!
// �ּ� ����ó�� �ּҰ��� �̿��� ������ �ǵ帮��!
// �ϼ������� ���!


int main()
{
	// 4����Ʈ ������ �ٱ��ϸ� ����Ұž�.
	// ������ �� �ٱ��� �̸��� number��� �Ұ�
	// �׷��ϱ� number���� �� �����ų�, number�� �� �ִ´ٰ� �ϸ�
	// �������� �˾Ƶ�� �ش� �ּҿ� 1�� �־��ָ� �ȴ�.
	
	int number = 1;

	// * �ּҸ� ��� �ٱ���
	// int �� �ٱ��ϸ� ���󰡸� int ������(�ٱ���)�� ����
	int* pointer = &number;
	// pointer �ٱ��Ͽ� �ִ� �ּҸ� Ÿ�� �̵��ؼ�, �� �ָ� �ִ� �ٱ��Ͽ� 2�� �ִ´�
	*pointer = 2;


	// Low Level(�����) �������� ���� �۵� ����� int*�� �Ȱ���
	int& reference = number;

	// C++ ���������� number��� �ٱ��Ͽ� �� �ٸ� �̸��� �ο��� ��.
	// number��� �ٱ��Ͽ� reference��� �ٸ� �̸��� �����ٰ�
	// ������ reference �ٱ��Ͽ��ٰ� �� �����ų� ������,
	// ���� number �ٱ���(�������ٰ�) �� ���� �����ų� ������ ��!
	reference = 3;

	// �׷��� ������ �� �ٸ� �̸��� ���� ������?
	// �׳� number = 3�̶�� �ص� �Ȱ�����...
	// ���� ���� ����!

	StatInfo info;
	CreateMonster(&info);
	PrintInfoByCopy(info);
	PrintInfoByPtr(&info);
	PrintInfoByRef(info);

	return 0;
}