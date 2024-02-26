#include <iostream>
using namespace std;

int main()
{
#pragma region 별찍기_1
	//// 별찍기()
	//// 유저들이 어떤 정수를 입력하면
	//// N*N개의 별을 찍었으면 좋겠다.
	//int n;

	//cout << "N*N의 별찍기를 할 N을 입력하세요: ";
	//cin >> n ;

	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = 0; j < n; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}
#pragma endregion


#pragma region 별찍기_2
//	// 별찍기_2
//	// 1개부터 시작해서 순차적으로 줄마다 별이 증가하게 수정
//	int n;
//	cout << "N개의 줄에 N개씩 별찍기를 할 N을 입력하세요: ";
//
//	cin >> n ;
//
//	for (int i = 0; i < n; i++)
//		{
//			for (int j = 0; j < i+1; j++)
//				{
//					cout << "*";
//				}
//		cout << endl;
//}
#pragma endregion

#pragma region 별찍기_3
//	// 별찍기_3
//	// N개부터 시작해서 순차적으로 줄마다 별이 1개씩 줄어들게 수정
//	int n;
//	cout << "N개의 줄에서 1개씩 줄어드는 별찍기를 할 N을 입력하세요: ";
//
//	cin >> n ;
//
//	for (int i = 0; i < n; i++)
//		{
//			// 0번째줄 = 4개
//			// 1번째줄 = 3개
//			// 2번째줄 = 2개
//			// i번째 줄 = 4-i개
//			for (int j = 0; j < n-i; j++)
//				{
//					cout << "*";
//				}
//		cout << endl;
//}
#pragma endregion

#pragma region 구구단_출력
	// 구구단)
	// 2*1=2
	// 2단부터~9단까지 구구단을 출력해주세요

	for (int i = 2; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			int answer = i * j;
			cout << i << " * " << j << " = " << answer << endl;
		}
		cout << endl;
	}
#pragma endregion
}