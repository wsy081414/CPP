#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>

using namespace std;



template <typename T>
class AutoPtr
{
public:
	AutoPtr( T* ptr=NULL)
		:_ptr( ptr)
		
	{
	}
	~AutoPtr()
	{
		if (_ptr != NULL)
		{
			delete _ptr;
		}
	}
	AutoPtr( AutoPtr<T> & ap)
	{
		_ptr = ap._ptr;
		ap._ptr = NULL;
	}
	AutoPtr<T>& operator =(AutoPtr<T>& ap)
	{
		delete _ptr;
		_ptr = ap._ptr;
		ap._ptr = NULL;
		return *this;
	}
	T* operator ->()
	{
		return _ptr;
	}
	T& operator *()
	{
		return *_ptr;
	}
private:
	T *_ptr;
};

struct	A
{
	int _b;
	char _a;
};
void test()
{

	AutoPtr<A>ap1(new A);
	ap1->_a = 'a';
	ap1->_b = 30;
	AutoPtr<A>ap2;
	ap2 = ap1;
	
}

int main()
{
	test();
	system("pause");
	return 0;
}