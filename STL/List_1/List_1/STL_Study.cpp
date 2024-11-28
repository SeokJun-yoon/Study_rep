#include <iostream>
using namespace std;
#include <list>

// 주제 : list

// vector와 list는 굉장히 유사하다고 볼 수 있으나, vector는 동적 배열 방식, list는 node 방식

// vector에 비해 자주 쓰이지는 않을 수 있으나, 차후에 배울 tree 구조의 자료 구조들을 이해하는데 도움이 된다.

// vector와 list의 차이에 대한 부분도 면접 단골 질문.


class Node
{
public:
	Node*	_next;
	Node*	_prev;
	int		_data;
};
// 예시로 스타의 캐리어와 인터셉터 같이 Node들이 인터셉터, list가 캐리어와 같은 느낌
// Node 구조로 인해서 ** 처음/끝 & 중간 삽입/삭제가 편하다 **
// 중간 삽입/삭제시에 node의 주소값을 가지고 동작하기 때문에

// 단일 / 이중 / 원형
// list : 연결 리스트

// [1] -> [2] -> [3] -> [4] -> [5]
// [1] <-> [2] <-> [3] <-> [4] <-> [5] <-> [6] <-> [ _Myhead : end() ] <->
// [1] <-> [2] <-> [3] <-> [4] <-> [5] <->



int main()
{
	// list (연결 리스트)
	// - list의 동작 원리
	// - 중간 삽입/삭제	(GOOD / GOOD)
	// - 처음/끝 삽입 삭제	(GOOD / GOOD)
	// - 임의 접근 (i번째 데이터의 위치를 찾으려면?)

	list<int> li;

	for (int i = 0; i < 100; i++)
		li.push_back(i);

	//li.push_front(10);
	// list에서 push_front가 가능한 이유? 처음 삽입이 어렵지 않기 때문.
	// 
	// vector에서는 push_front X
	int size = li.size();
	// li.capacity(); // 없음. 동적 배열의 개념이 아님.

	int first = li.front();
	int last = li.back();

	//li[3] = 10; // 없음
	// vector와 다르게 index를 통한 임의 접근이 불가함.
	// 매우 느리기 때문에 지원하지 않는 방식.

	list<int>::iterator itBegin = li.begin();
	list<int>::iterator itEnd = li.end();

	//list<int>::iterator itTest1 = --itBegin;
	//list<int>::iterator itTest2 = --itEnd;
	//list<int>::iterator itTest3 = ++itEnd;
	

	int* ptrBegin = &(li.front());
	int* ptrEnd = &(li.back());

	for (list<int>::iterator it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	//li.insert(itBegin, 100);

	//li.erase(li.begin());

	//li.pop_front();

	// 중간 삭제가 가능한 함수
	//li.remove(10); // 10에 해당하는 값을 알아서 찾아 지워주는 함수. vector는 중간 삽입 삭제가 어렵고, 그래서 vector에는 존재하지 않는 함수이다.

	// * 임의 접근이 안 된다.
	// * 중간 삽입/삭제 빠르다(?)
	// 50번 인덱스에 있는 데이터를 삭제한다면?
	list<int>::iterator it = li.begin();
	for (int i = 0; i < 50; i++)
		++it;

	li.erase(it);



	return 0;
}