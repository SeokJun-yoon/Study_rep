#include <iostream>
using namespace std;

// 오늘의 주제 : 함수(프로시저, 메소드, 루틴)

/*

input으로 무엇을 받고, output으로 무엇을 받을지 정해준다.

반환타입 함수이름([인자타입 매개변수])
{
	함수 내용

	return ---;
}
*/

// Hello World를 콘솔에 출력하는 함수를 만들어보자!
// input : 없음 / output : 없음
// 타입 : int float double char~~ void 

void PrintHelloWorld()
{
	cout << "Hello World!" << endl;
	
	return;
}

// 정수를 입력(함수) 받아서, 콘솔에 출력하는 함수를 만들어보자
// input : int // output : 없음
void PrintNumber(int number)
{
	cout << "넘겨주신 숫자는 " << number << " 입니다." << endl;
}

// 2를 곱하고 결과물을 뱉는 함수를 만들어보자
// input : int / output : int

int MultiplyBy2(int a)
{
	int b = a * 2;
	return b;
}

// 두 숫자를 곱해서 결과물을 뱉는 함수를 만들어보자
// input : int, int / output : int

int MultiplyBy(int a, int b)
{
	int c = a * b;
	return c;
}

// 디버깅 할 때,
// F5 - 중단점을 활용하여 걸리는지 확인하기 위해서
// F10 - 프로시저(함수) 단위로 진행 -> 세부적인 부분까지 들어가지 않음
// F11 - 한 단계씩 코드 진행 -> 함수 안까지 진입함

int main()
{
	int result = MultiplyBy(3, 5);
	PrintNumber(result);
	//PrintHelloWorld();

	return 0;
}