#include <iostream>
#include <time.h>
using namespace std;


int main()
{
	srand(time(NULL)); // 시드 설정
	
	// 1 2 3
	
	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;

	
	int game_count=0;	//	판 수
	int win_count = 0;	//	이긴 횟수
	

	while (1)
	{
		cout << "가위(1) / 바위(2) / 보(3) 골라주세요!" << endl;
		cout << "> ";

		if (game_count == 0)
		{
			cout << "현재 승률은 없습니다." << endl;
		}
		else
		{
			int win_rate = (win_count * 100) / game_count; // 승률
			cout << "현재 승률은 " << win_rate << "% 입니다." << endl;
		}

		// 컴퓨터
		int computervalue = 1 + (rand() % 3);	//	컴퓨터가 낸 값(1~3)

		// 사용자
		int input;	// 사용자의 입력값
		cin >> input;
		switch (input)
		{
	
		case 1:
			if (computervalue == SCISSORS)
			{
				cout << "가위(플레이어)" << " vs " << "가위(컴퓨터) " << "<< 비겼습니다! >> " << endl;
				break;
			}
			else if (computervalue==ROCK)
			{
				cout << "가위(플레이어)" << " vs " << "바위(컴퓨터) " << "<< 졌습니다! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "가위(플레이어)" << " vs " << "보(컴퓨터) " << "<< 이겼습니다! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else
				break;
			
		case 2:
			if (computervalue == SCISSORS)
			{
				cout << "바위(플레이어)" << " vs " << "가위(컴퓨터) " << "<< 이겼습니다! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == ROCK)
			{
				cout << "바위(플레이어)" << " vs " << "바위(컴퓨터) " << "<< 비겼습니다! >> " << endl;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "바위(플레이어)" << " vs " << "보(컴퓨터) " << "<< 졌습니다! >> " << endl;
				game_count++;
				break;
			}
			else
				break;

		case 3:
			if (computervalue == SCISSORS)
			{
				cout << "보(플레이어)" << " vs " << "가위(컴퓨터) " << "<< 졌습니다! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == ROCK)
			{
				cout << "보(플레이어)" << " vs " << "바위(컴퓨터) " << "<< 이겼습니다! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "보(플레이어)" << " vs " << "보(컴퓨터) " << "<< 비겼습니다! >> " << endl;
				break;
			}
			else
				break;

		default:
			cout << "잘못된 값 입력. 게임을 종료합니다." << endl;
			return 0;
		}

	}
}