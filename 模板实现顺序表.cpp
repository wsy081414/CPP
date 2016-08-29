#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<string>
#include<cassert>

using namespace std;

template<typename T>
class SeqList
{
public:
	SeqList();
	~SeqList();
	SeqList(const SeqList& s);
	SeqList<T>& operator=(const SeqList<T> s);
	T& operator[](const int& n);

	void PushBack(const T& data);
	void Display();
	void PushFront(const T& data);
	void PopBack();
	void PopFront();
	void Insert(int pos,const T& data);

	void Remove(const T& data);
	void RemoveAll(const T& data);
	void Sort();
	void reserve(int n);
	void erase(int pos);
	void Size();
	void Capacity();
private:
	void CheckCapacity();
	void CheckCapacity(int n);
private:
	T* _data;
	int _sz;
	int _capacity;
};



template<typename T>
SeqList<T>::SeqList()
:_data(NULL)
, _sz(0)
, _capacity(3)
{
	_data = new T[3];
}

template<typename T>
SeqList<T>::~SeqList()
{
	if (_data != NULL)
	{
		delete[] _data;
		_data = NULL;
		_sz = 0;
		_capacity = 0;
	}
}

template<typename T>
void SeqList<T>::PushBack(const T& data)
{
	CheckCapacity();
	_data[_sz] = data;
	_sz++;
}

template<typename T>
void SeqList<T>::Display()
{
	for (int i = 0; i < _sz; i++)
	{
		cout << _data[i] << " ";
	}
	cout << endl;
}


template<typename T>
void SeqList<T>::CheckCapacity()
{
	if (_sz == _capacity)
	{
		int NewCapacity = 2 * _capacity;
		T* Tmp = new T[NewCapacity];
		//在这里不使用memcpy()函数，主要是因为对于非内置类型memcpy()函数会出现浅拷贝问题，比如string类型，
		//这个类型中有一个_buf与_ptr,当储存少于16个的时候这时会储存在_buf当中的，如果多于16个，那个会单独开辟空间
		//进行储存,这时拷贝的时候就是拷贝过去这个储存的地址而已，所以这样调用析构函数的时候，当增加容量的时候，
		//这个时候会把储存string的那块空间进行释放，这个时候最后得到的结果就是你新拷贝的所指向的内容变为了释放以后的内容

		for (int i = 0; i < _sz; i++)
		{
			Tmp[i] = _data[i];
		}
	
		delete[] _data;
		_data = Tmp;
		_capacity = NewCapacity;
	}
}

template<typename T>
void SeqList<T>::CheckCapacity(int n)
{
	int NewCapacity = n;
	T* Tmp = new T[n];
	//在这里不使用memcpy()函数，主要是因为对于非内置类型memcpy()函数会出现浅拷贝问题，比如string类型，
	//这个类型中有一个_buf与_ptr,当储存少于16个的时候这时会储存在_buf当中的，如果多于16个，那个会单独开辟空间
	//进行储存,这时拷贝的时候就是拷贝过去这个储存的地址而已，所以这样调用析构函数的时候，当增加容量的时候，
	//这个时候会把储存string的那块空间进行释放，这个时候最后得到的结果就是你新拷贝的所指向的内容变为了释放以后的内容

	for (int i = 0; i < _sz; i++)
	{
		Tmp[i] = _data[i];
	}

	delete[] _data;
	_data = Tmp;
	_capacity = n;
}

template<typename T>
void SeqList<T>::PushFront(const T& data)
{
	CheckCapacity();
	for (int i = _sz; i >0; i--)
	{
		_data[i] = _data[i-1];
	}
	_data[0] = data;
	_sz++;
}

template<typename T>
void SeqList<T>::PopBack()
{
	_sz--;
}

template<typename T>
void SeqList<T>::PopFront()
{
	for (int i = 0; i < _sz; i++)
	{
		_data[i] = _data[i + 1];
	}
	_sz--;
}

template<typename T>
void SeqList<T>::Insert(int pos, const T& data)
{
	assert(pos >= 0 && pos < _sz);
	for (int i = _sz; i >=pos-1; i--)
	{
		_data[i + 1] = _data[i];

	}
	_data[pos-1] = data;
	_sz++;
}

template<typename T>
void SeqList<T>::Size()
{
	cout << _sz << endl;;
}

template<typename T>
void SeqList<T>::Capacity()
{
	cout << _capacity << endl;
}


template<typename T>
void SeqList<T>::Remove(const T& data)
{
	if (_sz == 0)
	{
		return;
	}
	for (int i = 0; i < _sz; i++)
	{
		if (_data[i] == data)
		{
			for (int j = i; j < _sz; j++)
			{
				_data[j] = _data[j + 1];

			}
			break;
		}
	}
	_sz--;
}
template<typename T>
void SeqList<T>::RemoveAll(const T& data)
{
	if (_sz == 0)
	{
		return;
	}
	for (int i = 0; i < _sz; i++)
	{
		if (_data[i] == data)
		{
			for (int j = i; j < _sz; j++)
			{
				_data[j] = _data[j + 1];

			}
			_sz--;
		}
		if (_data[i] == data)
		{
			i--;
		}
	}

}
template<typename T>
void SeqList<T>::Sort()
{
	int flag = 0;
	for (int i = 0; i < _sz; i++)
	{
		int flag = 1;
		for (int j = 0; j < _sz - i - 1; j++)
		{
			if (_data[j]>_data[j + 1])
			{
				std::swap(_data[j], _data[j + 1]);
				flag = 0;
			}
		}
		if (flag)
		{
			break;
		}
	}
}
template<typename T>
void SeqList<T>::reserve(int n)
{
	if (_capacity < n)
	{
		CheckCapacity(n);
	}

}
template<typename T>
void SeqList<T>::erase(int pos)
{
	assert(pos >= 0 && pos < _sz);
	for (int i = pos; i < _sz; i++)
	{
		_data[i - 1] = _data[i];
	}
	_sz--;
}


template<typename T>
SeqList<T>& SeqList<T>::operator=( SeqList<T> s)
{
	if (_data != s._data)
	{
		std::swap(_data, s._data);
		std::swap(_capacity, s._capacity);
		std::swap(_sz, s._sz);
	}
	return *this;
}

template<typename T>
T& SeqList<T>::operator[](const int& n)
{
	assert(n >= 0 && n < _sz);
	return _data[n];
}

template<typename T>
SeqList<T>::SeqList(const SeqList& s)
:_sz(s._sz)
, _capacity(s._capacity)
, _data(new T[s._capacity])
{
	for (int i = 0; i < _sz; i++)
	{
		_data[i] = s._data[i];
	}
	
}


