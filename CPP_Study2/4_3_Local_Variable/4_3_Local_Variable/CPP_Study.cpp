#include <iostream>
using namespace std;

// 오늘의 주제 : 지역 변수와 값 전달

// 전역 변수 (Data 영역에 들어감)
// 참고) 초기화 여부, const 등 .rodata .data .bss
// 전역 변수로 선언하는 것은 지양할 것. 코드가 복잡해질 수록 잘못 건드리거나 실수할 확률이 높아짐. 지역변수를 잘 활용할 것
int globalValue=0;

void Test()
{
	cout << "전역 변수의 값은 " << globalValue << endl;
	globalValue++;
}

void IncreaseHp(int hp)
{
	hp=hp+1;
}

int main()
{
	cout << "전역 변수의 값은 " << globalValue << endl;
	globalValue++;

	//Test();

	int hp=1;
	cout << "Increase 호출 전 : " << hp <<endl;
	IncreaseHp(hp);
	cout << "Increase 호출 후 : " << hp << endl;
	// 지역 변수
	int localValue = 0;

	return 0;
}