#include <iostream>
using namespace std;
#include <iomanip>

// 오늘의 주제 : 연습 문제

const int MAX = 100;
int board[MAX][MAX] = {};
int N;

void PrintBoard()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			cout << setfill('0') << setw(2) << board[y][x] << " ";
		}
		cout << endl;
	}
}

#pragma region 내 풀이
//void SetBoard()
//{
//	if (N % 2 == 0)
//	{
//	int value = 0;
//
//		for (int t = 0; t < (N / 2); t++)
//		{
//			for (int i = 0; i < N-1-(t*2); i++)
//			{
//				value++;
//				board[0+t][i+t] = value;	
//			}
//		
//			for (int j = 0; j < N-1- (t * 2); j++)
//			{
//				value++;
//				board[j + t][N - 1 - t] = value;
//			}
//		
//			for (int k = 0; k < N-1- (t * 2); k++)
//			{
//				value++;
//				board[N - 1 - t][N - 1 - k - t] = value;
//			}
//			
//			for (int l = 0; l < N - 1- (t * 2); l++)
//			{
//				value++;
//				board[N - 1 - l - t][0 + t] = value;
//			}
//
//		}
//
//	}
//	else
//	{
//		int value = 0;
//
//		for (int t = 0; t < (N / 2); t++)
//		{
//			for (int i = 0; i < N - 1 - (t * 2); i++)
//			{
//				value++;
//				board[0 + t][i + t] = value;
//			}
//
//			for (int j = 0; j < N - 1 - (t * 2); j++)
//			{
//				value++;
//				board[j + t][N - 1 - t] = value;
//			}
//
//			for (int k = 0; k < N - 1 - (t * 2); k++)
//			{
//				value++;
//				board[N - 1 - t][N - 1 - k - t] = value;
//			}
//
//			for (int l = 0; l < N - 1 - (t * 2); l++)
//			{
//				value++;
//				board[N - 1 - l - t][0 + t] = value;
//			}
//
//		}
//		board[(N/2)][N/2] = value + 1;
//	}
//
//}
#pragma endregion

#pragma region 강사님 풀이
void SetBoard()
{

}
#pragma endregion

int main()
{
	cout << "숫자 입력 : ";
	cin >> N;

	SetBoard();

	PrintBoard();

	return 0;
}