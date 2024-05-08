#include <iostream>
using namespace std;

// 오늘의 주제 : 함수 마무리

// 오버로딩 (중복 정의 : 함수 이름의 재사용)
// - 매개변수 갯수가 다르거나
// - 매개변수 타입이 다르거나 (순서가 다른걸 포함)

int Add(int a, int b)
{
	int result = a + b;
	return result;
}

// float Add(int a, int b)는 불가.
// 반환형식만 다른 경우(매개변수가 같고)에는 불가함
float Add(float a, float b)
{
	float result = a + b;
	return result;
}

// 기본 인자값
// 매개변수 값에 기본값을 부여하면, 함수 사용시에 적지 않아도 기본값으로 세팅이 된다. 대신 꼭 가장 뒷쪽에 들어가야 한다.
void SetPlayerInfo(int hp, int mp, int attack, int gulidid = 0, int castleid=0)
{

}

// 스택 오버플로우
// 너무 많은 호출을 하지 않도록
int Factorial(int n)
{
	if (n<=1)
		return 1;

		return n * Factorial(n-1);
}

int main()
{
	//float result = Add(1.5f, 2.1f);
	//cout << result << endl;

	//SetPlayerInfo(100,40,10,0,1);
	
	int result = Factorial(1000000);
	cout << result << endl;
	return 0;
}