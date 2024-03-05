#include <iostream>
using namespace std;

// 오늘의 주제 : 포인터 vs 참조

struct StatInfo
{
	int hp; // +0
	int attack; // +4
	int defence; // +8
};

// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 8;
	info->defence = 5;
}


// 2) 주소 전달 방식
// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void PrintInfo(StatInfo* info)
{
	cout << "---------------------" << endl;
	cout << "HP: " << info->hp << endl;
	cout << "ATTACK: " << info->attack << endl;
	cout << "DEFENCE: " << info->defence << endl;
	cout << "---------------------" << endl;
}

// StatInfo 구조체가 1000바이트짜리 대형 구조체라면?
// - (값 전달) StatInfo로 넘기면 1000바이트가 복사되는
// - (주소 전달) StatInfo*는 8바이트
// - (참조 전달) StatInfo&는 8바이트

// 3) 참조 전달 방식
void PrintInfo(StatInfo& info)
{
	cout << "---------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATTACK: " << info.attack << endl;
	cout << "DEFENCE: " << info.defence << endl;
	cout << "---------------------" << endl;
}


// 값 전달처럼 편리하게 사용하고!
// 주소 전달처럼 주소값을 이용해 진퉁을 건드리는!
// 일석이조의 방식!


int main()
{
	// 4바이트 정수형 바구니를 사용할거야.
	// 앞으로 그 바구니 이름을 number라고 할게
	// 그러니까 number에서 뭘 꺼내거나, number에 뭘 넣는다고 하면
	// 찰떡같이 알아듣고 해당 주소에 1을 넣어주면 된다.

	int number = 1;

	// * 주소를 담는 바구니
	// int 그 바구니를 따라가면 int 데이터(바구니)가 있음
	int* pointer = &number;
	// pointer 바구니에 있는 주소를 타고 이동해서, 그 멀리 있는 바구니에 2를 넣는다
	*pointer = 2;


	// Low Level(어셈블리) 관점에서 실제 작동 방식은 int*와 똑같음
	int& reference = number;

	// C++ 관점에서는 number라는 바구니에 또 다른 이름을 부여한 것.
	// number라는 바구니에 reference라는 다른 이름을 지어줄게
	// 앞으로 reference 바구니에다가 뭘 꺼내거나 넣으면,
	// 실제 number 바구니(진퉁에다가) 그 값을 꺼내거나 넣으면 됨!
	reference = 3;

	// 그런데 귀찮게 또 다른 이름을 짓는 이유는?
	// 그냥 number = 3이라고 해도 똑같은데...
	// 참조 전달 때문!

	StatInfo info;

	// 포인터 vs 참조 세기의 대결
	// 성능 : 똑같음!
	// 편의성 : 참조 승!

	// 1) 편의성 관련
	// 편의성이 좋다는게 꼭 장점만은 아니다.
	// 포인터는 주소를 넘기니 확실하게 원본을 넘긴다는 힌트를 줄 수 있는데.
	// 참조는 자연스럽게 모르고 지나칠 수도 있음!


	CreateMonster(&info);
	
	PrintInfo(&info);

	PrintInfo(info);

	return 0;
}