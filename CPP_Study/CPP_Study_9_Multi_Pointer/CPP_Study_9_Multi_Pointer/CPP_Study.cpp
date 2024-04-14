#include <iostream>
using namespace std;

// 오늘의 주제 : 다중 포인터

void SetNumber(int* a)
{
	*a = 1;
}

void SetMessage(const char* a)
{
	a = "Bye";
}

void SetMessage(const char** a)
{
	*a = "Bye";
}

void SetMessage2(const char*& a)
{
	a = "Wow";
}

int main()
{
	int a = 0;
	SetNumber(&a);
	//cout << a << endl;

	// .rdata Hello주소[H][e][l][l][o][\0]
	// .rdata Bye주소 [B][y][e][\0]
	// msg[ Hello 주소 ] << 8바이트
	const char* msg = "Hello";

	// [매개변수][RET][지역변수(msg( Hello 주소))]
	SetMessage(msg);
	//cout << msg << endl;

	// 주소2 [ ] << 1바이트
	// 주소1[ 주소2] << 8바이트
	// pp[ 주소 ] << 8바이트
	const char** pp=&msg;

	// [매개변수][RET][지역변수(msg( Hello주소 ))][매개변수(a(&msg msg의주소))][RET][지역변수]
	SetMessage(&msg);
	*pp = "Bye";
	//cout << msg << endl;

	// 다중 포인터 : 혼동스럽다?
	// 그냥 양파까기라고 생각하면 된다
	// *을 하나씩 까면서 타고 간다고 생각하면 된다
	SetMessage2(msg);
	cout << msg << endl;

	return 0;
}