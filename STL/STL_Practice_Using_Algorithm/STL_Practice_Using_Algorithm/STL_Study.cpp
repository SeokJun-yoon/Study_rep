#include <iostream>
#include <ctime>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>

// ���� : �˰���(Algorithm)
#include <algorithm>

int main()
{
	// �ڷᱸ�� (�����͸� �����ϴ� ����)
	// �˰��� (�����͸� ��� ����� ���ΰ�)

	// find
	// find_if
	// count
	// count_if
	// all_of
	// any_of
	// none_of
	// for_each
	// remove
	// remove_if

	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < v.size(); i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// Q1) number��� ���ڰ� ���Ϳ� �ִ��� üũ�ϴ� ���. (bool �� ��ȯ or ó������ �����ϴ� iterator ��ȯ)
	{
		int number = 50;


	}





	return 0;
}