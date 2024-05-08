#include <iostream>
using namespace std;

// 오늘의 주제 : 호출 스택

// 함수 선언
// 어떤 기능인지는 모르지만, 우선적으로 선언해주어 존재를 알려준다.
void Func1();
void Func2(int hp, int mp); // 매개변수의 변수명은 꼭 아래 구현부와 같지 않아도 된다
void Func3(float a);

// 디버깅 할 때, 호출스택을 볼 줄 알아야한다.
// F5, F10(프로시져 단위), F11(한 줄씩) 기억

void Func1()
{
	cout << "Func1" << endl;
	Func2(1,2);
}

void Func2(int a, int b)
{
	cout << "Func2" << endl;
	Func3(10);
}

void Func3(float a)
{
	cout << "Func3" << endl;
}
int main()
{
	cout << "main" << endl;

	Func1();
	return 0;
}