#include <iostream>
using namespace std;
#include <vector>
#include <list>

// 오늘의 주제 : list

// vector : 동적 배열

// 단일 / 이중 / 원형
// list : 연결 리스트

// [1] -> [2] -> [3] -> [4] -> [5]
// [1] <-> [2] <-> [3] <-> [4] <-> [5]
// [1] <-> [2] <-> [3] <-> [4] <-> [5] <->

// [1]    ->    [2]   ->  [3] -> [4]->[5]

class Node
{
public:
	Node* _next;
	Node* _prev;
	int _data;
};


int main()
{
	// list (연결 리스트)
	// - list의 동작 원리
	// - 중간 삽입/삭제 (GOOD / GOOD)
	// - 처음/끝 삽입/삭제 (GOOD / GOOD)
	// - 임의 접근 (i번째 데이터는 어디 있습니까?)

	list<int> li;

	for (int i = 0; i < 100; i++)
		li.push_back(i);

	//li.push_front(10);
	int size = li.size();
	//li.capacity(); 없음 -> 동적이지 않으므로 용량이란 개념이 없음

	int first = li.front();
	int last = li.back();

	// li[3] =10 // 없음 -> vector와 다름

	list<int>::iterator itBegin=li.begin();
	list<int>::iterator itEnd=li.end();
	
	for (list<int>::iterator it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	li.insert(itBegin, 100);

	li.erase(li.begin());

	li.pop_front();

	li.remove(10); // !!!

	return 0;
}