#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

const int T_SIZE = 10000000;

short rand_arr[T_SIZE];

#define abs(x) (((x)>0)?(x):-(x))

static long abs2(long x) 
{
	long y;
	y = x >> 31;    /* Not portable */
	return (x ^ y) - y;
}

int main()
{
	int sum;
	for (int i = 0; i < T_SIZE; ++i) rand_arr[i] = rand() - 16384;
	
	sum = 0;
	
	auto start_t = high_resolution_clock::now();

	for (int i = 0; i < T_SIZE; ++i) sum += abs(rand_arr[i]);
	
	auto du = high_resolution_clock::now() - start_t;

	cout << "[abs] Time " << duration_cast<milliseconds>(du).count() << " ms\n";
	cout << "Result : " << sum << endl;
	
	sum = 0;
	start_t = high_resolution_clock::now();

	for (int i = 0; i < T_SIZE; ++i) sum += abs2(rand_arr[i]);

	du = high_resolution_clock::now() - start_t;

	cout << "[abs2] Time " << duration_cast<milliseconds>(du).count() << " ms\n";
	cout << "Result : " << sum << endl;
}
