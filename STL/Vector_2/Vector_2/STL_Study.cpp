#include <iostream>
using namespace std;
#include <vector>

int main()
{
	// 컨테이너(Container) : 데이터를 저장하는 객체 (자료구조 Data Structure)

	// vector (동적 배열)
	// - vector의 동작 원리 (size/capacity)
	// - 중간 삽입/삭제
	// - 처음/끝 삽입/삭제
	// - 임의 접근

	// 반복자 (Iterator) : 포인터와 유사한 개념. 컨테이너의 원소(데이터)를 가리키고, 다음/이전 원소로 이동 가능
	vector<int> v(10);

	v.reserve(1000);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
		v[i] = i;

	//vector<int>::iterator it;
	//// iterator 내부를 확인해보면 ptr를 지원하는 것을 알 수 있음
	//int* ptr;

	//it = v.begin();
	//ptr = &v[0];

	//cout << (*it) << endl;
	//cout << (*ptr) << endl;

	//int a = 1;
	////int b = a++; // (b=1, a=2)
	//int c = ++a; // (c=2, a=2)

	//it++;
	//++it;
	//ptr++;
	//++ptr;
	//
	//it--;
	//--it;
	//ptr--;
	//--ptr;

	//it += 2;
	//it = it - 2;
	//ptr += 2;
	//ptr = ptr - 2;
	//// 전부 가능 함. 

	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();

	// 더 복잡해보이는데?
	// 다른 컨테이너는 v[i]와 같은 인덱스 접근이 안 될수도 있음
	// iterator는 vector뿐 아니라, 다른 컨테이너에도 공통적으로 있는 개념
	// 하지만 vector에서는 사실 위의 단순한 방식이 더 편하다
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) // 참고 it++ 보다 ++it가 조금 더 효율적임
	{
		cout << (*it) << endl;
	}

	int* ptrBegin = &v[0]; // v.begin()._Ptr;
	int* ptrEnd = ptrBegin + 10; // v.end()._Ptr;
	for (int* ptr = ptrBegin; ptr != ptrEnd; ++ptr)
	{
		cout << (*ptr) << endl;
	}
	
	// const int*
	//vector<int>::const_iterator cit1 = v.cbegin();
	//*cit1 = 100;

	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)
	{
		cout << (*it) << endl;
	}

	// - 중간 삽입/삭제 (BAD)
	// - 처음/끝 삽입/삭제 (BAD / GOOD)
	// - 임의 접근 (Random Access)
	// * 면접에서 기본기에 대한 단골 질문

	// vector = 동적 배열 = 동적으로 커지는 배열 = 배열
	// 원소가 하나의 메모리 블록에 연속하게 저장된다 !!!

	// [                 ]
	// [0][1][2][3][4][][]

	//v.push_back(1);
	//v.pop_back();
	// v.push_back과 v.pop_back은 있지만
	// v.push_front나 v.pop_front는 없다. 이 이유가 처음 삽입/삭제는 연속적이라는 벡터의 특성상
	// 중간 삽입/삭제와 별 다를바가 없기 때문이다.

	//vector<int>::iterator insertIt = v.insert(v.begin() + 2, 5);
	//// push_back 이나 pop_back보다 어렵게 되어 있는 이유 또한 벡터의 연속성 때문
	//vector<int>::iterator eraseIt1 = v.erase(v.begin() + 2);
	//vector<int>::iterator eraseIt2 = v.erase(v.begin() + 2, v.begin()+4);

	// 쭉~ 스캔을 하면서, 3이라는 데이터가 있으면 일괄 삭제하고 싶다

	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		int data = *it;
		if (data == 3)
		{
			// v.erase(it); 이대로 쓰면 안됨.
			it = v.erase(it);
		}
		else
		{
			++it;
		}
	}

	// 위의 예제에서 보았듯이 중간값을 사용할 때에는 연속적인 벡터의 특징 때문에 굉장히 유의해야 한다.

	return 0;
}