#include <iostream>
using namespace std;

// ������ ���� : Ÿ�� ��ȯ

// malloc -> void*�� ��ȯ��, �̸� �츮�� (Ÿ�� ��ȯ)�� ���� ����߾���

class Knight
{
public:
	int _hp = 10;
};

class Dog
{
public:
	// Ÿ�� ��ȯ ������
	Dog(const Knight& knight)
	{
		_age = knight._hp;
	}

	// Ÿ�� ��ȯ ������
	operator Knight()
	{
		return (Knight)(*this);
	}
public:
	int _age = 1;
	int _cuteness = 2;
};

class BullDog : public Dog
{
public:
	bool _french; // ����ġ �ҵ�
};

int main()
{
	// --------------- Ÿ�� ��ȯ ���� (��Ʈ�� �籸�� ����) ------------------

	// [1] �� Ÿ�� ��ȯ
	// Ư¡) �ǹ̸� �����ϱ� ���ؼ�, ���� ��ü�� �ٸ� ��Ʈ�� �籸��
	{
		int a = 123456789;	// 2�� ����
		float b = (float)a;	// �ε��Ҽ���(���� + ��ȿ����)
		cout << b << endl;
	}

	// [2] ���� Ÿ�� ��ȯ
	// Ư¡) ��Ʈ���� �籸������ �ʰ�, '����'�� �ٲٴ� ��
	// ���� �� ���� ������, ������ Ÿ�� ��ȯ�� '���� Ÿ�� ��ȯ'�� ������ ���� �����ϱ� �ϼ������� ��������.

	{
		int a = 123456789;	// 2�� ����
		float b = (float&)a;	// �ε��Ҽ���(���� + ��ȿ����)
		cout << b << endl;
	}

	// --------------- ������ �з� ------------------
	// Ư¡) �ǹ̰� �׻� 100% ������ ��ġ�ϴ� ���
	// ���� Ÿ���̸鼭 ũ�⸸ �� ū �ٱ��Ϸ� �̵�
	// ���� �ٱ��� -> ū �ٱ��Ϸ� �̵� OK (��ĳ����)
	// ex) char -> short, short -> int, int -> __int64
	{
		int a = 123456789;
		__int64 b = a;
		cout << b << endl;
	}

	// [2] �Ҿ����� ��ȯ
	// Ư¡) �ǹ̰� �׻� 100% ��ġ�Ѵٰ� �������� ���ϴ� ���
	// Ÿ���� �ٸ��ų�
	// ���� Ÿ�������� ū �ٱ��� -> ���� �ٱ��� �̵� (�ٿ�ĳ����)
	{
		int a = 123456789;
		float b = a;
		short c = a;
		cout << b << endl;
		cout << c << endl;
	}

	// --------------- ���α׷��� �ǵ��� ���� �з� ------------------

	// [1] �Ͻ��� ��ȯ
	// Ư¡) �̹� �˷��� Ÿ�� ��ȯ ��Ģ�� ���� �����Ϸ��� '�ڵ�'���� Ÿ�� ��ȯ
	{
		int a = 123456789;
		float b = a; // �Ͻ�������
	}

	// [2] ������ ��ȯ
	{
		int a = 123456789;
		int* b = (int*)a; // �Ͻ�������
		cout << b << endl;
	}

	// --------------- �ƹ��� ���� ���谡 ���� Ŭ���� ������ ��ȯ ------------------
	
	// [1] �������� Ŭ���� ������ '�� Ÿ��' ��ȯ
	// Ư¡) �Ϲ������� �� ��(���� : Ÿ�� ��ȯ ������, Ÿ�� ��ȯ ������)6
	{
		Knight knight;
		Dog dog = (Dog)knight;

		Knight knight2 = dog;
	}

	// [2] �������� Ŭ���� ������ ���� Ÿ�� ��ȯ
	// Ư¡) ���������δ� OK
	{
		Knight knight;
		// ������� : ������=����

		// [ �ּ� ] -> [ Dog ]
		Dog& dog = (Dog&)knight;
		dog._cuteness = 12;
	}

	// --------------- [1] ��� ���� Ŭ������ ������ ��ȯ ------------------
	{
		Dog dog;
		BullDog bulldog = dog;
	}


	Dog* dog5 = new Dog;
	int* arr = new int;

	delete dog5; // �Ҹ��ڰ� �Ҹ�
	delete arr;

	return 0;
}