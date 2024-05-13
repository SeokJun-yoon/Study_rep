#include <iostream>
using namespace std;

// 오늘의 주제 : 참조

struct StatInfo
{
	int hp;	// +0
	int attack;	// +4
	int defence; // +8
};

// [매개변수][RET][지역변수] [매개변수(&info)][RET][지역변수]
void CreateMonster(StatInfo* info)
{
	info->hp = 100;
	info->attack = 8;
	info->defence = 5;
}

// [매개변수][RET][지역변수] [매개변수(info(100, 8, 5))][RET][지역변수]
// 복사본만 건드린 것이므로, 함수 리턴시에 날아가버린다
void CreateMonster(StatInfo info)
{
	info.hp = 100;
	info.attack = 8;
	info.defence = 5;
}

// 값을 수정하지 않는다면, 양쪽 다 일단 문제 없음

// 1) 값 전달 방식
// [매개변수][RET][지역변수(info)] [매개변수(info)][RET][지역변수]
// 출력의 용도에서는 복사본을 써도 문제가 없음
void PrintInfoByCopy(StatInfo info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATT: " << info.attack << endl;
	cout << "DEF: " << info.defence << endl;
	cout << "---------------------------" << endl;
}

// 2) 주소 전달 방식
// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void PrintInfoByPtr(StatInfo* info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info->hp << endl;
	cout << "ATT: " << info->attack << endl;
	cout << "DEF: " << info->defence << endl;
	cout << "---------------------------" << endl;
}

// StatInfo 구조체가 1000바이트짜리 대형 구조체라면?
// - (값 전달) StatInfo로 넘기면 1000바이트가 복사되는
// - (주소 전달) StatInfo*는 8바이트 (x64 기준)
// 즉 포인터를 이용한 주소 전달 방식이 성능 차이를 많이 낼 수 있다
// - (참조 전달) StatInfo&는 8바이트

// 3) 참조 전달 방식
// 값 전달처럼 편리하게 사용하고!
// 주소 전달처럼 주소값을 이용해 진본을 건드리는
// 일석이조의 방식!

void PrintInfoByRef(StatInfo& info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATT: " << info.attack << endl;
	cout << "DEF: " << info.defence << endl;
	cout << "---------------------------" << endl;
}


int main()
{
	// 4바이트 정수형 바구니를 사용할거야.
	// 앞으로 그 바구니 이름을 number라고 할게.
	// 그러니까 number에서 뭘 꺼내거나, number에 뭘 넣는다고 하면
	// 찰떡같이 알아듣고 해당주소에 1을 넣어주면 된다!
	int number = 1;

	// * 주소를 담는 바구니
	// int 그 바구니를 따라가면 int 데이터(바구니)가 있음
	int* pointer = &number;
	// pointer 바구니에 있느느 주소를 타고 이동해서, 그 멀리 있는 바구니에 2를 넣는다
	*pointer = 2;

	// 로우레벨(어셈블리) 관점에서 실제 작동 방식은 int*와 똑같음
	// 실제로 실행해보면 포인터랑 100% 똑같다
	int& reference = number;

	// C++ 관점에서는 number라는 바구니에 또 다른 이름을 부여한 것.
	// number라는 바구니에 reference라는 다른 이름을 지어줄게~
	// 앞으로 reference 바구니에 뭘 꺼내거나 넣으면,
	// 실제 number 바구니(진퉁에다가) 그 값을 꺼내거나 넣으면 됨!
	reference = 3;
	
	// 그런데 귀찮게 또 다른 이름을 짓는 이유는?
	// 그냥 number = 3이라고 해도 똑같은데...
	// 참조 전달 때문!

	StatInfo info;
	CreateMonster(&info);

	PrintInfoByCopy(info);
	PrintInfoByPtr(&info);
	PrintInfoByRef(info);
	return 0;
}