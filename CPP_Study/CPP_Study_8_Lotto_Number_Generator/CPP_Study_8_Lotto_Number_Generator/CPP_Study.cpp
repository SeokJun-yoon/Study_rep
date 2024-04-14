#include <iostream>
#include <time.h>
using namespace std;

// 오늘의 주제 : 로또 번호 생성기

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

#pragma region 내가 짠 코드
//void Sort(int numbers[], int count)
//{
//	
//	for (int i = 0; i < count; i++)
//	{
//		if (i + 1 >= count)
//			break;
//
//		else
//			for (int j = i; j < count; j++)
//			{
//				if (j + 1 >= count)
//					break;
//				else if (numbers[j] > numbers[j + 1])
//				{
//					Swap(numbers[j], numbers[j + 1]);
//				}
//				else
//					continue;
//			}
//	}
//}
//
//// 3) 로또 번호 생성
//void ChooseLotto(int numbers[])
//{
//	srand((unsigned)time(NULL)); // 랜덤 시드 설정
//
//
//	for (int i=0; i < 6; i++)
//	{
//		numbers[i] = (rand() % 45)+1;
//	}
//
//	Sort(numbers, 6);
//
//	for (int i = 0; i < 6; i++)
//	{
//		if (i + 1 >= 6)
//			break;
//
//		else
//			for (int j = i; j < 6; j++)
//			{
//				if (numbers[j] == numbers[j + 1])
//					numbers[j] = 1 + (rand() % 45);
//				else
//					continue;
//			}
//	}
//
//	Sort(numbers, 6);
//
//	// TODO : 랜덤으로 1~45 사이의 숫자 6개를 골라주세요! (단, 중복이 없어야 함)
//}
//
//
//int main()
//{
//	// 1) Swap 함수 만들기
//	int a = 1;
//	int b = 2;
//
//	Swap(a, b);
//	// a = 2, b = 1
//
//	// cout << a << " " << b << endl;
//
//	// 2) 정렬 함수 만들기 (작은 숫자가 먼저 오도록 정렬)
//
//	int numbers[6] = { 1,42,3,15,5,6 };
//	//Sort(numbers, sizeof(numbers) / sizeof(int));
//
//	//cout << "After sorting numbers : " ;
//	//for (int i = 0; i < sizeof(numbers) / sizeof(int); i++)
//	//	cout << numbers[i] << " ";
//	//cout << endl;
//	//// { 1, 3, 5, 6, 15, 42 }
//	ChooseLotto(numbers);
//	
//
//	return 0;
//}
//
#pragma endregion

#pragma region 강사님 코드

// 2) 정렬 함수 만들기 (작은 숫자가 먼저 오도록 정렬)
// { 1, 42, 3, 15, 5, 6 } => { 1, 3, 5, 6, 15, 42 }
void Sort(int numbers[], int count)
{
	for (int i = 0; i < count; i++)
	{
		// i번째 값이 제일 좋은 후보라고 가정
		int best = i;

		// 다른 후보와 비교를 통해 제일 좋은 후보를 찾아나선다
		for (int j = i + 1; j < count; j++)
		{
			if (numbers[j] < numbers[best])
				best = j;
		}
		
		// 제일 좋은 후보와 교체하는 과정
		if (i!=best)
			Swap(numbers[i], numbers[best]);
	}
}

// 3) 로또 번호 생성
void ChooseLotto(int numbers[])
{
	srand((unsigned)time(NULL)); // 랜덤 시드 설정
	
	int count = 0;
	while (count != 6)
	{
		int randValue = 1 + (rand() % 45);// 0~44

		// 이미 찾은 값인지?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == randValue)
			{
				// 이미 찾은 값
				found = true;
				break;
			}
		}

		// 못 찾았으면 추가!
		if (found == false)
		{
			numbers[count] = randValue;
			count++;
		}
	}

	Sort(numbers, 6);

	// TODO : 랜덤으로 1~45 사이의 숫자 6개를 골라주세요! (단, 중복이 없어야 함)
}

int main()
{
	// 1) Swap 함수 만들기
	int a = 1;
	int b = 2;
	Swap(a, b);
	// a = 2, b = 1

	// 2) 정렬 함수 만들기 (작은 숫자가 먼저 오도록 정렬)
	int numbers[6] = { 1, 42, 3, 15, 5, 6 };
	Sort(numbers, sizeof(numbers) / sizeof(int));
	// { 1, 3, 5, 6, 15, 42 }

	// 3) 로또 번호 생성기
	ChooseLotto(numbers);

	for (int i = 0; i < 6; i++)
		cout << numbers[i] << endl;
}
#pragma endregion