#include <iostream>
using namespace std;

// ������ ���� : �б⹮
// �����͸� �޸𸮿� �Ҵ��ϰ� �����ϴ� ����� ���ؼ��� �˾ƺ�
// ������ �����͸� �̿��ؼ� �����ΰ��� �ϰ� �ʹٸ�?

int main()
{
	int hp = 100; // ���� HP
	int damage = 100; // �÷��̾� ������

	hp -= damage; // �ǰ� ����
	bool isDead = (hp <= 0); // óġ ����


	// ���Ͱ� �׾����� ����ġ �߰�
	// ��������� CMP, JMP


	// �ڵ� ��Ÿ���� ���, �Ϲ������� if ��ĭ ��� (����)
	// ������ �����̱� �ϴ�

	// if (isDead==true)
	// if�� �߰�ȣ�� ���� �ʾ��� ���, �ٷ� �ؿ� �� �ٱ����� ��ȿ
	if (isDead)
		cout << "���͸� óġ�߽��ϴ�." << endl;

	// if (!isDead)
	// if/else �ȿ� if/else���� ��ø���ε� ����� ����
	// �������� ����Ͽ� �ʹ� ���� ��ø X
	// else if�� ����ϸ� �������� ������ ��. 

	//else
	//{
	//	// TODO
	//	if (hp<20)
	//		cout << "���Ͱ� �������� �ֽ��ϴ�" << endl;

	//	else
	//		cout << "���Ͱ� �ݰ��߽��ϴ�." << endl;
	//}
	else if (hp <= 20)
		cout << "���Ͱ� �������� �ֽ��ϴ�" << endl;
	else
		cout << "���Ͱ� �ݰ��߽��ϴ�" << endl;

	const int ROCK = 0;
	const int PAPER = 1;
	const int SCISSORS = 2;

	int input = ROCK;

	if (input == ROCK)
		cout << "������ �½��ϴ�" << endl;
	else if (input == PAPER)
		cout << "���� �½��ϴ�" << endl;
	else if (input == SCISSORS)
		cout << "������ �½��ϴ�" << endl;
	else
		cout << "�� ���̴ϱ�?" << endl;

	// switch - case - break
	// break�� ������Ű�� ������� �޶��� �� �����Ƿ� break ����
	// break�� �Ȱɸ� switch���� ��Ż���� �ʰ� �� ���� case ������� �۵���

	// ���� �迭�� ���� �� �ִ�.
	// if/else�� ���� ���뼺�� ��������
	switch (input)
	{
	case ROCK:
		cout << "���� �½��ϴ�" << endl;
		break;
	case PAPER:
		cout << "���� �½��ϴ�" << endl;
		break;
	case SCISSORS:
		cout << "������ �½��ϴ�" << endl;
		break;
	// else�� ������ ��
	default:
		cout << "�� ���̴ϱ�?" << endl;
	}
	
}