#include <iostream>
using namespace std;

class Knight
{
public:
	// [1] 기본 생성자 (인자가 없음)
	Knight()
	{
		cout << "Knight() 기본 생성자 호출" << endl;
	}

	// [2] 복사 생성자 (자기 자신의 클래스 참조 타입을 인자로 받음)
	// 일반적으로 '똑같은' 데이터를 지닌 객체가 생성되길 기대한다
	Knight(const Knight& knight)
	{
		_hp = knight._hp;
		_attack = knight._attack;
		_posX = knight._posX;
		_posY = knight._posY;
		cout << "Knight() 복사 생성자 호출" << endl;
	}

	// [3] 기타 생성자
	// 이 중에서 인자를 1개만 받는 [기타 생성자]를
	// [타입 변환 생성자]라고 부르기도 함
	Knight(int hp)
	{
		cout << "Knight() 생성자 호출" << endl;
		_hp = hp;
	}

	Knight(int hp, int attack, int posX, int posY)
	{
		_hp = hp;
		_attack = attack;
		_posX = posX;
		_posY = posY;
	}

	// [4] 소멸자
	~Knight()
	{
		cout << "Knight(int) 소멸자 호출" << endl;
	}

public:
	// 멤버 변수
	int _hp;
	int _attack;
	int _posX;
	int _posY;

public:
	// 멤버 함수
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
