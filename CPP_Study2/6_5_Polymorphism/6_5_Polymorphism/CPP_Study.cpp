#include <iostream>
using namespace std;

// ������ ���� : ������

// ��ü���� (OOP Object Oriented Programming)
// - ��Ӽ�
// - ���м� = ĸ��ȭ
// - ������

// ������(Polymorphism = Poly + morph = ���� �Ȱ�����, ����� �ٸ��� �����Ѵ�)
// - �����ε�(Overloading) = �Լ� �ߺ� ���� = �Լ� �̸��� ����
// - �������̵�(Overriding) = ������ = �θ� Ŭ������ �Լ��� �ڽ� Ŭ�������� ������

// ���ε�(Binding) = ���´�
// - ���� ���ε�(Static Binding) : ������ ������ ����
// - ���� ���ε�(Dynamic Binding) : ���� ������ ����

// �Ϲ� �Լ��� ���� ���ε��� ���
// ���� ���ε��� ���Ѵٸ�? -> ���� �Լ� (virtual function)

// �׷��� ���� ��ü�� � Ÿ������ ��� �˰� �˾Ƽ� �����Լ��� ȣ�����ذɱ�?
// - ���� �Լ� ���̺� (vftable)

// .vftable [] 4����Ʈ(32) 8����Ʈ(64)
// [VMove] [VDie]

// ���� ���� �Լ� : ������ ���� '�������̽�'�� �����ϴ� �뵵�� ����ϰ� ���� ���
// �߻� Ŭ���� : ���� ���� �Լ��� 1�� �̻� ���ԵǸ� �ٷ� �߻� Ŭ������ ����
// - ���������� ��ü�� ���� �� ���� ��

class Player
{
public:
	Player()
	{
		_hp = 100;
	}
	void Move() { cout << "Move Player !" << endl; }

	//void Move(int a) { cout << "Move Player (int) ! " << endl; }
	virtual void VMove() { cout << "VMove Player !" << endl; }
	virtual void VDie() { cout << "VDie Player !" << endl; }

	// ���� ���� �Լ�
	virtual void VAttack() = 0;	

public:
	int _hp;
};

class Knight : public Player
{
public:
	Knight()
	{
		_stamina = 100;
	}

	void Move() { cout << "Move Knight!" << endl; }
	// ���� ������ �Լ��� ���� ȣ���ϸ� ��ӹ��� �Լ��� �ƴ� �� �Լ��� ȣ��ȴ�.
	
	// ���� �Լ��� �����Ǹ� �ϴ��� ���� �Լ���!
	virtual void VMove() { cout << "Move Knight !" << endl; }
	virtual void VDie() { cout << "VDie Knight !" << endl; }

	virtual void VAttack() { cout << "VAttack Knight" << endl; }


public:
	int _stamina;
};

class Mage : public Player
{
public:
	
public:
	int _mp;
};

void MovePlayer(Player* player)
{
	player->VMove();
}

void MoveKnight(Knight* knight)
{
	knight->Move();
}


int main()
{
	//Player p;
	//MovePlayer(&p); // �÷��̾�� �÷��̾��? YES
	//MoveKnight(&p); // �÷��̾�� ����? NO
	//p.Move();
	//p.Move(100);

	Knight k;
	//MoveKnight(&k); // ���� ����? YES
	MovePlayer(&k); // ���� �÷��̾��? YES
	// Move 'Player'�� ȣ��ȴ�

	return 0;
}