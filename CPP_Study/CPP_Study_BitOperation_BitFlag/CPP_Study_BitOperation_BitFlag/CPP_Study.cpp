#include <iostream>
using namespace std;

bool stunned;
bool polymorph;

unsigned char flag; // 부호를 없애야 >>를 하더라도 부호비트가 딸려오지 않음

// 한 번 정해지면 절대 바뀌지 않을 값들
// constant의 약자인 const를 붙임 (변수를 상수화 한다고 함)
// const를 붙였으면 초기값을 반드시 지정해야 함

// 그러면 const도 바뀌지 않는 읽기 전용
// .rodata?
// 사실 C++ 표준에서 꼭 그렇게 하라는 말이 없음
// 그냥 컴파일러 (VS) 마음이라는 것
const int AIR = 0;
const int STUN = 1;
const int POLYMORPH = 2;
const int FEAR = 3;
const int INVINCIBLE = 4;

// 전역 변수

// [데이터 영역]
// .data (초기값이 있는 경우)
int a=2;

// .bss (초기값 없는 경우)
int b;

// .rodata (읽기 전용 데이터)
const char* msg = "Hello World";

int main()
{
	// [스택 영역]
	int c = 3;

#pragma region 비트 연산

	// 언제 필요한가? (사실 많이는 없음)
	// 비트 단위의 조작이 필요할 때
	// - 대표적으로 BitFlag

	// ~ bitwise not
	// 단일 숫자의 모든 비트를 대상으로, 0은 1, 1은 0으로 뒤바꿈

	// & bitwise and
	// 두 숫자의 모든 비트 쌍을 대상으로, and를 한다

	// | bitwise or
	// 두 숫자의 모든 비트 쌍을 대상으로, or를 한다

	// ^ bitwise xor
	// 두 숫자의 모든 비트 쌍을 대상으로, xor를 한다 (xor은 같으면 0, 다르면 1이 된다)
	// * xor의 특징, 같은 숫자로 xor을 하게 된다면 0이 되고 한 번 더 xor을 하게 되면 원래 숫자가 나오게 되어
	// 암호학과 같은 학문에서 유용하게 쓰인다.

	// << 비트 좌측 이동
	// 비트열을 N만큼 왼쪽으로 이동
	// 왼쪽의 넘치는 N개의 비트는 버림. 새로 생성되는 N개의 비트는 0으로 채운다
	// *2를 할 때 자주 보이는 패턴
	
	// >> 비트 우측 이동
	// 비트열을 N만큼 오른쪽으로 이동
	// 오른쪽의 넘치는 N개의 비트는 버림. 
	// 왼쪽 생성되는 N개의 비트는
	// - 부호 비트가 존재할 경우 부호 비트를 따라감 (부호있는 정수라면 이 부분을 유의)
	// 아니면 0


	// 실습
	// 0b0000 [무적][공포][변이][스턴][에어본]

	// 무적 상태로 만든다
	flag = (1 << INVINCIBLE);

	// 변이 상태를 추가한다 ( 무적 + 변이 )
	flag |= (1 << POLYMORPH);

	// 무적인지 확인하고 싶다? (다른 상태는 관심 없음)
	// bitmask
	bool invincible=(flag & (1 << INVINCIBLE)) != 0;

	// 무적이거나 스턴 상태인지 확인하고 싶다면?
	bool mask = (1 << INVINCIBLE) | (1 << STUN);
	bool stunOrInvincible = ((flag & 0b1010) != 0);

#pragma endregion
}