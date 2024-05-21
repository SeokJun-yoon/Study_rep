#include <iostream>
using namespace std;

// ������ ���� : ����

struct StatInfo
{
	int hp;	// +0
	int attack;	// +4
	int defence; // +8
};

// [�Ű�����][RET][��������] [�Ű�����(&info)][RET][��������]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 8;
	info->defence = 5;
}

// [�Ű�����][RET][��������] [�Ű�����(info(100, 8, 5))][RET][��������]
// ���纻�� �ǵ帰 ���̹Ƿ�, �Լ� ���Ͻÿ� ���ư�������
void CreateMonster(StatInfo info)
{
	info.hp = 100;
	info.attack = 8;
	info.defence = 5;
}

// Ư�� ������ �����ϴ� ���͸� ã�� �Լ�
StatInfo* FindMonster()
{
	// TODO : Heap �������� ������ ã�ƺ�
	// ã�Ҵ�!
	// return monster~;

	return nullptr;
}


StatInfo globalInfo;
// 2) �ּ� ���� ���
// [�Ű�����][RET][��������(info)] [�Ű�����(&info)][RET][��������]
void PrintInfoByPtr(const StatInfo* const info)
{
	if (info == nullptr)
		return;

	cout << "---------------------------" << endl;
	cout << "HP: " << info->hp << endl;
	cout << "ATT: " << info->attack << endl;
	cout << "DEF: " << info->defence << endl;
	cout << "---------------------------" << endl;

	// �� �ڿ� ���δٸ�?
	// StatInfo* const info
	// info��� �ٱ����� ���빰[�ּ�]�� �ٲ� �� ����
	// info�� �ּҰ��� ���� �ٱ��� -> �� �ּҰ��� �����̴�!

	// �� ������ ���δٸ�?
	// const StaticInfo* info
	// info�� '����Ű�� �ִ�' �ٱ����� ���빰�� �ٲ� �� ����
	// '����' �ٱ����� ���빰�� �ٲ� �� ����

	// info[ �ּҰ� ]	�ּҰ�[ ������ ]
	// info=&globalInfo;

	// info->hp=10000;
}

// StatInfo ����ü�� 1000����Ʈ¥�� ���� ����ü���?
// - (�� ����) StatInfo�� �ѱ�� 1000����Ʈ�� ����Ǵ�
// - (�ּ� ����) StatInfo*�� 8����Ʈ (x64 ����)
// �� �����͸� �̿��� �ּ� ���� ����� ���� ���̸� ���� �� �� �ִ�
// - (���� ����) StatInfo&�� 8����Ʈ

// 3) ���� ���� ���
// �� ����ó�� ���ϰ� ����ϰ�!
// �ּ� ����ó�� �ּҰ��� �̿��� ������ �ǵ帮��
// �ϼ������� ���!

void PrintInfoByRef(const StatInfo& info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATT: " << info.attack << endl;
	cout << "DEF: " << info.defence << endl;
	cout << "---------------------------" << endl;

	// ������ �Դ�
	//info.hp=10000;	const�� ����ؼ� ����
}

#define OUT
void ChangeInfo(OUT StatInfo& info)
{
	info.hp = 1000;
}

int main()
{
	StatInfo info;

	CreateMonster(&info);

	// ������ vs ���� ������ ���
	// ���� : �Ȱ���!
	// ���Ǽ� : ���� ��!

	// 1) ���Ǽ� ����
	// ���Ǽ��� ���ٴ°� �� �������� �ƴϴ�.
	// �����ʹ� �ּҸ� �ѱ�� Ȯ���ϰ� ������ �ѱ�ٴ� ��Ʈ�� �� �� �ִµ�
	// ������ �ڿ������� �𸣰� ����ĥ ���� ����!
	// ex) ������� ��ģ�ٸ�?
	// const�� ����ؼ� �̷� ������� ��ġ�� �κ� ���� ����

	// ����� �����͵� const�� ��� ����
	// * �������� �տ� ���̴���, �ڿ� ���̴��Ŀ� ���� �ǹ̰� �޶�����.

	// 2) �ʱ�ȭ ����
	// ���� Ÿ���� �ٱ����� 2��° �̸� (��Ī?)
	// -> �����ϴ� ����� ������ �ȵ�
	// �ݸ� �����ʹ� �׳� �~ �ּҶ�� �ǹ�
	// -> ����� �������� ���� ���� ����
	// �����Ϳ��� '����'�� �ǹ̷� ? nullptr
	// ���� Ÿ���� �̷� nullptr

	StatInfo* pointer = nullptr; // ��� �ּҵ� ����Ű�� ���� ���� ���´� -> nullptr
	pointer=&info;
	PrintInfoByPtr(pointer);

	StatInfo& reference=info;
	PrintInfoByRef(reference);

	// �׷��� �����?
	// ��� Team By Team... ������ ���� ����
	// ex) ���ۿ��� ���� ���¼ҽ��� ���� ���� ������ ������ ���
	// ex) �𸮾� �������� reference�� �ֿ�

	// Rookiss ��ȣ ��Ÿ��)
	// - ���� ��쵵 ����ؾ� �Ѵٸ� pointer (null üũ �ʼ�)
	// - �ٲ��� �ʰ� �д� �뵵(readonly)�� ����ϸ� const ret&
	// - �� �� �Ϲ������� ret (��������� ȣ���� �� OUT�� ���δ�)
	ChangeInfo(OUT info);

	// Bonus) �����ͷ� ����ϴ��� ������ �Ѱ��ַ���?
	// pointer[ �ּ�(&info) ]	ref, info [ ������ ]
	PrintInfoByRef(*pointer);

	// Bonus) ������ ����ϴ��� �����ͷ� �Ѱ��ַ���?
	// pointer[ �ּ� ]	reference, info[ ������ ]
	PrintInfoByPtr(&reference);

	return 0;
}