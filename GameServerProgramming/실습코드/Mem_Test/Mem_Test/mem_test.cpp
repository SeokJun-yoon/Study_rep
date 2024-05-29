#include <iostream>
#include <chrono>
#include <windows.h>

using namespace std;
using namespace std::chrono;

#define CACHE_LINE_SIZE 64

int main()
{
	for (int i = 0; i < 21; ++i) 
	{
		int size = (1024 * 1024) << i;
		char* a = (char*)malloc(size);
		unsigned int index = 0;
		volatile unsigned int tmp = 0;
		auto start = high_resolution_clock::now();
		for (int j = 0; j < 100000000; ++j) 
		{
			tmp += a[index % size];
			index += CACHE_LINE_SIZE * 11;
		}
		auto dur = high_resolution_clock::now() - start;
		cout << "SIZE : " << size / (1024 * 1024) << "M. Time : ";
		cout << duration_cast<milliseconds>(dur).count() << "msecs\n";
		free((void*)a);
	}
}