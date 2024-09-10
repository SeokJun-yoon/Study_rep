#include <iostream>
using namespace std;
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

// ������ ���� : ����Ʈ ������ (smart pointer) 

class Knight
{
public:
	Knight() { cout << "Knight ����" << endl; }
	~Knight() { cout << "Knight �Ҹ�" << endl; }

	void Attack()
	{
		if (_target)
		{
			_target->_hp -= _damage;
			cout << "HP: " << _target->_hp << endl;
		}
	}

public:
	int _hp = 100;
	int _damage = 10;
	shared_ptr<Knight> _target = nullptr;

};

class RefCountBlock
{
public:
	int _refCount = 1;
};

template<typename T>
class SharedPtr
{
public:
	SharedPtr() { }
	SharedPtr(T* ptr) : _ptr(ptr)
	{
		if (_ptr != nullptr)
		{
			_block = new RefCountBlock();
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	SharedPtr(const SharedPtr& sptr) : _ptr(sptr._ptr), _block(sptr._block)
	{
		if (_ptr != nullptr)
		{
			_block->_refCount++;
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	void operator=(const SharedPtr& sptr)
	{
		_ptr = sptr._ptr;
		_block = sptr._block;

		if (_ptr != nullptr)
		{
			_block->_refCount++;
			cout << "RefCount : " << _block->_refCount << endl;
		}
	}

	~SharedPtr()
	{
		if (_ptr != nullptr)
		{
			_block->_refCount--;
			cout << "RefCount : " << _block->_refCount << endl;

			if (_block->_refCount == 0)
			{
				delete _ptr;
				delete _block;
				cout << "Delete Data" << endl;
			}
		}
	}
public:
	T* _ptr = nullptr;
	RefCountBlock* _block = nullptr;
};



int main()
{
	//Knight* k1 = new Knight();
	//Knight* k2 = new Knight();

	//k1->_target = k2;

	//delete k2;

	//k1->Attack();

	// ����Ʈ ������ : �����͸� �˸´� ��å�� ���� �����ϴ� ��ü (�����͸� �����ؼ� ���)
	// shared_ptr, weak_ptr, unique_ptr
	//SharedPtr<Knight> k2 ;
	//{
	//	SharedPtr<Knight> k1(new Knight());
	//	 k2 = k1;
	//}

	shared_ptr<Knight> k1 = make_shared<Knight>();

	{
		shared_ptr<Knight> k2 = make_shared<Knight>();
		k1->_target = k2;
	}


	return 0;
}