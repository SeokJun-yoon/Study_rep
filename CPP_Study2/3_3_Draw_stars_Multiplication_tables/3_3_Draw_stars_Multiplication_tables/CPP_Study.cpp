#include <iostream>
using namespace std;

// cout -> console out
// endl -> endline
int main()
{
	// ����� (1)
	// �������� � ������ �Է��ϸ� N*N���� ���� ���

	// solution)
	//int input;
	//cin >> input;	// �Է� �ޱ�
	//
	//for (int i = 0; i < input; i++)
	//{
	//	for (int j = 0; j < input; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}

	// ����� (2)
	// 1������ �����ؼ� ���������� �ٸ��� �����ϰ� ����

	// solution)
	//int input;
	//cin >> input;

	//for (int i = 0; i < input; i++)
	//{
	//	for (int j = 0; j < i + 1; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}
	
	// ����� (3)
	// N������ �����ؼ� �ٸ��� 1���� �پ��� ���·�

	//int input;
	//cin >> input;

	//for (int i = 0; i < input; i++)
	//{
	//	// N=4 �� ���,
	//	// 0��°�� = 4��
	//	// 1��°�� = 3��
	//	// 2��°�� = 2��
	//	// i��°�� = 4-i��
	//	for (int j = 0; j < (input - i); j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}


	// ������
	// 2 * 1 = 2
	// 2�ܺ���~9�ܱ��� �������� ����ϱ�

	// solution)
	for (int i = 2; i <= 9; i++)
	{
		for (int j = 1; j <= 9 ; j++)
		{
			cout << i << " * " << j << " = " << i*j << endl;
		}
	}
	
}