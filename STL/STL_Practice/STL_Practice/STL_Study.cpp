#include <iostream>
#include <ctime>
using namespace std;
#include <vector>

// 주제 : 연습문제

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// Q1) number라는 숫자가 벡터에 있는지 체크하는 기능 (return 값 형태 : bool or 처음으로 등장하는 iterator)
	{
		int number = 50;

		bool found = false;

		// TODO

		// 알고리즘 : for문을 벡터의 처음부터 끝까지 돌리면서 내용물이 number 변수와 같은지 비교하고,
		// number와 같은 수가 있다면 bool값을 true로 변경하기
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i] == number)
			{
				found = true;
				cout << v[i] << endl;
				cout << found << endl;
				break;
			}
			else
			{
				cout << v[i] << " : NO" << endl;
			}
		}
	}

	// Q2) 11로 나뉘는 숫자가 벡터에 안에 있는지 체크하는 기능 (bool or 처음으로 등장하는 iterator)
	{
		bool found = false;

		 // TODO

		 // 알고리즘 : for문을 벡터의 처음부터 끝까지 돌리면서 v[i] % 11 == 0 일 경우
		 // bool값 true로 변경

		for (int i = 0; i < v.size(); i++)
		{
			cout << v[i] << endl;

			if (v[i] % 11 == 0)
			{
				found = true;
				cout << "found" << endl;
				break;
			}
		}


	}

	//// Q3) 홀수인 숫자의 개수는? (count)
	{
		int count = 0;

		// TODO
		// 알고리즘 : for문을 벡터의 처음부터 끝까지 돌면서 v[i] % 2 != 0일 경우, count++
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i] %2 != 0)
			{
				count++;
			}
		}
		cout << count << endl;
	}

	// Q4) 벡터에 들어가 있는 모든 숫자들에 3을 곱해줄 것
	{
		// TODO
		// 알고리즘 : for문을 벡터의 처음부터 끝까지 돌면서 v[i] *= 3 해줄 것
		for (int i = 0; i < v.size(); i++)
		{
			cout << "[" << i << "]" << "x3 전 : " << v[i];
			v[i] *= 3;
			cout << " -> " << v[i] << endl;
		}
	}

	return 0;
}