#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>
#include<cassert>
using namespace std;
template<typename T>
class Stack
{
public:
	Stack()
		:_pdata(NULL)
		, _sz(0)
		, _capacity(0)
	{
	
	}
	void Push(const T& d)
	{
		CheckCapacity();
		_pdata[_sz++] = d;
	}
	void Pop()
	{
		assert(_sz > 0);
		_sz--;
	}
	T Top()
	{
		assert(_sz > 0);
		return _pdata[_sz-1];
	}
	size_t Size()
	{
		return _sz;
	}
	bool Empty()
	{
		if (_sz == 0)
			return true;
		else
			return false;
	}
protected:
	void CheckCapacity()
	{
		if (_sz == _capacity)
		{
			size_t NewCapacity = _capacity * 2 + 1;
			T* Temp = new T[NewCapacity];
			for (size_t i = 0; i < _sz; i++)
			{
				Temp[i] = _pdata[i];
			}
			delete _pdata;
			_pdata = Temp;
			_capacity = NewCapacity;
		}
	}
protected:
	T * _pdata;
	size_t _sz;
	size_t _capacity;
};

