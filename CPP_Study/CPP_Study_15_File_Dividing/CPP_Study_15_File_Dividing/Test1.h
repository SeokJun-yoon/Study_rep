//#pragma once
// 중복되더라도 한 번만 읽도록 한다.
// pragma once가 최적화 하기에 좋다.

// 아래 #ifndef - #define - #endif 문법도 읽을 줄은 알아야한다.
// 기능은 중복 방지로 같다.

// 헤더 파일에 쓸데없는 너무 많은 것을 넣지 말 것.

#ifndef _TEST1_H__
#define _TEST1_H__

struct StatInfo
{
	int hp;
	int attack;
	int defence;
};

void Test_1();

void Test_2();

void Test_3();

#endif