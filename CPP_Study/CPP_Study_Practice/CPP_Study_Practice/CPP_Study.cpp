#include <iostream>
using namespace std;

int main()
{
#pragma region �����_1
	//// �����()
	//// �������� � ������ �Է��ϸ�
	//// N*N���� ���� ������� ���ڴ�.
	//int n;

	//cout << "N*N�� ����⸦ �� N�� �Է��ϼ���: ";
	//cin >> n ;

	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = 0; j < n; j++)
	//	{
	//		cout << "*";
	//	}
	//	cout << endl;
	//}
#pragma endregion


#pragma region �����_2
//	// �����_2
//	// 1������ �����ؼ� ���������� �ٸ��� ���� �����ϰ� ����
//	int n;
//	cout << "N���� �ٿ� N���� ����⸦ �� N�� �Է��ϼ���: ";
//
//	cin >> n ;
//
//	for (int i = 0; i < n; i++)
//		{
//			for (int j = 0; j < i+1; j++)
//				{
//					cout << "*";
//				}
//		cout << endl;
//}
#pragma endregion

#pragma region �����_3
//	// �����_3
//	// N������ �����ؼ� ���������� �ٸ��� ���� 1���� �پ��� ����
//	int n;
//	cout << "N���� �ٿ��� 1���� �پ��� ����⸦ �� N�� �Է��ϼ���: ";
//
//	cin >> n ;
//
//	for (int i = 0; i < n; i++)
//		{
//			// 0��°�� = 4��
//			// 1��°�� = 3��
//			// 2��°�� = 2��
//			// i��° �� = 4-i��
//			for (int j = 0; j < n-i; j++)
//				{
//					cout << "*";
//				}
//		cout << endl;
//}
#pragma endregion

#pragma region ������_���
	// ������)
	// 2*1=2
	// 2�ܺ���~9�ܱ��� �������� ������ּ���

	for (int i = 2; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			int answer = i * j;
			cout << i << " * " << j << " = " << answer << endl;
		}
		cout << endl;
	}
#pragma endregion
}