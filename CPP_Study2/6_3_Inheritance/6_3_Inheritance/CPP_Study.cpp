#include <iostream>
using namespace std;

// 오늘의 주제 : 상속성

// 객체지향 (OOP object Oriented Programming)
// - 상속성
// - 은닉성
// - 다형성

struct StatInfo
{
	int hp;
	int attack;
	int defence;
};

// 메모리
// 
// 
// [   Knight   ] - 4바이트 - 2층
// [ [ Player ] ] - 12바이트 - 1층
// 건물을 쌓고 해체하는 순서처럼 부모 클래스의 생성자 먼저 , 소멸은 자식 클래스 먼저

// 상속(Inheritance) ? 부모 -> 자식에게 유산을 물려주는 것

// 생성자(N)/소멸자(1)

// 생성자는 탄생을 기념해서 호출되는 함수?
// - Knight를 생성하면 -> Player의 생성자? Knight의 생성자?
// -> 솔로몬의 선택 ! 그냥 둘 다 호출하자!
// ★ 생성자는 각자 호출이 된다!

// 실제 게임 구조에서?
// GameObject
// - Creature
// -- Player, Monster, Npc, Pet
// - Projectile
// -- Arrow, Fireball
// - Env

// Item
// - Weapon
// -- Sword, Bow
// - Armor
// -- Helmet, Boots, Armor
// - Consumable
// -- Potion, Scroll

class GameObject
{
public:
	int _objectid;
};

class Player : public GameObject
{
public:
	Player()
	{
		_hp=0;
		_attack=0;
		_defence=0;
		cout << "Player() 기본 생성자 호출" << endl;
	}

	Player(int hp)
	{
		_hp = 0;
		_attack = 0;
		_defence = 0;
		cout << "Player(int hp) 생성자 호출" << endl;
	}

	~Player()
	{
		cout << "~Player() 소멸자 호출" << endl;
	}

	void Move() { cout << "Player Move 호출" << endl; }
	void Attack() { cout << "Player Attack 호출" << endl; }
	void Die() { cout << "Player Die 호출" << endl; }

public:
	int _hp;
	int _attack;
	int _defence;
};

// class는 일종의 설계도
class Knight : public Player
{
public:
	Knight()
	{
		/*
		선(먼저)처리 영역
		- 여기서 Player() 생성자를 호출
		*/

		_stamina=100;
		cout << "Knight() 기본 생성자 호출" << endl;
	}

	Knight(int stamina) : Player(100)
	{
		/*
			선(먼저)처리 영역
			- 여기서 Player(int hp) 생성자를 호출
		*/

		_stamina=stamina;
		cout << "Knight(int stamina) 생성자 호출" << endl;
	}

	~Knight()
	{
		cout << "~Knight() 소멸자 호출" << endl;
	}
	/*
		후(나중에)처리 영역
		- 여기서 ~Player() 소멸자를 호출
	*/

	// 재정의 : 부모님의 유산을 거부하고 새로운 이름으로 만든?
	void Move() { cout << "Knight Move 호출" << endl; }

public :
	int _stamina;
};

class Mage : public Player
{
public:

public:
	int _mp;
};

int main()
{
	Knight k(100);

	k._hp=100;
	k._attack=10;
	k._defence=5;
	k._stamina=50;

	//k.Move();	// 재정의한 Move함수(즉, 부모함수의 Move가 아님)가 사용된다
	// k.Player::Move() <- 사실상 쓸 일은 없음.
	// 그냥 부모 class의 설계를 잘할 것 

	//k.Attack();
	//k.Die();

	return 0;
}