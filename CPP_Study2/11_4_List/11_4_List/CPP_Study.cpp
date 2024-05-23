#include <iostream>
using namespace std;
#include <vector>
#include <list>

// ������ ���� : list

// vector : ���� �迭

// ���� / ���� / ����
// list : ���� ����Ʈ

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
	// list (���� ����Ʈ)
	// - list�� ���� ����
	// - �߰� ����/���� (GOOD / GOOD)
	// - ó��/�� ����/���� (GOOD / GOOD)
	// - ���� ���� (i��° �����ʹ� ��� �ֽ��ϱ�?)

	list<int> li;

	for (int i = 0; i < 100; i++)
		li.push_back(i);

	//li.push_front(10);
	int size = li.size();
	//li.capacity(); ���� -> �������� �����Ƿ� �뷮�̶� ������ ����

	int first = li.front();
	int last = li.back();

	// li[3] =10 // ���� -> vector�� �ٸ�

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