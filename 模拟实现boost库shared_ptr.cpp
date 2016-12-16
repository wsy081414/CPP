#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>

using namespace std;

template<typename T>
class SharedPtr
{
public:
	
	SharedPtr(T* ptr)
		:_ptr(ptr)
		, _pCount(new int (1))
	{
	
	}
	~SharedPtr()
	{
		release();
	}
	SharedPtr(SharedPtr<T> & sp)
		:_ptr(sp._ptr)
		, _pCount(sp._pCount)
	{
		
		(*_pCount)++;
	}
	//现代写法
	
	//在这里把a和this进行交换，然后再析构a的时候，这个时候会让原来_ptr的count--。
	//然后_ptr又指向了a，在这里创建了a的零时变量，这个时候就会给_count++。
	
	SharedPtr<T> operator = (SharedPtr<T> sp)
	{
		std::swap(_ptr, sp._ptr);
		std::swap(_pCount, sp._pCount);
	}
	//传统写法
	
	/*SharedPtr<T> operator = (SharedPtr<T>& sp)
	{
		if (this != &sp)
		{
			release();
			_ptr = sp._ptr;
			_pCount = sp._pCount;
			(*_pCount)++;
		}
	}*/
	T* operator ->()
	{
		return _ptr;
	}
	T& operator *()
	{
		return *_ptr;
	}
private:
	void release()
	{
		if (--(*_pCount) == 0)
		{
			delete _ptr;
			delete _pCount;
			_ptr = _pCount = NULL;
		}
	}
private:
	T* _ptr;
	int* _pCount;
};

void test1()
{
	SharedPtr<int>sp1(new int(5));
	SharedPtr<int>sp2 = sp1;;
	SharedPtr<int>sp3(sp2);
}

int main()
{
	test1();
	system("pause");
	return 0;
}