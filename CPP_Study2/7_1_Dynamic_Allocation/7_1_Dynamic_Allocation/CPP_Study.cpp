#include <iostream>
using namespace std;

// ������ ����: ���� �Ҵ�

// �޸� ���� ����
// -������ �ڵ尡 ����Ǵ� ���� -> �ڵ� ����
// - ����(global) / ����(static) ���� -> ������ ����
// - ���� ���� / �Ű� ���� -> ���� ����
// - ���� �Ҵ� -> �� ����

// ���ݱ��� ������ ���� / ���� ���� �̿��ؼ�
// �̷� ���� ���α׷��� �� ����� �Դ�!
// ���� ���ο� ������ �ʿ��ұ�?

// ���� ��Ȳ)
// - MMORPG ���� 1��~5����, ���� 1����~ 500������
// - ���� ���� �̺�Ʈ -> 5�е��� ���Ͱ� 10�� ���� ����

// ���� ����
// �Լ��� ������ ���� �����Ǵ� �Ҿ����� �޸�
// ��� �Լ��� �Ű����� �ѱ�ٰų�, �ϴ� �뵵�δ� OK
// - �޸� ����
// ���α׷��� ����Ǵ� ���߿��� '������' ���Ǵ�

// �������)
// - �ʿ��� ���� ����ϰ�, �ʿ������ �ݳ��� �� �ִ�!
// - �׷��鼭�� (���ð��� �ٸ���) �츮�� ����/�Ҹ� ������ ������ �� �ִ�!
// - �׷� �Ƹ��ٿ� �޸� ����? -> HEAP
// �����Ҵ�� ������ �Լ�/������ : malloc / free, new / delete, new[] / delete[]

// malloc
// - �Ҵ��� �޸� ũ�⸦ �ǳ��ش�.
// - �޸� �Ҵ� �� ���� �ּҸ� ����Ű�� �����͸� ��ȯ���ش�. (�޸� ������ ��� NULL ��ȯ)

// free
// - malloc (Ȥ�� ��Ÿ calloc, realloc ���� ����) �� ���� �Ҵ�� ������ ����
// - �� �����ڰ� �Ҵ�/���Ҵ�

// new / delete
// - C++�� �߰���
// - malloc/free �Լ�! new/delete�� ������(operator)
class Monster
{
public:
	Monster()
	{
		cout << "Monster()" << endl;
	}

	~Monster()
	{
		cout << "~Monster()" << endl;
	}

public:
	int _hp;
	int _x;
	int _y;
};

int main()
{
	// ���� ���� [�޸���] [LOL] [���÷��̾�]
	// -----------------------------------
	// Ŀ�� ���� (Windows ���� �ٽ� �ڵ�)

	// ���� ����) �ü������ �����ϴ� API ȣ��
	// Ŀ�� ����) �޸� �Ҵ��ؼ� �ǳ���
	// ���� ����) �� �� �� ���Կ�

	// C++������ �⺻������ CRT(C��Ÿ�� ���̺귯��)�� [�� ������]�� ���� �� ���� ���
	// ��, ���� ���Ѵٸ� �츮�� ���� API�� ���� ���� �����ϰ� ������ ���� ���� (MMORPG ���� �޸� Ǯ��)

	// �׷��� ���! void*?? �����ϱ�
	// *�� �����ϱ� �����ʹ� ������ (�ּҸ� ��� �ٱ���) => OK
	// Ÿ�� ���� void �ƹ��͵� ���� ? => NO
	// Ÿ�� ���� void ���� �ִ��� �𸣰����ϱ� �ʰ� ������ ��ȯ�ؼ� ����ض� => OK
	void* pointer = malloc(sizeof(Monster));	// 1000�� ����Ʈ ���� �ǹ���

	Monster* m1 = (Monster*)pointer;
	m1->_hp = 100;
	m1->_x = 1;
	m1->_y = 2;

	// Heap Overflow
	// - ��ȿ�� �� ������ �ʰ��ؼ� ����ϴ� ����

	// free�� �����ָ�? �ٷ� ũ���ð� ������ ������, ���̺� �ڵ忡���� �޸� ���� ������ �Ͼ �� �ִ�.
	free(pointer);
	pointer = nullptr;
	m1 = nullptr;

	// Double Free
	// - �̰� ��κ� ũ���ø� ���� ������
	//free(pointer);

	// Use-After-Free
	// - ���α׷��� ���� : OMG ���ߴ�!
	// - ��Ŀ ���� : �ɺô�!

	// new-delete
	// - new�� malloc�� ���� ���� �ѵ�~ �迭�� ���� N�� �����͸� ���� �Ҵ��Ϸ���?

	// malloc/free vs new/delete
	// - ��� ���Ǽ� -> new/delete ��!
	// - Ÿ�Կ� ������� Ư���� ũ���� �޸� ������ �Ҵ��������? -> malloc/free ��!

	// �׷��� ���� ���� ���� �ٺ����� �߿��� ���̴� ���� ����!
	// �ڡڡڡڡ� new/delete�� (����Ÿ���� Ŭ������ ���) ������/�Ҹ��ڸ� ȣ�����ش�!
	
	Monster* m2 = new Monster;
	m2->_hp = 200;
	m2->_x = 2;
	m2->_y = 3;
	delete m2;

	Monster* m3 = new Monster[5];
	m3->_hp = 200;
	m3->_x = 2;
	m3->_y = 3;

	Monster* m4 = (m3 + 2);
	m4->_hp = 300;
	m4->_x = 4;
	m4->_y = 5;

	delete[] m3;

	return 0;
}