#include <iostream>
using namespace std;
#include <iomanip>

// ������ ���� : ���� ����

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

#pragma region �� Ǯ��
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

#pragma region ����� Ǯ��

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

		// dy[], dx[]�� ���� �ȴٸ� ���� switch(dir) �κ��� �ʿ䰡 ������.
		// �ڵ� ȿ��ȭ ��Ű��
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
			dir = (dir + 1) % 4;	// �ڵ带 ȿ�������� © �� �ִ�.
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
	cout << "���� �Է� : ";
	cin >> N;

	SetBoard();

	PrintBoard();

	return 0;
}

#pragma endregion
