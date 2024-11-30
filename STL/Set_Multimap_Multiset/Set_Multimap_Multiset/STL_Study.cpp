#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>

// ���� : set, multimap, multiset

// Key�� Value ���� ���� �ڷᱸ���� set.

// multimap / multiset�� �ߺ� Ű���� ����ϴ� ��쿡 ���.
// ��� �󵵰� ������ �ʴ�.

#include <set>

int main()
{
	// (Key = Value)
	set<int> s;

	// �ְ�
	// ����
	// ã��
	// ��ȸ�ϰ�
	
	// �ְ�
	s.insert(10);
	s.insert(30);
	s.insert(20);
	s.insert(50);
	s.insert(40);
	s.insert(70);
	s.insert(90);
	s.insert(80);
	s.insert(100);

	// ����
	s.erase(40);
	
	// ã��
	set<int>::iterator findIt = s.find(50);
	if (findIt == s.end())
	{
		cout << "�� ã��" << endl;
	}
	
	else
	{
		cout << "ã��" << endl;
	}



	// ��ȸ�ϰ�
	for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << (*it) << endl;
	}

	cout << "-------------------------------------------------" << endl;

	multimap<int, int> mm;

	// �ְ�
	mm.insert(make_pair(1, 100));
	mm.insert(make_pair(1, 200));
	mm.insert(make_pair(1, 300));
	mm.insert(make_pair(2, 400));
	mm.insert(make_pair(2, 500));

	// mm[1] = 500; // �ȵ�

	// ����
	mm.erase(1);
	unsigned int count = mm.erase(1);

	// ã��
	//multimap<int, int>::iterator itFind = mm.find(1);
	//if (itFind != mm.end())
	//	mm.erase(itFind);

	pair<multimap<int, int>::iterator, multimap<int, int>::iterator> itPair;
	itPair = mm.equal_range(1);

	mm.lower_bound(1);
	mm.upper_bound(1);

	for (multimap<int, int>::iterator it = itPair.first; it != itPair.second; ++it)
	{
		cout << it->first << " " << it->second << endl;
	}

	cout << "-----------------------------------------------------------------" << endl;

	multiset<int> ms;

	// �ְ�
	ms.insert(100);
	ms.insert(100);
	ms.insert(100);
	ms.insert(200);
	ms.insert(200);

	// ã��
	multiset<int>::iterator findIt2 = ms.find(100);

	pair<multiset<int>::iterator, multiset<int>::iterator> itPair2;
	itPair2 = ms.equal_range(100);

	for (multiset<int>::iterator it = itPair2.first; it != itPair2.second; ++it)
	{
		cout << *it << endl;
	}



	return 0;
}