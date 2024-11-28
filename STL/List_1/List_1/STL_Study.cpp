#include <iostream>
using namespace std;
#include <list>

// ���� : list

// vector�� list�� ������ �����ϴٰ� �� �� ������, vector�� ���� �迭 ���, list�� node ���

// vector�� ���� ���� �������� ���� �� ������, ���Ŀ� ��� tree ������ �ڷ� �������� �����ϴµ� ������ �ȴ�.

// vector�� list�� ���̿� ���� �κе� ���� �ܰ� ����.


class Node
{
public:
	Node*	_next;
	Node*	_prev;
	int		_data;
};
// ���÷� ��Ÿ�� ĳ����� ���ͼ��� ���� Node���� ���ͼ���, list�� ĳ����� ���� ����
// Node ������ ���ؼ� ** ó��/�� & �߰� ����/������ ���ϴ� **
// �߰� ����/�����ÿ� node�� �ּҰ��� ������ �����ϱ� ������

// ���� / ���� / ����
// list : ���� ����Ʈ

// [1] -> [2] -> [3] -> [4] -> [5]
// [1] <-> [2] <-> [3] <-> [4] <-> [5] <-> [6] <-> [ _Myhead : end() ] <->
// [1] <-> [2] <-> [3] <-> [4] <-> [5] <->



int main()
{
	// list (���� ����Ʈ)
	// - list�� ���� ����
	// - �߰� ����/����	(GOOD / GOOD)
	// - ó��/�� ���� ����	(GOOD / GOOD)
	// - ���� ���� (i��° �������� ��ġ�� ã������?)

	list<int> li;

	for (int i = 0; i < 100; i++)
		li.push_back(i);

	//li.push_front(10);
	// list���� push_front�� ������ ����? ó�� ������ ����� �ʱ� ����.
	// 
	// vector������ push_front X
	int size = li.size();
	// li.capacity(); // ����. ���� �迭�� ������ �ƴ�.

	int first = li.front();
	int last = li.back();

	//li[3] = 10; // ����
	// vector�� �ٸ��� index�� ���� ���� ������ �Ұ���.
	// �ſ� ������ ������ �������� �ʴ� ���.

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

	// �߰� ������ ������ �Լ�
	//li.remove(10); // 10�� �ش��ϴ� ���� �˾Ƽ� ã�� �����ִ� �Լ�. vector�� �߰� ���� ������ ��ư�, �׷��� vector���� �������� �ʴ� �Լ��̴�.

	// * ���� ������ �� �ȴ�.
	// * �߰� ����/���� ������(?)
	// 50�� �ε����� �ִ� �����͸� �����Ѵٸ�?
	list<int>::iterator it = li.begin();
	for (int i = 0; i < 50; i++)
		++it;

	li.erase(it);



	return 0;
}