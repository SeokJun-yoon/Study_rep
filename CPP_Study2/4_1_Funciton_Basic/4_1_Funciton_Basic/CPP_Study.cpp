#include <iostream>
using namespace std;

// 오늘의 주제 : 함수(프로시저, 메소드, 루틴)

/*
	input으로 무엇을 받고, output으로 무엇을 뱉을지 정해준다.

반환타입 함수이름([인자타입 매개변수])
{
	함수 내용

	return ---;
}
*/

// Hello World를 콘솔에 출력하는 함수를 만들어보자!
// input : 없음 / output : 없음
// 타입 : int float double char~~ void

// 함수 이름은 일반적으로 첫 글자 대문자로 네이밍
void PrintHelloWorld(void) // input의 void는 생략 가능
{
	cout << "Hello World" << endl;

	return;

	// 리턴 뒤의 코드는 유효하지 않다.
}

// 정수를 입력(함수) 받아서, 콘솔에 출력하는 함수를 만들어보자.
// input : int // output : 없음
void PrintNumber(int number)
{
	cout << "넘겨주신 숫자는 " << number << " 입니다" << endl;
}

// 2를 곱하고 그 결과를 뱉는 함수를 만들어보자.
int MultiplyBy2(int a)
{
	int b = a * 2;
	return b;
}

// 두 숫자를 곱해서 결과물을 뱉는 함수를 만들어보자
// input : int, int / output : int

int MultiplyBy(int a, int b)
{
	int c=a*b;
	return c;
}

int main()
{
	int a=3;
	int b=5;
	int result = MultiplyBy(b,a); // 함수의 매개변수로 사용한 변수는 그저 순서의 의미이다. 위에서 선언한 변수랑은 상관이 없음
	PrintNumber(result);
	//PrintHelloWorld();
}