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

// 특정 조건을 만족하는 몬스터를 찾는 함수
StatInfo* FindMonster()
{

}


StatInfo globalInfo;
// 2) 주소 전달 방식
// [매개변수][RET][지역변수(info)] [매개변수(&info)][RET][지역변수]
void PrintInfo(const StatInfo* const info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info->hp << endl;
	cout << "ATT: " << info->attack << endl;
	cout << "DEF: " << info->defence << endl;
	cout << "---------------------------" << endl;

	// 별 뒤에 붙인다면?
	// StatInfo* const info
	// info라는 바구니의 내용물[주소]을 바꿀 수 없음
	// info는 주소값을 갖는 바구니 -> 이 주소값이 고정이다!

	// 별 이전에 붙인다면?
	// const StaticInfo* info
	// info가 '가리키고 있는' 바구니의 내용물을 바꿀 수 없음
	// '원격' 바구니의 내용물을 바꿀 수 없음

	// info[ 주소값 ]	주소값[ 데이터 ]
	// info=&globalInfo;

	// info->hp=10000;
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

void PrintInfo(const StatInfo& info)
{
	cout << "---------------------------" << endl;
	cout << "HP: " << info.hp << endl;
	cout << "ATT: " << info.attack << endl;
	cout << "DEF: " << info.defence << endl;
	cout << "---------------------------" << endl;

	// 신입이 왔다
	//info.hp=10000;	const를 사용해서 막음
}


int main()
{
	StatInfo info;

	CreateMonster(&info);

	// 포인터 vs 참조 세기의 대결
	// 성능 : 똑같음!
	// 편의성 : 참조 승!

	// 1) 편의성 관련
	// 편의성이 좋다는게 꼭 장점만은 아니다.
	// 포인터는 주소를 넘기니 확실하게 원본을 넘긴다는 힌트를 줄 수 있는데
	// 참조는 자연스럽게 모르고 지나칠 수도 있음!
	// ex) 마음대로 고친다면?
	// const를 사용해서 이런 마음대로 고치는 부분 개선 가능

	// 참고로 포인터도 const를 사용 가능
	// * 기준으로 앞에 붙이느냐, 뒤에 붙이느냐에 따라 의미가 달라진다.

	// 2) 초기화 여부
	// 참조 타입은 바구니의 2번째 이름 (별칭?)
	// -> 참조하는 대상이 없으면 안됨
	// 반면 포인터는 그냥 어떤~ 주소라는 의미
	// -> 대상이 실존하지 않을 수도 있음
	// 포인터에서 '없다'는 의미로 ? nullptr
	// 참조 타입은 이런 nullptr

	StatInfo* pointer = nullptr; // 어떠한 주소도 가리키고 있지 않은 상태다 -> nullptr
	pointer=&info;
	PrintInfo(pointer);

	StatInfo& reference=info;
	PrintInfo(reference);
	return 0;
}