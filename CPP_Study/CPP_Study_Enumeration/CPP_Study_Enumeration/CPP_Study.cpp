#include <iostream>
#include <time.h>
using namespace std;


int main()
{
	srand(time(NULL)); // �õ� ����

	// 1 2 3

	// ����ΰ� �˰ڴµ�. �ʹ� ���� ���� ��� ����?
	// �ϳ��� ��Ʈ�ε�?
	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;

	// ���ڸ� ���� ���ϸ� ù ���� 0���� ����
	// �� ���� ������ ���� �� +1
	enum ENUM_SRP
	{
		ENUM_SCISSORS=1,
		ENUM_ROCK,
		ENUM_PAPER
	};

	// #�� ������ -> ��ó�� ���ù�
	// #include <iostream>�̶�� ������ ã�Ƽ� �ش� ������ �׳� ����/�ٿ��ֱ� �ϴ� ����.
	//1) ��ó�� 2) ������ 3) ��ũ
#define DEFINE_SCISSROS 1
#define DEFINE_ROCK 2
#define DEFINE_PAPER 3
#define DEFINE_TEST cout << "Hello World" << endl;


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
			if (computervalue == ENUM_SCISSORS)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				break;
			}
			else if (computervalue == ENUM_ROCK)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == ENUM_PAPER)
			{
				cout << "����(�÷��̾�)" << " vs " << "��(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else
				break;

		case 2:
			if (computervalue == ENUM_SCISSORS)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == ENUM_ROCK)
			{
				cout << "����(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				break;
			}
			else if (computervalue == ENUM_PAPER)
			{
				cout << "����(�÷��̾�)" << " vs " << "��(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else
				break;

		case 3:
			if (computervalue == ENUM_SCISSORS)
			{
				cout << "��(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �����ϴ�! >> " << endl;
				game_count++;
				break;
			}
			else if (computervalue == ENUM_ROCK)
			{
				cout << "��(�÷��̾�)" << " vs " << "����(��ǻ��) " << "<< �̰���ϴ�! >> " << endl;
				game_count++;
				win_count++;
				break;
			}
			else if (computervalue == ENUM_PAPER)
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