#include <iostream>
using namespace std;

// ������ ���� : �Լ� ������

// �����ε� (�ߺ� ���� : �Լ� �̸��� ����)
// - �Ű����� ������ �ٸ��ų�
// - �Ű����� Ÿ���� �ٸ��ų� (������ �ٸ��� ����)

int Add(int a, int b)
{
	int result = a + b;
	return result;
}

// float Add(int a, int b)�� �Ұ�.
// ��ȯ���ĸ� �ٸ� ���(�Ű������� ����)���� �Ұ���
float Add(float a, float b)
{
	float result = a + b;
	return result;
}

// �⺻ ���ڰ�
// �Ű����� ���� �⺻���� �ο��ϸ�, �Լ� ���ÿ� ���� �ʾƵ� �⺻������ ������ �ȴ�. ��� �� ���� ���ʿ� ���� �Ѵ�.
void SetPlayerInfo(int hp, int mp, int attack, int gulidid = 0, int castleid=0)
{

}

// ���� �����÷ο�
// �ʹ� ���� ȣ���� ���� �ʵ���
int Factorial(int n)
{
	if (n<=1)
		return 1;

		return n * Factorial(n-1);
}

int main()
{
	//float result = Add(1.5f, 2.1f);
	//cout << result << endl;

	//SetPlayerInfo(100,40,10,0,1);
	
	int result = Factorial(1000000);
	cout << result << endl;
	return 0;
}