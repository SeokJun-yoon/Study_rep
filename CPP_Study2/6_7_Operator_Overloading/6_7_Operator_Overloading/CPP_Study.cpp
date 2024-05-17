#include <iostream>
using namespace std;

// ������ ���� : ������ �����ε�(Operator Overloading)

// ������ vs �Լ�
// - �����ڴ� �ǿ������� ����/Ÿ���� �����Ǿ� ����

// ������ �����ε�?
// �ϴ� [������ �Լ�]�� �����ؾ� ��
// �Լ��� ����Լ� vs �����Լ��� �����ϴ� ��ó��, ������ �Լ��� �� ���� ������� ���� �� ����

// - ��� ������ �Լ� version
// -- a op b ���¿��� �������� �������� ����� (a�� Ŭ�������� ����. a�� '���� �ǿ�����'��� ��
// -- �Ѱ�) a�� Ŭ������ �ƴϸ� ��� ����

// - ���� ������ �Լ� version
// -- a op b ���¶�� a, b��θ� ������ �Լ��� �ǿ����ڷ� ������ش�

// �׷� ������ �� ������? �׷��� ����. ������ �� �� �ϳ��� �����ϴ� ��쵵 �ֱ� ����.
// - ��ǥ������ ���� ������ (a = b)�� ���� ������ ver�δ� �� �����.

// ���� ���� ������
// - ���� �����ڰ� ���� �迡 [���� ���� ������]�� ���� �˾ƺ���
// �� �� �򰥸��� [���� ������] [���� ������] [���� ���� ������]
// - ���� ���� ������ = ���� ������ ��, �ڱ� �ڽ��� ���� Ÿ���� ���ڷ� �޴� ��

// ��Ÿ
// - ��� �����ڸ� �� �����ε� �� �� �ִ� ���� �ƴϴ� (:: . .*�̷� �� �ȵ�)
// - ��� �����ڰ� �� 2�� ���� �ִ� ���� �ƴ�. ++ -- �� ��ǥ�� (���� ������)
// - ���� ������ ++ --
// -- ������ (++a) operator++()
// -- ������ (a++) operator++(int)
class Position
{
public:

	Position operator+(const Position& arg)
	{
		Position pos;
		pos._x = _x + arg._x;
		pos._y = _y + arg._y;
		return pos;
	}

	Position operator+(int arg)
	{
		Position pos;
		pos._x = _x + arg;
		pos._y = _y + arg;
		return pos;
	}

	bool operator==(const Position& arg)
	{
		return _x == arg._x && _y == arg._y;
	}

	Position& operator=(int arg)
	{
		_x = arg;
		_y = arg;

		//Position* this = �� �ڽ��� �ּ�;
		return *this;
	}

	// [���� ������] [���� ���� ������] �� Ư�� ��츦 �޴� ������,
	// �� �״�� ��ü�� '����'�Ǳ� ���ϴ� Ư¡ ����
	// TODO) �����Ҵ� �ð��� �� �ڼ��� �˾ƺ� ��
	Position& operator=(const Position& arg)
	{
		_x = arg._x;
		_y = arg._y;

		//Position* this = �� �ڽ��� �ּ�;
		return *this;
	}

	Position& const operator++()
	{
		_x++;
		_y++;
	}

	Position operator++(int)
	{
		Position ret = *this;
		_x++;
		_y++;
		return ret;
	}


public:
	int _x;
	int _y;
};

//void operator=(const Position& a, int b)
//{
//	a._x = b;
//	a._y = b;
//}


Position operator+(int a, const Position& b)
{
	Position ret;

	ret._x = b._x + a;
	ret._y = b._y + a;

	return ret;
}

int main()
{
	int a = 1;
	int b = 2;
	int c = a + 3.0f;

	int c = a + b;

	Position pos;
	pos._x = 0;
	pos._y = 0;

	Position pos2;
	pos2._x = 1;
	pos2._y = 1;

	Position pos3=pos+pos2;
	// pos3=pos.operator+(pos2);

	//Position pos4 = pos + 1;
	Position pos4 = 1 + pos;  //  <- ��� ������ �Լ� ver�δ� �̷��԰� �ȵ�. 

	bool isSame = (pos3 == pos4);

	 Position pos5;
	 pos3 = (pos5 = 5);

	 // (Pos&) ��~	(Pos)���簪 �ٰ�~
	 pos5 = pos3++;
	 pos3++;

	 ++(++pos3);

	return 0;
}