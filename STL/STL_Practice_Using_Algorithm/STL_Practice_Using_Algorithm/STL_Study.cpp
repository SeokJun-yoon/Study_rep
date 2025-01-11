#include <iostream>
#include <ctime>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>

// 주제 : 알고리즘(Algorithm)
#include <algorithm>

int main()
{
	// 자료구조 (데이터를 저장하는 구조)
	// 알고리즘 (데이터를 어떻게 사용할 것인가)

	// find
	// find_if
	// count
	// count_if
	// all_of
	// any_of
	// none_of
	// for_each
	// remove
	// remove_if

	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < v.size(); i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// Q1) number라는 숫자가 벡터에 있는지 체크하는 기능. (bool 값 반환 or 처음으로 등장하는 iterator 반환)
	{
		int number = 50;


	}





	return 0;
}