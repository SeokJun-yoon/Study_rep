#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// ������ ���� : �߰�ȣ �ʱ�ȭ { }

class Knight
{
public:

};

int main()
{
	
	// �߰�ȣ �ʱ�ȭ { }
	int a = 0;
	int b(0);
	int c{ 0 };

	Knight k1;
	Knight k2 = k1;	// ���� ������ (���� ������X)

	Knight k3{ k1 };

	return 0;
}