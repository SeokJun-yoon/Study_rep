#include <iostream>
using namespace std;

// 오늘의 주제 : 반복문
// 데이터를 메모리에 할당하고 가공하는 방법에 대해 알아봄
// 가공한 데이터를 이용해서 무엇인가를 하고 싶다면?

// 코딩은 사실상 분기문과 반복문만 잘 쓰면 된다	

int main()
{
	// - while 문
	// while - 동안에
	// if-else 굉장히 유용하다

	// 하지만 한 번만 실행하는게 아니라, 특정 조건까지 계속 반복해야 하는 상황
	// ex) 게임을 끌 때까지 계속 게임을 실행해라
	// ex) 목적지에 도달할 때까지- 계속 이동하라

	// 마찬가지로 while 한 칸띄고 (조건문) 형태가 가장 통상적
	// while문도 마찬가지로 중괄호로 묶음
	// 보통 팀 컨벤션에 따라 다르긴 하지만 
	// while (1)
	// {
	// }
	// 위와 같은 형태가 많음 (큰 상관은 X)

	int count = 0;

	while (count < 5)
	{
		count++;
		cout << "Hello World" << endl;
	}

	// while과 do-while의 차이?
	// 한 번이라도 실행을 하느냐 안하느냐의 차이
	// do-while 문은 우선 한 번은 실행하고 조건식 진입
	//do
	//{
	//	cout << "Hello World" << endl;
	//	count++;
	//} while (count < 5);
	// do-while의 사용 빈도는 잦지 않으나
	// 읽을줄은 알아야 한다 정도

	// - for 문
	// 사실상 코딩을 할 때에는 거의 9:1 비율로 for문을 많이 사용하게 될 것임
	// for (초기식(1); 조건식(2); 제어식(4))
	// {
	//	 cout << "Hello World" << endl (3)
	// }

	// 루프문의 흐름 제어 break; continue;

	// break 등장 시 즉시 반복문을 빠져나감
	// 중첩 while, for에서 break 등장 시 중괄호 범위안에 속해 있는 반복문을 빠져나가는 것
	// continue는 skip의 기능

	//for (int count = 0; count < 5; count++)
	//{
	//	cout  << "Hello World" << endl;
	//}

	int round = 1;
	int hp = 100;
	int damage = 27;

	// 무한 루프 : 전투 시작
	while (true)
	{
		hp -= damage;
		if (hp < 0)
			hp = 0; // 음수 체력을 0으로 보정

		// 시스템 메시지
		cout << "Round " << round << " 몬스터 체력 " << hp << endl;

		if (hp == 0)
		{
			cout << "몬스터 처치!" << endl;
			break;	// while 문을 나가라는 의미
		}

		if (round == 5)
		{
			cout << "제한 라운드 종료" << endl;
			break;
		}

		round++;
	}

	// ★ 정해진 횟수만큼 실행한다면? -> for문 선택
	// for문같은 경우에는 직관적으로 for문안의 내용이 보이기 때문에 실수를 줄일 수 있다.


	// 1-10 사이의 홀수만 출력하기

	for (int count = 0; count <= 10; count++)
	{
		bool isEven=((count%2)==0);

		if (isEven)
			continue;	// continue를 활용해 맞지 않는 조건 skip

		cout << count << endl;
	}
}