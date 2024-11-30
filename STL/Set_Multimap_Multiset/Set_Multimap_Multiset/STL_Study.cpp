#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>

// 주제 : set, multimap, multiset

// Key와 Value 값이 같은 자료구조가 set.

// multimap / multiset은 중복 키값을 허용하는 경우에 사용.
// 사용 빈도가 높지는 않다.

#include <set>

int main()
{
	// (Key = Value)
	set<int> s;

	// 넣고
	// 빼고
	// 찾고
	// 순회하고
	
	// 넣고
	s.insert(10);
	s.insert(30);
	s.insert(20);
	s.insert(50);
	s.insert(40);
	s.insert(70);
	s.insert(90);
	s.insert(80);
	s.insert(100);

	// 빼고
	s.erase(40);
	
	// 찾고
	set<int>::iterator findIt = s.find(50);
	if (findIt == s.end())
	{
		cout << "못 찾음" << endl;
	}
	
	else
	{
		cout << "찾음" << endl;
	}



	// 순회하고
	for (set<int>::iterator it = s.begin(); it != s.end(); ++it)
	{
		cout << (*it) << endl;
	}

	cout << "-------------------------------------------------" << endl;

	multimap<int, int> mm;

	// 넣고
	mm.insert(make_pair(1, 100));
	mm.insert(make_pair(1, 200));
	mm.insert(make_pair(1, 300));
	mm.insert(make_pair(2, 400));
	mm.insert(make_pair(2, 500));

	// mm[1] = 500; // 안됨

	// 빼고
	mm.erase(1);
	unsigned int count = mm.erase(1);

	// 찾고
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

	// 넣고
	ms.insert(100);
	ms.insert(100);
	ms.insert(100);
	ms.insert(200);
	ms.insert(200);

	// 찾고
	multiset<int>::iterator findIt2 = ms.find(100);

	pair<multiset<int>::iterator, multiset<int>::iterator> itPair2;
	itPair2 = ms.equal_range(100);

	for (multiset<int>::iterator it = itPair2.first; it != itPair2.second; ++it)
	{
		cout << *it << endl;
	}



	return 0;
}