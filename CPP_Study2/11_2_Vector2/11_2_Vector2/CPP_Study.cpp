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

	// �ݺ���(Iterator) : �����Ϳ� ������ ����. �����̳��� ����(������)�� ����Ű��, ����/���� ���ҷ� �̵� ����

	vector<int> v(10);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
	{
		v[i] = i;
	}

	//vector<int>::iterator it;
	//int* ptr;		

	//it = v.begin();
	//ptr = &v[0];

	//cout << (*it) << endl;
	//cout << (*ptr) << endl;

	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();

	// �� ������ ���̴µ�?
	// �ٸ� �����̳ʴ� v[i]�� ���� �ε��� ������ �� �� ���� ����
	// iterator�� vector�� �ƴ϶�, �ٸ� �����̳ʿ��� ���������� �ִ� ���� (���� vector�� �ƴ� �ٸ� �����̳ʷ� �ٲ㵵 ������ �����ϴ�)
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << (*it) << endl;
	}

	int* ptrBegin = &v[0]; // v.begin()._Ptr;
	int* ptrEnd = ptrBegin + 10; // v.end()._ptr;
	for (int* ptr = ptrBegin; ptr != ptrEnd; ++ptr)
	{
		cout << (*ptr) << endl;
	}

	// const int*;
	//vector<int>::const_iterator cit1 = v.cbegin();
	// *cit1 = 100; // const�̹Ƿ� ���� �Ұ�
	
	// ���� Ȱ���� ���� ����. �˾Ƹ� �� ��
	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++i)
	{
		cout << (*it) << endl;
	}

	return 0;
}