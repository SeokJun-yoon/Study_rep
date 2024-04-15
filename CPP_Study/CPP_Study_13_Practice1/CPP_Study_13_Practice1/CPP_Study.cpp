#include <iostream>
using namespace std;

// ������ ���� : �������� (���ڿ� ����)

// ����1) ���ڿ� ���̸� ����ϴ� �Լ� (StrLen �Լ� ������)
int StrLen(const char* str)
{
	// str �̶�� ���ڿ��� ���̸� ��ȯ
	int count = 0;
	while (str[count] != '\0')
	{
		count++;
	}

	return count;
}

// ����2) ���ڿ� ���� �Լ�
// [H][e][l][l][o]['\0'][][][][][][][][][][][]
// [H][e][l][l][o]['\0'][][][][][][][][][][][]

char* StrCpy(char* dest, char* src)
{
	//// �迭�� Ȱ���� ����
	//int i = 0;
	//while (src[i] != '\0')
	//{
	//	dest[i] = src[i];
	//	i++;
	//}

	//dest[i] = '\0';

	//return dest;

	// �����͸� Ȱ���� ����

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


// ����3) ���ڿ� �����̴� �Լ�
// ���� Ǭ Ǯ��
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

// ����� Ǯ��
char* StrCat(char* dest, char* src)
{
	//// �迭�� Ȱ���� ���
	//int len = StrLen(dest);

	//int i = 0;
	//while (src[i] != '\0')
	//{
	//	dest[len + i] = src[i];
	//	i++;
	//}
	//dest[len + i] = '\0';

	//return dest;

	// �����͸� Ȱ���� ���

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

// ����4) �� ���ڿ��� ���ϴ� �Լ�
// ���� Ǭ Ǯ��
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

// ����� Ǯ��
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

// ����5) ���ڿ��� ������ �Լ�
// ���� Ǭ Ǯ��
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

// ����� Ǯ��
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