#include <iostream>
using namespace std;

// ������ ���� : ĳ���� (Ÿ�� ��ȯ)

// << ���� �ܰ� ���� >>
// 1) static_cast  < ��� �󵵰� ���� ����
// 2) dynamic_cast
// 3) const_cast
// 4) reinterpret_cast

class Player
{
public:
	virtual ~Player() { }
};

class Knight : public Player
{

};

class Archer : public Player
{

};

class Dog
{

};

void PrintName(char* str)
{
	cout << str << endl;
}

int main()
{
	// (int) �� ���� ����� C ���� ���� ������� �������� �����

	// static_cast : Ÿ�� ��Ģ�� ���߾� �� �� ������� ĳ���ø� ������ش�
	// 1) int <-> float
	// 2) Player* -> Knight* (�ٿ�ĳ����) << ��, ������ ���� ����

	int hp = 100;
	int maxHp = 200;
	float ratio = static_cast<float>(hp) / maxHp;

	// �θ�->�ڽ� �ڽ�->�θ�
	Player* p = new Knight();
	Knight* k1 = static_cast<Knight*>(p);

	// dynamic_cast : ��� ���迡���� ���� ����ȯ
	// RTTI (RunTime Type Information)
	// �������� Ȱ���� ���
	// - virtual �Լ��� �ϳ��� �����, ��ü�� �޸𸮿� ���� �Լ� ���̺� (vftable) �ּҰ� ���Եȴ�
	// - ���� �߸��� Ÿ������ ĳ������ ������, nullptr ��ȯ ************
	// �̸� �̿��ؼ� �´� Ÿ������ ĳ������ �ߴ��� Ȯ���� �����ϴ�.
	Knight* k2 = dynamic_cast<Knight*>(p);

	// const_cast : const�� ���̰ų� ���ų�
	PrintName(const_cast<char*>("SJyoon"));

	// reinterpret_cast
	// ���� �����ϰ� ������ ������ ĳ����
	// 're-interpret' : �ٽ�-�����ϴ�/�����ϴ�
	// - �����Ͷ� ���� ������� �ٸ� Ÿ�� ��ȯ ��
	__int64 address = reinterpret_cast<__int64>(k2);

	Dog* dog1 = reinterpret_cast<Dog*>(k2);

	void* p = malloc(1000);
	Dog* dog2 = reinterpret_cast<Dog*>(p);

	return 0;
}