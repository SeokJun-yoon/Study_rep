#include <iostream>
using namespace std;

// 오늘의 주제 : 분기문
// 데이터를 메모리에 할당하고 가공하는 방법에 대해서는 알아봄
// 가공한 데이터를 이용해서 무엇인가를 하고 싶다면?

int main()
{
	int hp = 100; // 몬스터 HP
	int damage = 100; // 플레이어 데미지

	hp -= damage; // 피격 판정
	bool isDead = (hp <= 0); // 처치 판정


	// 몬스터가 죽었으면 경험치 추가
	// 어셈블리에서 CMP, JMP


	// 코딩 스타일의 경우, 일반적으로 if 한칸 띄고 (조건)
	// 취향의 영역이긴 하다

	// if (isDead==true)
	// if문 중괄호로 묶지 않았을 경우, 바로 밑에 한 줄까지만 유효
	if (isDead)
		cout << "몬스터를 처치했습니다." << endl;

	// if (!isDead)
	// if/else 안에 if/else문을 중첩으로도 사용이 가능
	// 가독성을 고려하여 너무 많은 중첩 X
	// else if도 사용하면 가독성에 도움이 됨. 

	//else
	//{
	//	// TODO
	//	if (hp<20)
	//		cout << "몬스터가 도망가고 있습니다" << endl;

	//	else
	//		cout << "몬스터가 반격했습니다." << endl;
	//}
	else if (hp <= 20)
		cout << "몬스터가 도망가고 있습니다" << endl;
	else
		cout << "몬스터가 반격했습니다" << endl;

	const int ROCK = 0;
	const int PAPER = 1;
	const int SCISSORS = 2;

	int input = ROCK;

	if (input == ROCK)
		cout << "바위를 냈습니다" << endl;
	else if (input == PAPER)
		cout << "보를 냈습니다" << endl;
	else if (input == SCISSORS)
		cout << "가위를 냈습니다" << endl;
	else
		cout << "뭘 낸겁니까?" << endl;

	// switch - case - break
	// break를 누락시키면 결과값이 달라질 수 있으므로 break 조심
	// break를 안걸면 switch문을 이탈하지 않고 그 다음 case 내용까지 작동함

	// 정수 계열만 넣을 수 있다.
	// if/else문 보다 범용성이 떨어진다
	switch (input)
	{
	case ROCK:
		cout << "보를 냈습니다" << endl;
		break;
	case PAPER:
		cout << "보를 냈습니다" << endl;
		break;
	case SCISSORS:
		cout << "가위를 냈습니다" << endl;
		break;
	// else의 역할을 함
	default:
		cout << "뭘 낸겁니까?" << endl;
	}
	
}