#include <iostream>
#include <time.h>
using namespace std;

// ������ ���� : �ζ� ��ȣ ������

void Swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

#pragma region ���� § �ڵ�
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
//// 3) �ζ� ��ȣ ����
//void ChooseLotto(int numbers[])
//{
//	srand((unsigned)time(NULL)); // ���� �õ� ����
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
//	// TODO : �������� 1~45 ������ ���� 6���� ����ּ���! (��, �ߺ��� ����� ��)
//}
//
//
//int main()
//{
//	// 1) Swap �Լ� �����
//	int a = 1;
//	int b = 2;
//
//	Swap(a, b);
//	// a = 2, b = 1
//
//	// cout << a << " " << b << endl;
//
//	// 2) ���� �Լ� ����� (���� ���ڰ� ���� ������ ����)
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

#pragma region ����� �ڵ�

// 2) ���� �Լ� ����� (���� ���ڰ� ���� ������ ����)
// { 1, 42, 3, 15, 5, 6 } => { 1, 3, 5, 6, 15, 42 }
void Sort(int numbers[], int count)
{
	for (int i = 0; i < count; i++)
	{
		// i��° ���� ���� ���� �ĺ���� ����
		int best = i;

		// �ٸ� �ĺ��� �񱳸� ���� ���� ���� �ĺ��� ã�Ƴ�����
		for (int j = i + 1; j < count; j++)
		{
			if (numbers[j] < numbers[best])
				best = j;
		}
		
		// ���� ���� �ĺ��� ��ü�ϴ� ����
		if (i!=best)
			Swap(numbers[i], numbers[best]);
	}
}

// 3) �ζ� ��ȣ ����
void ChooseLotto(int numbers[])
{
	srand((unsigned)time(NULL)); // ���� �õ� ����
	
	int count = 0;
	while (count != 6)
	{
		int randValue = 1 + (rand() % 45);// 0~44

		// �̹� ã�� ������?
		bool found = false;
		for (int i = 0; i < count; i++)
		{
			if (numbers[i] == randValue)
			{
				// �̹� ã�� ��
				found = true;
				break;
			}
		}

		// �� ã������ �߰�!
		if (found == false)
		{
			numbers[count] = randValue;
			count++;
		}
	}

	Sort(numbers, 6);

	// TODO : �������� 1~45 ������ ���� 6���� ����ּ���! (��, �ߺ��� ����� ��)
}

int main()
{
	// 1) Swap �Լ� �����
	int a = 1;
	int b = 2;
	Swap(a, b);
	// a = 2, b = 1

	// 2) ���� �Լ� ����� (���� ���ڰ� ���� ������ ����)
	int numbers[6] = { 1, 42, 3, 15, 5, 6 };
	Sort(numbers, sizeof(numbers) / sizeof(int));
	// { 1, 3, 5, 6, 15, 42 }

	// 3) �ζ� ��ȣ ������
	ChooseLotto(numbers);

	for (int i = 0; i < 6; i++)
		cout << numbers[i] << endl;
}
#pragma endregion