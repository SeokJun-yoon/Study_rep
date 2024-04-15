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

void SetBoard()
{
	if (N % 2 == 0)
	{
		for (int t = 0; t < (N / 2); t++)
		{
			for (int i = 0; i < N; i++)
			{
				board[0+t][i+t] = i + 1;
			}

			for (int j = 0; j < N; j++)
			{
				board[j+t][N - 1+t] = N + j;
			}

			for (int k = 0; k < N; k++)
			{
				board[N - 1+t][N - 1 - k+t] = (N * 2 - 1) + k;
			}

			for (int l = 0; l < N - 1; l++)
			{
				board[N - 1 - l+t][0+t] = (1 + (N - 1) * 3) + l;
			}

		}

	}
}

int main()
{
	cout << "숫자 입력 : ";
	cin >> N;

	SetBoard();

	PrintBoard();

	return 0;
}