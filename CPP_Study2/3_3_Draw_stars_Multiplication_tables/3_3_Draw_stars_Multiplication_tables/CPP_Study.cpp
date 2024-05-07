#include <iostream>
using namespace std;

// cout -> console out
// endl -> endline
int main()
{
	// 별찍기 (1)
	// 유저들이 어떤 정수를 입력하면 N*N개의 별을 찍기

	// solution)
	//int input;
	//cin >> input;	// 입력 받기
	//
	//for (int i = 0; i < input; i++)
	//{
	//	for (int j = 0; j < input; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}

	// 별찍기 (2)
	// 1개부터 시작해서 순차적으로 줄마다 증가하게 수정

	// solution)
	//int input;
	//cin >> input;

	//for (int i = 0; i < input; i++)
	//{
	//	for (int j = 0; j < i + 1; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}
	
	// 별찍기 (3)
	// N개부터 시작해서 줄마다 1개씩 줄어드는 형태로

	//int input;
	//cin >> input;

	//for (int i = 0; i < input; i++)
	//{
	//	// N=4 일 경우,
	//	// 0번째줄 = 4개
	//	// 1번째줄 = 3개
	//	// 2번째줄 = 2개
	//	// i번째줄 = 4-i개
	//	for (int j = 0; j < (input - i); j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}


	// 구구단
	// 2 * 1 = 2
	// 2단부터~9단까지 구구단을 출력하기

	// solution)
	for (int i = 2; i <= 9; i++)
	{
		for (int j = 1; j <= 9 ; j++)
		{
			cout << i << " * " << j << " = " << i*j << endl;
		}
	}
	
}