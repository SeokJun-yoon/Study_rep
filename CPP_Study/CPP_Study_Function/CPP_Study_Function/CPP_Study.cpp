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

void PrintHelloWorld()
{
	cout << "Hello World!" << endl;
	
	return;
}

// ������ �Է�(�Լ�) �޾Ƽ�, �ֿܼ� ����ϴ� �Լ��� ������
// input : int // output : ����
void PrintNumber(int number)
{
	cout << "�Ѱ��ֽ� ���ڴ� " << number << " �Դϴ�." << endl;
}

// 2�� ���ϰ� ������� ��� �Լ��� ������
// input : int / output : int

int MultiplyBy2(int a)
{
	int b = a * 2;
	return b;
}

// �� ���ڸ� ���ؼ� ������� ��� �Լ��� ������
// input : int, int / output : int

int MultiplyBy(int a, int b)
{
	int c = a * b;
	return c;
}

// ����� �� ��,
// F5 - �ߴ����� Ȱ���Ͽ� �ɸ����� Ȯ���ϱ� ���ؼ�
// F10 - ���ν���(�Լ�) ������ ���� -> �������� �κб��� ���� ����
// F11 - �� �ܰ辿 �ڵ� ���� -> �Լ� �ȱ��� ������

int main()
{
	int result = MultiplyBy(3, 5);
	PrintNumber(result);
	//PrintHelloWorld();

	return 0;
}