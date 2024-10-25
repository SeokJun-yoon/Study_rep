#include <iostream>
using namespace std;
#include <vector>

template<typename T>
class Vector
{
public:
	Vector() : _data(nullptr), _size(0), _capacity(0)
	{

	}

	~Vector()
	{
		if (_data)
			delete[] _data;
	}

	void push_back(const T& val)
	{
		if (_size==_capacity)
	}

	int size() { return _size; }
	int capacity() { return _capacity; }

private:
	T* _data;
	int _size;
	int _capacity;
};

int main()
{
	vector<int> v;

	for (int i = 0; i < 100; i++)
	{
		v.push_back(i);
		cout << v.size() << " " << v.capacity() << endl;
	}

	for (int i = 0; i < v.size(); i++)
	{
		cout << v[i] << endl;
	}

	cout << "----------------------------" << endl;

	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << (*it) << endl;
	}

	return 0;
}
