#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>

// 주제 : map

// vector, list의 치명적인 단점
// -> 원하는 조건에 해당하는 데이터를 빠르게 찾을 수 [없다]


class Player
{
public:
	Player() : _playerId(0) {}
	Player(int playerId) : _playerId(playerId) { }
public:
	int _playerId;

};

int main()
{
	// 연관 컨테이너

	// map : 균형 이진 트리 (AVL)
	// - 노드 기반

	class Node
	{
	public:
		Node* _left;
		Node* _right;

		// DATA
		pair<int, Player*> _data;
		//int		_key;
		//Player* _value;
	};
	
	srand(static_cast<unsigned int>(time(nullptr)));
	// (Key, Value)
	map<int, int> m;

	// 10만명
	for (int i = 0; i < 100000; i++)
	{
		m.insert(pair<int, int>(i, i * 100));
	}

	// 5만명 퇴장
	for (int i = 0; i < 50000; i++)
	{
		int randomValue = rand() % 50000;

		// Erase By Key
		m.erase(randomValue);
	}

	// Q) ID = 1만인 Player 찾고 싶다.
	// A) 매우 빠르게 찾을 수 있음
	//
	//map<int, int>::iterator findIt = m.find(10000);
	//if (findIt != m.end())
	//{
	//	cout << "찾음" << endl;
	//}
	//else
	//{
	//	cout << "못 찾음" << endl;
	//}

	// map 순회
	for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		pair<const int, int>& p = (*it);
		int key = it->first;
		int value = it->second;

		cout << key << " " << value << endl;
	}

	// 없으면 추가, 있으면 수정
	map<int, int>::iterator findIt = m.find(10000);
	if (findIt != m.end())
	{
		findIt->second = 200;
	}
	else
	{
		m.insert(make_pair(10000, 200));
	}

	// 없으면 추가, 있으면 수정 version 2
	m[5] = 500;	// 위의 코드를 단순하게 사용 가능함

	m.clear();
	// 다만, [] 연산자 사용할 때 주의
	// 대입을 하지 않더라도 (Key/Value) 형태의 데이터가 추가된다!
	for (int i = 0; i < 10; i++)
	{
		cout << m[i] << endl;
		// 이렇게 사용하는 순간, 데이터가 없었더라도 세팅이 된다는 점을 조심해야 함
	}

	// 넣고		(insert, [])
	// 빼고		(erase)
	// 찾고		(find, [])
	// 반복자	(map::iterator) (*it) pair<key, value>

	return 0;
}