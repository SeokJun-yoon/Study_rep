#include <iostream>
#include <ctime>
using namespace std;
#include <vector>

// ���� : ��������

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		int num = rand() % 100;
		v.push_back(num);
	}

	// Q1) number��� ���ڰ� ���Ϳ� �ִ��� üũ�ϴ� ��� (return �� ���� : bool or ó������ �����ϴ� iterator)
	{
		int number = 50;

		bool found = false;

		// TODO

		// �˰��� : for���� ������ ó������ ������ �����鼭 ���빰�� number ������ ������ ���ϰ�,
		// number�� ���� ���� �ִٸ� bool���� true�� �����ϱ�
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i] == number)
			{
				found = true;
				cout << v[i] << endl;
				cout << found << endl;
				break;
			}
			else
			{
				cout << v[i] << " : NO" << endl;
			}
		}
	}

	// Q2) 11�� ������ ���ڰ� ���Ϳ� �ȿ� �ִ��� üũ�ϴ� ��� (bool or ó������ �����ϴ� iterator)
	{
		bool found = false;

		 // TODO

		 // �˰��� : for���� ������ ó������ ������ �����鼭 v[i] % 11 == 0 �� ���
		 // bool�� true�� ����

		for (int i = 0; i < v.size(); i++)
		{
			cout << v[i] << endl;

			if (v[i] % 11 == 0)
			{
				found = true;
				cout << "found" << endl;
				break;
			}
		}


	}

	//// Q3) Ȧ���� ������ ������? (count)
	{
		int count = 0;

		// TODO
		// �˰��� : for���� ������ ó������ ������ ���鼭 v[i] % 2 != 0�� ���, count++
		for (int i = 0; i < v.size(); i++)
		{
			if (v[i] %2 != 0)
			{
				count++;
			}
		}
		cout << count << endl;
	}

	// Q4) ���Ϳ� �� �ִ� ��� ���ڵ鿡 3�� ������ ��
	{
		// TODO
		// �˰��� : for���� ������ ó������ ������ ���鼭 v[i] *= 3 ���� ��
		for (int i = 0; i < v.size(); i++)
		{
			cout << "[" << i << "]" << "x3 �� : " << v[i];
			v[i] *= 3;
			cout << " -> " << v[i] << endl;
		}
	}

	return 0;
}