#include <iostream>
#include <vector>
using namespace std;

// 오늘의 주제 : vector
// 면접 단골. 

int main()
{
	// STL (Standard Template Library)
	// 프로그래밍할 때 필요한 자료구조/알고리즘들을
	// 템플릿으로 제공하는 라이브러리

	// 컨테이너(Container) : 데이터를 저장하는 객체 (자료구조 Data Structure)

	// vector (동적 배열)
	// - vector의 동작 원리 (size/capacity)
	// - 중간 삽입/삭제
	// - 처음/끝 삽입/삭제
	// - 임의 접근

	// 배열 --> 크기를 유동적으로 조절할 수 없다는 큰 단점이 있음

	// 동적 배열
	// 매우 매우 중요한 개념 -> 어떤 마법을 부렸길래 배열을 '유동적으로' 사용한 것인가?

	// 1) (여유분을 두고) 메모리를 할당한다
	// 2) 여유분까지 꽉 찼으면, 메모리를 증설한다

	// 01) 여유분은 얼만큼이 적당할까?
	// 02) 증설을 얼만큼 해야 할까?
	// 03) 기존의 데이터를 어떻게 처리할까?

	vector<int> v;
	vector<int> v2 = v; // 그대로 복사도 가능
	// cf. vector<int> v(1000); capacity를 생성자에서 정해주고 시작
	// vector<int> v(1000, 0); 1000개의 capacity에 데이터 0으로 초기화

	// size (실제 사용 데이터 개수)
	// 1 2 3 4 5 6 7 ...

	// v.resize(1000); // 1000개의 사이즈를 잡고 시작

	//v.reserve(1000); // reserve는 자동으로 증설하는것이 아닌 직접 할당 공간을 정해주는 것 이 예시에서는 1000
	// 예시 질문) vector를 사용 할 때 초반에 reserve로 capacity를 조절해주는 이유?
	// capacity가 변화할 때마다 기존의 데이터들을 복사하는 과정이 일어나므로 처음부터 어느정도 근사치의 capacity를 잡아준다면
	// 복사에 대한 부분을 생략할 수 있으므로 더 효율적이기 때문이다.

	// capacity (여유분을 포함한 용량 개수) - 약 1.5배씩 증가
	// 여유분을 하나씩이 아닌 더 여유 있게 증설하는 이유?
	// 복사에 대한 비용을 고려했을 때, 하나씩 증설할 경우 너무 잦은 복사가 일어나기 때문
	// 1 2 3 4 6 9 13 19 28 42 63 ...
	// C++의 경우에서는 size가 감소한다고 capacity가 줄어들지는 않는다.

	for (int i = 0; i < 1000; i++)
	{
		v.push_back(100); // 맨 뒤에 데이터를 추가
		// v.pop_back(); 맨 뒤의 데이터를 삭제
		cout << v.size() << " " << v.capacity() << endl;
	}

	v.front(); // 맨 앞의 데이터
	v.back(); // 맨 뒤의 데이터
	v.clear(); // vector을 비워버림
	vector<int>().swap(v);	// capacity까지 비워버리는 방법 (잘 쓰이지는 않음)
	cout << v.size() << " " << v.capacity() << endl;


	return 0;
}