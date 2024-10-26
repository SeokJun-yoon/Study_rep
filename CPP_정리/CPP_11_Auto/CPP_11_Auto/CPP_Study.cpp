#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

// 오늘의 주제 : Auto

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

	// auto는 일종의 조커카드
	// 형식 연역 (type deduction)
	// -> 말이 되게 잘 맞춰보도록
	// 추론 규칙은 생각보다 복잡해질 수 있다

	auto f = &d;
	const auto test1 = b;
	auto* test2 = e;
	
	// 주의!
	// 기본 auto는 const, & 무시
	int& reference = a;
	const int cst = a;

	auto test1 = reference; // test1의 자료형은 int로 인식
	auto test2 = cst;		// test2의 자료형도 int로 인식(const 아님)

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
	{
		int& data = v[i];
		// auto data = v[i] --> auto가 참조값을 무시하게 됨
		// auto를 사용하고 싶다면 auto& data와 같이 &를 붙여주거나
		// auto data = static_cast<int&>v[i] 와 같이 캐스팅을 해주어야 한다.
		// 근데 캐스팅을 할거면 auto의 사용하는 장점이 없으므로 일반적으로는 캐스팅으로 사용하지 않음
		data = 100;
	}

	// 아무튼 이제 기존의 타입은 잊어버리고 auto만 사용한다?
	// NO! (주관적 생각)
	// -> 타이핑이 길어지는 경우 OK
	// -> 가독성을 위해 일반적으로는 놔두는게 좋다.

	map<int, int> m;
	auto ok = m.insert(make_pair(1, 100));
	// 위와 같이 어떤 형식인지 크게 신경 쓸 필요 없을 때,
	
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		cout << *it << endl;
	}


	return 0;
}