#include <iostream>
using namespace std;

// ������ ���� : ȣ�� ����

// �Լ� ����
void Func1();
void Func2(int a, int b);
void Func3(float a);

void Func1()
{
	cout << "Func1" << endl;

	// push 2
	// push 1
	// call Func2
	Func2(1, 2);
}

void Func2(int a, int b)
{
	cout << "Func2" << endl;

	Func3(10);
}

void Func3(float a)
{
	cout << "Func3" << endl;
}

int main()
{
	cout << "main" << endl;
	Func1();

	return 0;
}