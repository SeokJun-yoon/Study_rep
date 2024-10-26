#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

// ������ ���� : Auto

class Knight
{
public:
	int _hp;
};

template<typename T>
void Print(T t)
{
	cout << t << endl;
}

int main()
{
	// Moder C++ (C++11)

	//int a = 3;
	//float b = 3.14f;
	//double c = 1.23;
	//Knight d = Knight();
	//const char* e = "abcdefg";

	auto a = 3;
	auto b = 3.14f;
	auto c = 1.23;
	auto d = Knight();
	auto e = "abcdefg";

	// auto�� ������ ��Ŀī��
	// ���� ���� (type deduction)
	// -> ���� �ǰ� �� ���纸����
	// �߷� ��Ģ�� �������� �������� �� �ִ�

	auto f = &d;
	const auto test1 = b;
	auto* test2 = e;
	
	// ����!
	// �⺻ auto�� const, & ����
	int& reference = a;
	const int cst = a;

	auto test1 = reference; // test1�� �ڷ����� int�� �ν�
	auto test2 = cst;		// test2�� �ڷ����� int�� �ν�(const �ƴ�)

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
	{
		int& data = v[i];
		// auto data = v[i] --> auto�� �������� �����ϰ� ��
		// auto�� ����ϰ� �ʹٸ� auto& data�� ���� &�� �ٿ��ְų�
		// auto data = static_cast<int&>v[i] �� ���� ĳ������ ���־�� �Ѵ�.
		// �ٵ� ĳ������ �ҰŸ� auto�� ����ϴ� ������ �����Ƿ� �Ϲ������δ� ĳ�������� ������� ����
		data = 100;
	}

	// �ƹ�ư ���� ������ Ÿ���� �ؾ������ auto�� ����Ѵ�?
	// NO! (�ְ��� ����)
	// -> Ÿ������ ������� ��� OK
	// -> �������� ���� �Ϲ������δ� ���δ°� ����.

	map<int, int> m;
	auto ok = m.insert(make_pair(1, 100));
	// ���� ���� � �������� ũ�� �Ű� �� �ʿ� ���� ��,
	
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		cout << *it << endl;
	}


	return 0;
}