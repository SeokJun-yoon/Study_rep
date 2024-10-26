#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// 오늘의 주제 : 중괄호 초기화 { }

class Knight
{
public:

};

int main()
{
	
	// 중괄호 초기화 { }
	int a = 0;
	int b(0);
	int c{ 0 };

	Knight k1;
	Knight k2 = k1;	// 복사 생성자 (대입 연산자X)

	Knight k3{ k1 };

	return 0;
}