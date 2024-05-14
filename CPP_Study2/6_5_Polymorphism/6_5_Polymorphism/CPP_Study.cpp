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

class Player
{
public:
	void Move() { cout << "Move Player !" << endl; }

	//void Move(int a) { cout << "Move Player (int) ! " << endl; }
	virtual void VMove() { cout << "Move Player !" << endl; }

public:
	int _hp;
};

class Knight : public Player
{
public:
	void Move() { cout << "Move Knight!" << endl; }
	// ���� ������ �Լ��� ���� ȣ���ϸ� ��ӹ��� �Լ��� �ƴ� �� �Լ��� ȣ��ȴ�.
	
	// ���� �Լ��� �����Ǹ� �ϴ��� ���� �Լ���!
	virtual void VMove() { cout << "Move Knight !" << endl; }

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
	Player p;
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