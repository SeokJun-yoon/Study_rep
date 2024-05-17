#include <iostream>
using namespace std;

// ������ ���� : ��ü���� ������

// 1) struct vs class

// C++������ struct�� class�� ���� �� �� ���̴�.
// struct�� �⺻ ���� �����ڰ� public�̰�, class�� private�̴�.
// �� �̷��� ������? C++�� C���� �Ļ��Ǿ� �����߱� ������, ȣȯ���� ��Ű�� ����.
// -> struct�� �׳� ����ü (������ ����)�� ǥ���ϴ� �뵵
// -> class�� ��ü ���� ���α׷����� Ư¡�� ��Ÿ���� �뵵

struct TestStruct
{
public:
	int _a;
	int _b;
};

class TestClass
{
private:
	int _a;
	int _b;
};

// 2) static ����, static �Լ� (static = ���� = ������)

class Marine
{
public:
	// Ư�� ���� ��ü�� ������
	int _hp;

	void TakeDamage(int damage)
	{
		_hp -= damage;
	}

	static void SetAttack()
	{
		s_attack = 100;
	}

	// Ư�� ���� ��ü�� ����
	// �����̶�� 'Ŭ����' ��ü�� ����
	static int s_attack; // ���赵�����θ� ����

};

// static ������ � �޸�?
// �ʱ�ȭ �ϸ� .data
// �ʱ�ȭ �� �ϸ� .bss
int Marine::s_attack = 0;

class Player
{
public:
	int _id;
};

int GenerateId()	// id�� 1, 2, 3,4 �̷������� �߱��� �ؾ� �Ѵٸ�,
{
	// �����ֱ�: ���α׷� ����/���� (�޸𸮿� �׻� �ö� ����)
	// ���ù���: 

	// ���� ���� ��ü
	static int s_id = 1;	// ó������ 1�� �ʱ�ȭ, �� ���� ȣ��Ǹ� ȣ��Ǵ´�� ���� ����

	return s_id++;
}

int main() 
{
	//TestStruct ts;
	//ts._a = 1;

	//TestClass tc;
	//tc._a = 1;

	Marine m1;
	m1._hp = 40;

	Marine::s_attack = 6;
	//m1.s_attack = 6;
	Marine m2;
	m2._hp = 40;
	//m2.s_attack = 6;

	// ���� ���ݷ� ���׷��̵� �Ϸ�! (Academy���� ���׷��̵� ��)
	Marine::s_attack = 7;
	//m1.s_attack = 7;
	//m2.s_attack = 7;
	Marine::SetAttack();

	// ���� �ƴ�. .data ����
	static int id = 10;
	int a = id;

	cout << GenerateId() << endl;
	cout << GenerateId() << endl;
	cout << GenerateId() << endl;
	cout << GenerateId() << endl;
	cout << GenerateId() << endl;

	return 0;
}
