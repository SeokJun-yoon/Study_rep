#include <iostream>
#include <time.h>
using namespace std;


int main()
{
	srand(time(NULL)); // �õ� ����
	
	// 1 2 3
	
	const int SCISSORS = 1;
	const int ROCK = 2;
	const int PAPER = 3;

	
	int game_count=0;	//	�� ��
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
			else if (computervalue==ROCK)
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