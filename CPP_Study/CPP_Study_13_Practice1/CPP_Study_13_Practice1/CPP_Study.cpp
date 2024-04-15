#include <iostream>
using namespace std;

// 오늘의 주제 : 연습문제 (문자열 관련)

// 문제1) 문자열 길이를 출력하는 함수 (StrLen 함수 만들어보기)
int StrLen(const char* str)
{
	// str 이라는 문자열의 길이를 반환
	int count = 0;
	while (str[count] != '\0')
	{
		count++;
	}

	return count;
}

// 문제2) 문자열 복사 함수
// [H][e][l][l][o]['\0'][][][][][][][][][][][]
// [H][e][l][l][o]['\0'][][][][][][][][][][][]

char* StrCpy(char* dest, char* src)
{
	//// 배열을 활용한 버전
	//int i = 0;
	//while (src[i] != '\0')
	//{
	//	dest[i] = src[i];
	//	i++;
	//}

	//dest[i] = '\0';

	//return dest;

	// 포인터를 활용한 버전

	char* ret = dest;

	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}

	*dest = '\0';

	return ret;
}


// 문제3) 문자열 덧붙이는 함수
// 내가 푼 풀이
//char* StrCat(char* dest, char* src)
//{
//	int index_dest = 0;
//	int index_src = 0;
//	while (src[index_src]!='\0')
//	{
//		if (dest[index_dest] != 0)
//		{
//			index_dest++;
//		}
//		else
//		{
//			dest[index_dest] = src[index_src];
//			index_src++;
//		}
//	}
//
//	return dest;
//}

// 강사님 풀이
char* StrCat(char* dest, char* src)
{
	//// 배열을 활용한 방법
	//int len = StrLen(dest);

	//int i = 0;
	//while (src[i] != '\0')
	//{
	//	dest[len + i] = src[i];
	//	i++;
	//}
	//dest[len + i] = '\0';

	//return dest;

	// 포인터를 활용한 방법

	char* ret = dest;

	while (*dest != '\0')
		dest++;

	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';

	return ret;
}

// 문제4) 두 문자열을 비교하는 함수
// 내가 푼 풀이
//int StrCmp(char* a, char* b)
//{
//	int i = 0;
//	while (1)
//	{
//		if (a[i] - b[i]==0)
//		{
//			i++;
//			if ((a[i] == '\0') && (b[i] != '\0'))
//				return -1;
//			else if ((a[i] != '\0') && (b[i] == '\0'))
//				return 1;
//		}
//		else if (a[i] > b[i])
//		{
//			return 1;
//		}
//		else
//		{
//			return -1;
//		}
//	}
//}

// 강사님 풀이
int StrCmp(char* a, char* b)
{
	int i = 0;

	while (a[i] != '\0' || b[i] != '\0')
	{
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
		i++;
	}

	return 0;
}

// 문제5) 문자열을 뒤집는 함수
// 내가 푼 풀이
//void ReverseStr(char* str)
//{
//	int i = 0;
//	int Str_Len = strlen(str);
//	char temp = 0;
//
//	while (str[i] != '\0')
//	{
//		temp = str[i];
//		str[i] = str[(Str_Len - 1)-i];
//		str[(Str_Len - 1) - i] = temp;
//		i++;
//		if (i*2 >=Str_Len)
//			break;
//	}
//}

// 강사님 풀이
void ReverseStr(char* str)
{
	int len = StrLen(str);

	for (int i = 0; i < len / 2; i++)
	{
		int temp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = temp;
	}
}

#pragma warning(disable: 4996)
int main()
{
	const int BUF_SIZE = 100;

	// [H][e][l][l][o]['\0'][][][][][][][][][][][]
	char a[BUF_SIZE] = "HelloWorld";
	char b[BUF_SIZE] = "bb";

	//int len = StrLen(a);
	//cout << len;

	//StrCpy(b, a);
	//cout << b << endl;

	//StrCat(a, b);
	//cout << a << endl;


	//int value= StrCmp(a, b);
	//cout << value << endl;
	ReverseStr(a);
	cout << a << endl;
	return 0;
}