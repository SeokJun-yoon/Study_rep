#include <iostream>
using namespace std;

// ������ ���� : �Լ�(���ν���, �޼ҵ�, ��ƾ)

/*
	input���� ������ �ް�, output���� ������ ������ �����ش�.

��ȯŸ�� �Լ��̸�([����Ÿ�� �Ű�����])
{
	�Լ� ����

	return ---;
}
*/

// Hello World�� �ֿܼ� ����ϴ� �Լ��� ������!
// input : ���� / output : ����
// Ÿ�� : int float double char~~ void

// �Լ� �̸��� �Ϲ������� ù ���� �빮�ڷ� ���̹�
void PrintHelloWorld(void) // input�� void�� ���� ����
{
	cout << "Hello World" << endl;

	return;

	// ���� ���� �ڵ�� ��ȿ���� �ʴ�.
}

// ������ �Է�(�Լ�) �޾Ƽ�, �ֿܼ� ����ϴ� �Լ��� ������.
// input : int // output : ����
void PrintNumber(int number)
{
	cout << "�Ѱ��ֽ� ���ڴ� " << number << " �Դϴ�" << endl;
}

// 2�� ���ϰ� �� ����� ��� �Լ��� ������.
int MultiplyBy2(int a)
{
	int b = a * 2;
	return b;
}

// �� ���ڸ� ���ؼ� ������� ��� �Լ��� ������
// input : int, int / output : int

int MultiplyBy(int a, int b)
{
	int c=a*b;
	return c;
}

int main()
{
	int a=3;
	int b=5;
	int result = MultiplyBy(b,a); // �Լ��� �Ű������� ����� ������ ���� ������ �ǹ��̴�. ������ ������ �������� ����� ����
	PrintNumber(result);
	//PrintHelloWorld();
}