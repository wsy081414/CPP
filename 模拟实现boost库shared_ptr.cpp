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
	//�ִ�д��
	
	//�������a��this���н�����Ȼ��������a��ʱ�����ʱ�����ԭ��_ptr��count--��
	//Ȼ��_ptr��ָ����a�������ﴴ����a����ʱ���������ʱ��ͻ��_count++��
	
	SharedPtr<T> operator = (SharedPtr<T> sp)
	{
		std::swap(_ptr, sp._ptr);
		std::swap(_pCount, sp._pCount);
	}
	//��ͳд��
	
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