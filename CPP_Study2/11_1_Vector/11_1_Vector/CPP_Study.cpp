#include <iostream>
using namespace std;
#include <vector>

// ������ ���� : vector
// ���� �ܰ�
int main()
{
	// STL (Standard TEmplate Library)
	// ���α׷����� �� �ʿ��� �ڷᱸ��/�˰������
	// ���ø����� �����ϴ� ���̺귯��

	// �����̳�(Container) : �����͸� �����ϴ� ��ü (�ڷᱸ�� Data Structure)

	// vector (���� �迭)
	// - vector�� ���� ���� (size/capacity)
	// - �߰� ���Ի���
	// - ó��/�� ����/����
	// - ���� ����
	
	// �迭
	//const int MAX_SIZE = 10;
	//int arr[MAX_SIZE] = {};

	//for (int i = 0; i < MAX_SIZE; i++)
	//	arr[i] = i;

	//for (int i = 0; i < MAX_SIZE; i++)
	//	cout << arr[i] << endl;

	// ���� �迭
	// �ſ� �ſ� �߿��� ���� -> � ������ �ηȱ淡 �迭�� '����������' ����� ���ΰ�?

	// 1) (������� �ΰ�) �޸𸮸� �Ҵ��Ѵ�
	// 2) ����б��� �� á����, �޸𸮸� �����Ѵ�.
	
	// Q1) �������� ��ŭ�� �����ұ�?
	// Q2) ������ ��ŭ �ؾ��ұ�?
	// Q3) ������ �����͸� ��� ó���ұ�?

	// [ 1 2 3 4 5 ]
	// [         

	vector<int> v;
	// size (���� ��� ������ ����)
	// 1 2 3 4 5 6 7.....

	// capacity (�������� ������ �뷮 ����)
	// 1 2 3 4  6 9 13 19 28 42 63...
	for (int i = 0; i < 1000; i++)
	{
		v.push_back(100);
		cout << v.size() << " " << v.capacity() << endl;
	}

	return 0;
}