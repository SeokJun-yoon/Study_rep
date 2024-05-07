#include <iostream>
#include <time.h>
using namespace std;

// ������ (enumeration)

// ����� �� �˰ڴµ�, �ʹ� ���� ���� ��� ����?
// �ϳ��� ��Ʈ�ε�?
// const�� ��� �ݵ�� �ʱⰪ�� �ο��ؾ� ��
const int SCISSORS = 1;
const int ROCK = 2;
const int PAPER = 3;

// ���ڸ� �������� ������ 0���� ����
// �� ���� ������ ���� �� + 1
enum ENUM_SRP
{
	ENUM_SCISSORS=1,
	ENUM_ROCK,
	ENUM_PAPER,
};

// #�� ������ -> ��ó�� ���ù�
// #include <iostream> �̶�� ������ ã�Ƽ� �ش� ������ �׳� ����-�ٿ��ֱ�
// 1) ��ó�� 2) ������(�뿪�� ����) 3) ��ũ
// #define �� ��ũ��ȭ ���� ����
#define DEFINE_SCISSORS 1+2
// �Ʒ��� ���� ����� �״�� �� (�ڵ带 �ٲ�ġ���ϴ� ����)
#define DEFINE_TEST cout << "Hello World" << endl;

// ����� ���� �� ����� enum - const - #define ���� ���ٰ� �Ѵ�.
// ��, �ظ��ϸ� #define�� ��������.

int main()
{
	int result = DEFINE_SCISSORS *2; // 6�� �ƴ� 5�� �ȴ�. ��? DEFINE_SCISSORS = 3�� �ƴ�, 1+2�� �״�� ���� ������
	cout << result << endl;
	DEFINE_TEST;

	srand(time(NULL)); // �õ� ����

	// 1 2 3

	int game_count = 0;	//	�� ��
	int win_count = 0;	//	�̱� Ƚ��


	while (1)
	{
		cout << "����(1) / ����(2) / ��(3) ����ּ���!" << endl;
		cout << "> ";

		if (game_count == 0)
		{
			cout << "���� �·��� �����ϴ�." << endl;
		}
		else
		{
			int win_rate = (win_count * 100) / game_count; // �·�
			cout << "���� �·��� " << win_rate << "% �Դϴ�." << endl;
		}

		// ��ǻ��
		int computervalue = 1 + (rand() % 3);	//	��ǻ�Ͱ� �� ��(1~3)

		// �����
		int input;	// ������� �Է°�
		cin >> input;
		switch (input)
		{

		case 1:
			if (computervalue == SCISSORS)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				break;
			}
			else if (computervalue == ROCK)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "����(�÷��̾�)" << " vs " << "��(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else
				break;

		case 2:
			if (computervalue == SCISSORS)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == ROCK)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "����(�÷��̾�)" << " vs " << "��(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else
				break;

		case 3:
			if (computervalue == SCISSORS)
			{
				cout << "��(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == ROCK)
			{
				cout << "��(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == PAPER)
			{
				cout << "��(�÷��̾�)" << " vs " << "��(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				break;
			}
			else
				break;

		default:
			cout << "�߸��� �� �Է�. ������ �����մϴ�." << endl;
			return 0;
		}

	}
}