#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>

using namespace std;



struct File
{
	void operator ()(void *ptr)
	{
		fclose((FILE*)ptr);
	}
};

template<typename T>
struct DefaultDel
{
	void operator()(T *ptr)
	{
		delete ptr;
	}
};
template<typename T>
struct DeleteArr
{
	void operator()(T *ptr)
	{
		delete[] ptr;
	}
};


template<typename T>
struct Free
{
	void operator()(T *ptr)
	{
		free(ptr);
	}
};


template<typename T,typename D=DefaultDel<T>>
class SharedPtr
{
public:
	
	SharedPtr(T* ptr)
		:_ptr(ptr)
		, _pCount(new int (1))
		, _del(D())
	{
	
	}
	~SharedPtr()
	{
		release();
	}
	SharedPtr(SharedPtr<T,D> & sp)
		:_ptr(sp._ptr)
		, _pCount(sp._pCount)
	{
		
		(*_pCount)++;
	}
	//现代写法
	SharedPtr<T,D> operator = (SharedPtr<T,D> sp)
	{
		std::swap(_ptr, sp._ptr);
		std::swap(_pCount, sp._pCount);
	}
	//传统写法
	/*SharedPtr<T,D> operator = (SharedPtr<T,D>& sp)
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
			_del(_ptr);
			delete _pCount;
			_ptr = NULL;
			_pCount = NULL;
		}
	}
private:
	T* _ptr;
	int* _pCount;
	D _del;//所给的删除器类型
};

void test1()
{
	SharedPtr<FILE, File>	sp(fopen("test.txt", "w"));
	SharedPtr<int > sp2(new int(1));
	SharedPtr<int > sp3((int *)malloc(sizeof(5)));
}

int main()
{
	test1();
	system("pause");
	return 0;
}