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

enum DIR
{
	RIGHT=0,
	DOWN=1,
	LEFT=2,
	UP=3,
};

bool CanGo(int y, int x)
{
	if (y < 0 || y >= N)
		return false;
	if (x < 0 || x >= N)
		return false;
	if (board[y][x] != 0)
		return false;
	return true;
}

void SetBoard()
{
	int dir = RIGHT;
	int num = 1;
	int y = 0;
	int x = 0;

	int dy[] = { 0,1,0,-1 };
	int dx[] = { 1,0,-1,0 };


	while (true)
	{
		board[y][x] = num;

		if (num == N * N)
			break;

		int nextY = y + dy[dir];
		int nextX = x + dx[dir];

		// dy[], dx[]를 쓰게 된다면 굳이 switch(dir) 부분이 필요가 없어짐.
		// 코드 효율화 시키기
		//switch (dir)
		//{
		//case RIGHT:
		//	nextY = y;
		//	nextX = x + 1;
		//	break;
		//case DOWN:
		//	nextY = y + 1;
		//	nextX = x;
		//	break;
		//case LEFT:
		//	nextY = y;
		//	nextX = x - 1;
		//	break;
		//case UP:
		//	nextY = y - 1;
		//	nextX = x;
		//	break;
		//}

		if (CanGo(nextY, nextX))
		{
			y = nextY;
			x = nextX;
			num++;
		}
		else
		{
			dir = (dir + 1) % 4;	// 코드를 효율적으로 짤 수 있다.
			/*switch (dir)
			{
			case RIGHT:
				dir = DOWN;
				break;
			case DOWN:
				dir = LEFT;
				break;
			case LEFT:
				dir = UP;
				break;
			case UP:
				dir = RIGHT;
				break;
			}*/
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

#pragma endregion
