#include <iostream>
using namespace std;

// ������ ���� : ȣ�� ����

// �Լ� ����
// � ��������� ������, �켱������ �������־� ���縦 �˷��ش�.
void Func1();
void Func2(int hp, int mp); // �Ű������� �������� �� �Ʒ� �����ο� ���� �ʾƵ� �ȴ�
void Func3(float a);

// ����� �� ��, ȣ�⽺���� �� �� �˾ƾ��Ѵ�.
// F5, F10(���ν��� ����), F11(�� �پ�) ���

void Func1()
{
	cout << "Func1" << endl;
	Func2(1,2);
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