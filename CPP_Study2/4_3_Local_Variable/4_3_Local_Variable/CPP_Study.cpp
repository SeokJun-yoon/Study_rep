#include <iostream>
using namespace std;

// ������ ���� : ���� ������ �� ����

// ���� ���� (Data ������ ��)
// ����) �ʱ�ȭ ����, const �� .rodata .data .bss
// ���� ������ �����ϴ� ���� ������ ��. �ڵ尡 �������� ���� �߸� �ǵ帮�ų� �Ǽ��� Ȯ���� ������. ���������� �� Ȱ���� ��
int globalValue=0;

void Test()
{
	cout << "���� ������ ���� " << globalValue << endl;
	globalValue++;
}

void IncreaseHp(int hp)
{
	hp=hp+1;
}

int main()
{
	cout << "���� ������ ���� " << globalValue << endl;
	globalValue++;

	//Test();

	int hp=1;
	cout << "Increase ȣ�� �� : " << hp <<endl;
	IncreaseHp(hp);
	cout << "Increase ȣ�� �� : " << hp << endl;
	// ���� ����
	int localValue = 0;

	return 0;
}