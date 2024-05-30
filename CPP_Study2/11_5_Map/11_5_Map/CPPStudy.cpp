#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
// ������ ���� : map
#include <map>

class Player
{
public:
	Player() : _playerId(0) { }
	Player(int playerId) : _playerId(playerId) { }
public:
	int _playerId;
};


int main()
{
#pragma region vector�� ���� Ȯ��
	//// ���� �����̳�

	//vector<Player*> v;
	//
	//for (int i = 0; i < 1000000; i++)
	//{
	//	Player* p = new Player(i);
	//	v.push_back(p);
	//}

	//// 5������ ����
	//for (int i = 0; i < 50000; i++)
	//{
	//	int randIndex = rand() % v.size();

	//	Player* p = v[randIndex];
	//	delete p;

	//	v.erase(v.begin() + randIndex);
	//}

	//// (ID = 2���÷��̾�)�� (ID = 1 �� Player)�� �����ϰ� �;��
	//// Q) ID = 1���� Player�� ã���ּ���

	//// vector�δ� �Ʒ��� ���� �� ȿ�������� ã�´�. �� �����͸� ������ ã�Ⱑ ��ƴ�.
	//bool found = false;

	//for (int i = 0; i < v.size(); i++)
	//{
	//	if (v[i]->_playerId == 10000)
	//	{
	//		found = true;
	//		break;
	//	}
	//}

	//// vector, list�� ġ������ ����
	//// -> ���ϴ� ���ǿ� �ش��ϴ� �����͸� ������ ã�� �� ����.
#pragma endregion vector�� ���� Ȯ��


#pragma region map
	// map : ���� ����Ʈ�� (AVL)
	// + ��� ���

	class Node
	{
	public:
		Node* _left;
		Node* _right;
		// DATA
		pair<int, Player> _data;
		//int _key;
		//Player* _value;
	};
	
	srand(static_cast<unsigned int>(time(nullptr)));
	map<int, int> m;


	// ���� key ���� �ٸ� Value�� �ο�? -> ( X ) 
	pair<map<int, int>::iterator, bool> ok;
	ok = m.insert(make_pair(1, 100));
	ok = m.insert(make_pair(1, 200));	// ������ Ű ���� Value�� insert�ϸ� ���� ��


	// 10����
	for (int i = 0; i < 100000; i++)
	{
		m.insert(pair<int, int>(i, i * 100));
	}

	// 5���� ����
	for (int i = 0; i < 50000; i++)
	{
		int randomValue = rand() % 50000;

		// Erase By Key
		m.erase(randomValue);
	}

	// Q) ID = 1���� Player ã�� �ʹ�!
	// A) �ſ� ������ ã�� �� ����

	map<int, int>::iterator findIt = m.find(10000);
	if (findIt != m.end())
	{
		cout << "ã��" << endl;
	}
	else
	{
		cout << "�� ã��" << endl;
	}

	// map ��ȸ
	for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		pair<const int, int>& p = (*it);
		int key = it->first;
		int value = it->second;

		cout << key << " " << value << endl;
	}

	// ������ �߰�, ������ ����
	map<int, int>::iterator findIt = m.find(10000);
	if (findIt != m.end())
	{
		findIt->second = 200;
	}
	else
	{
		m.insert(make_pair(10000, 10000));
	}

	// ������ �߰�, ������ ���� version 2
	m[5] = 500;	// 5�� �ش��ϴ� Ű�� ������ �߰��ϰ�, value�� 500 ����, ������ 5�� Ű�� value���� 500���� ����
	
	m.clear();

	// [] ������ ����� �� ����
	// ������ ���� �ʴ��� (Key/Value) ������ �����Ͱ� �߰��ȴ�!
	for (int i = 0; i < 10; i++)
	{
		cout << m[i] << endl;
	}

#pragma endregion map
	return 0;
}