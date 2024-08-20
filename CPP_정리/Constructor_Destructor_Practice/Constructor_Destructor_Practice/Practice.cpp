#include <iostream>
using namespace std;

class Knight
{
public:
	// [1] �⺻ ������ (���ڰ� ����)
	Knight()
	{
		cout << "Knight() �⺻ ������ ȣ��" << endl;
	}

	// [2] ���� ������ (�ڱ� �ڽ��� Ŭ���� ���� Ÿ���� ���ڷ� ����)
	// �Ϲ������� '�Ȱ���' �����͸� ���� ��ü�� �����Ǳ� ����Ѵ�
	Knight(const Knight& knight)
	{
		_hp = knight._hp;
		_attack = knight._attack;
		_posX = knight._posX;
		_posY = knight._posY;
		cout << "Knight() ���� ������ ȣ��" << endl;
	}

	// [3] ��Ÿ ������
	// �� �߿��� ���ڸ� 1���� �޴� [��Ÿ ������]��
	// [Ÿ�� ��ȯ ������]��� �θ��⵵ ��
	Knight(int hp)
	{
		cout << "Knight() ������ ȣ��" << endl;
		_hp = hp;
	}

	Knight(int hp, int attack, int posX, int posY)
	{
		_hp = hp;
		_attack = attack;
		_posX = posX;
		_posY = posY;
	}

	// [4] �Ҹ���
	~Knight()
	{
		cout << "Knight(int) �Ҹ��� ȣ��" << endl;
	}

public:
	// ��� ����
	int _hp;
	int _attack;
	int _posX;
	int _posY;

public:
	// ��� �Լ�
	void Move(int x, int y);
	void Attack();
	void Die()
	{
		_hp = 0;
		this->_hp = 1;
		cout << "Die" << endl;
	}
};

void Knight::Move(int x, int y)
{
	_posX = x;
	_posY = y;
	cout << "Move" << endl;
}

void Knight::Attack()
{
	cout << "Attack" << _attack << endl;
}
