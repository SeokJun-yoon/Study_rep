#include <iostream>
using namespace std;
#include <vector>

// 오늘의 주제 : vector
// 면접 단골
int main()
{
	// STL (Standard TEmplate Library)
	// 프로그래밍할 때 필요한 자료구조/알고리즘들을
	// 템플릿으로 제공하는 라이브러리

	// 컨테이너(Container) : 데이터를 저장하는 객체 (자료구조 Data Structure)

	// vector (동적 배열)
	// - vector의 동작 원리 (size/capacity)
	// - 중간 삽입삭제
	// - 처음/끝 삽입/삭제
	// - 임의 접근

	// 반복자(Iterator) : 포인터와 유사한 개념. 컨테이너의 원소(데이터)를 가리키고, 다음/이전 원소로 이동 가능

	vector<int> v(10);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
	{
		v[i] = i;
	}

	//vector<int>::iterator it;
	//int* ptr;		

	//it = v.begin();
	//ptr = &v[0];

	//cout << (*it) << endl;
	//cout << (*ptr) << endl;

	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();

	// 더 복잡해 보이는데?
	// 다른 컨테이너는 v[i]와 같은 인덱스 접근이 안 될 수도 있음
	// iterator는 vector뿐 아니라, 다른 컨테이너에도 공통적으로 있는 개념 (따라서 vector가 아닌 다른 컨테이너로 바꿔도 동작이 가능하다)
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << (*it) << endl;
	}

	int* ptrBegin = &v[0]; // v.begin()._Ptr;
	int* ptrEnd = ptrBegin + 10; // v.end()._ptr;
	for (int* ptr = ptrBegin; ptr != ptrEnd; ++ptr)
	{
		cout << (*ptr) << endl;
	}

	// const int*;
	//vector<int>::const_iterator cit1 = v.cbegin();
	// *cit1 = 100; // const이므로 수정 불가
	
	// 자주 활용할 일은 없음. 알아만 둘 것
	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++i)
	{
		cout << (*it) << endl;
	}

	return 0;
}