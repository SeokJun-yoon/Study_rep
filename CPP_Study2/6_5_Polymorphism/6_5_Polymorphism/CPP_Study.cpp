#include <iostream>
using namespace std;

// 오늘의 주제 : 다형성

// 객체지향 (OOP Object Oriented Programming)
// - 상속성
// - 은닉성 = 캡슐화
// - 다형성

// 다형성(Polymorphism = Poly + morph = 겉은 똑같은데, 기능이 다르게 동작한다)
// - 오버로딩(Overloading) = 함수 중복 정의 = 함수 이름의 재사용
// - 오버라이딩(Overriding) = 재정의 = 부모 클래스의 함수를 자식 클래스에서 재정의

// 바인딩(Binding) = 묶는다
// - 정적 바인딩(Static Binding) : 컴파일 시점에 결정
// - 동적 바인딩(Dynamic Binding) : 실행 시점에 결정

// 일반 함수는 정적 바인딩을 사용
// 동적 바인딩을 원한다면? -> 가상 함수 (virtual function)

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
	// 같은 형태의 함수를 만들어서 호출하면 상속받은 함수가 아닌 위 함수가 호출된다.
	
	// 가상 함수는 재정의를 하더라도 가상 함수다!
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
	//MovePlayer(&p); // 플레이어는 플레이어다? YES
	//MoveKnight(&p); // 플레이어는 기사다? NO
	//p.Move();
	//p.Move(100);

	Knight k;
	//MoveKnight(&k); // 기사는 기사다? YES
	MovePlayer(&k); // 기사는 플레이어다? YES
	// Move 'Player'가 호출된다

	return 0;
}