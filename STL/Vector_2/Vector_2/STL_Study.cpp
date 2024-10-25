#include <iostream>
using namespace std;
#include <vector>

int main()
{
	// �����̳�(Container) : �����͸� �����ϴ� ��ü (�ڷᱸ�� Data Structure)

	// vector (���� �迭)
	// - vector�� ���� ���� (size/capacity)
	// - �߰� ����/����
	// - ó��/�� ����/����
	// - ���� ����

	// �ݺ��� (Iterator) : �����Ϳ� ������ ����. �����̳��� ����(������)�� ����Ű��, ����/���� ���ҷ� �̵� ����
	vector<int> v(10);

	v.reserve(1000);

	for (vector<int>::size_type i = 0; i < v.size(); i++)
		v[i] = i;

	//vector<int>::iterator it;
	//// iterator ���θ� Ȯ���غ��� ptr�� �����ϴ� ���� �� �� ����
	//int* ptr;

	//it = v.begin();
	//ptr = &v[0];

	//cout << (*it) << endl;
	//cout << (*ptr) << endl;

	//int a = 1;
	////int b = a++; // (b=1, a=2)
	//int c = ++a; // (c=2, a=2)

	//it++;
	//++it;
	//ptr++;
	//++ptr;
	//
	//it--;
	//--it;
	//ptr--;
	//--ptr;

	//it += 2;
	//it = it - 2;
	//ptr += 2;
	//ptr = ptr - 2;
	//// ���� ���� ��. 

	vector<int>::iterator itBegin = v.begin();
	vector<int>::iterator itEnd = v.end();

	// �� �����غ��̴µ�?
	// �ٸ� �����̳ʴ� v[i]�� ���� �ε��� ������ �� �ɼ��� ����
	// iterator�� vector�� �ƴ϶�, �ٸ� �����̳ʿ��� ���������� �ִ� ����
	// ������ vector������ ��� ���� �ܼ��� ����� �� ���ϴ�
	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) // ���� it++ ���� ++it�� ���� �� ȿ������
	{
		cout << (*it) << endl;
	}

	int* ptrBegin = &v[0]; // v.begin()._Ptr;
	int* ptrEnd = ptrBegin + 10; // v.end()._Ptr;
	for (int* ptr = ptrBegin; ptr != ptrEnd; ++ptr)
	{
		cout << (*ptr) << endl;
	}
	
	// const int*
	//vector<int>::const_iterator cit1 = v.cbegin();
	//*cit1 = 100;

	for (vector<int>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it)
	{
		cout << (*it) << endl;
	}

	// - �߰� ����/���� (BAD)
	// - ó��/�� ����/���� (BAD / GOOD)
	// - ���� ���� (Random Access)
	// * �������� �⺻�⿡ ���� �ܰ� ����

	// vector = ���� �迭 = �������� Ŀ���� �迭 = �迭
	// ���Ұ� �ϳ��� �޸� ��Ͽ� �����ϰ� ����ȴ� !!!

	// [                 ]
	// [0][1][2][3][4][][]

	//v.push_back(1);
	//v.pop_back();
	// v.push_back�� v.pop_back�� ������
	// v.push_front�� v.pop_front�� ����. �� ������ ó�� ����/������ �������̶�� ������ Ư����
	// �߰� ����/������ �� �ٸ��ٰ� ���� �����̴�.

	//vector<int>::iterator insertIt = v.insert(v.begin() + 2, 5);
	//// push_back �̳� pop_back���� ��ư� �Ǿ� �ִ� ���� ���� ������ ���Ӽ� ����
	//vector<int>::iterator eraseIt1 = v.erase(v.begin() + 2);
	//vector<int>::iterator eraseIt2 = v.erase(v.begin() + 2, v.begin()+4);

	// ��~ ��ĵ�� �ϸ鼭, 3�̶�� �����Ͱ� ������ �ϰ� �����ϰ� �ʹ�

	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		int data = *it;
		if (data == 3)
		{
			// v.erase(it); �̴�� ���� �ȵ�.
			it = v.erase(it);
		}
		else
		{
			++it;
		}
	}

	// ���� �������� ���ҵ��� �߰����� ����� ������ �������� ������ Ư¡ ������ ������ �����ؾ� �Ѵ�.

	return 0;
}