#include <iostream>
using namespace std;

// 오늘의 주제 : 타입 변환

// malloc -> void*을 반환학, 이를 우리가 (타입 변환)을 통해 사용했었음

class Knight
{
public:
	int _hp = 10;
};

class Dog
{
public:
	// 타입 변환 생성자
	Dog(const Knight& knight)
	{
		_age = knight._hp;
	}

	// 타입 변환 연산자
	operator Knight()
	{
		return (Knight)(*this);
	}
public:
	int _age = 1;
	int _cuteness = 2;
};

class BullDog : public Dog
{
public:
	bool _french; // 프렌치 불독
};

int main()
{
	// --------------- 타입 변환 유형 (비트열 재구성 여부) ------------------

	// [1] 값 타입 변환
	// 특징) 의미를 유지하기 위해서, 원본 객체와 다른 비트열 재구성
	{
		int a = 123456789;	// 2의 보수
		float b = (float)a;	// 부동소수점(지수 + 유효숫자)
		cout << b << endl;
	}

	// [2] 참조 타입 변환
	// 특징) 비트열을 재구성하지 않고, '관점'만 바꾸는 것
	// 거의 쓸 일은 없지만, 포인터 타입 변환도 '참조 타입 변환'과 동일한 룰을 따르니까 일석이조로 공부하자.

	{
		int a = 123456789;	// 2의 보수
		float b = (float&)a;	// 부동소수점(지수 + 유효숫자)
		cout << b << endl;
	}

	// --------------- 안전도 분류 ------------------
	// 특징) 의미가 항상 100% 완전히 일치하는 경우
	// 같은 타입이면서 크기만 더 큰 바구니로 이동
	// 작은 바구니 -> 큰 바구니로 이동 OK (업캐스팅)
	// ex) char -> short, short -> int, int -> __int64
	{
		int a = 123456789;
		__int64 b = a;
		cout << b << endl;
	}

	// [2] 불안전한 변환
	// 특징) 의미가 항상 100% 일치한다고 보장하지 못하는 경우
	// 타입이 다르거나
	// 같은 타입이지만 큰 바구니 -> 작은 바구니 이동 (다운캐스팅)
	{
		int a = 123456789;
		float b = a;
		short c = a;
		cout << b << endl;
		cout << c << endl;
	}

	// --------------- 프로그래머 의도에 따라 분류 ------------------

	// [1] 암시적 변환
	// 특징) 이미 알려진 타입 변환 규칙에 따라서 컴파일러가 '자동'으로 타입 변환
	{
		int a = 123456789;
		float b = a; // 암시적으로
	}

	// [2] 명시적 변환
	{
		int a = 123456789;
		int* b = (int*)a; // 암시적으로
		cout << b << endl;
	}

	// --------------- 아무런 연관 관계가 없는 클래스 사이의 변환 ------------------
	
	// [1] 연관없는 클래스 사이의 '값 타입' 변환
	// 특징) 일반적으로 안 됨(예외 : 타입 변환 생성자, 타입 변환 연산자)6
	{
		Knight knight;
		Dog dog = (Dog)knight;

		Knight knight2 = dog;
	}

	// [2] 연관없는 클래스 사이의 참조 타입 변환
	// 특징) 명시적으로는 OK
	{
		Knight knight;
		// 어셈블리 : 포인터=참조

		// [ 주소 ] -> [ Dog ]
		Dog& dog = (Dog&)knight;
		dog._cuteness = 12;
	}

	// --------------- [1] 상속 관계 클래스의 사이의 변환 ------------------
	{
		Dog dog;
		BullDog bulldog = dog;
	}


	Dog* dog5 = new Dog;
	int* arr = new int;

	delete dog5; // 소멸자가 불림
	delete arr;

	return 0;
}