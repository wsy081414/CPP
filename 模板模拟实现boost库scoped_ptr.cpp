#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>

using namespace std;


template<typename T>
class ScopedPtr
{
public:
	ScopedPtr( T* ptr=NULL)
		:_ptr(ptr)
	{
	}
	~ScopedPtr()
	{
		if (_ptr != NULL)
		{
			delete _ptr;
		}
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
	ScopedPtr(ScopedPtr <T> & sp);
	ScopedPtr<T>& operator=(ScopedPtr<T>&ap);
private:
	T *_ptr;

};

void test1()
{
	ScopedPtr<int>sp1(new int(15));
	ScopedPtr<int>sp2;

	cout << *sp1 << endl;
	
}

int main()
{
	test1();
	system("pause");
	return 0;
}