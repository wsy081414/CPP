#pragma once


#include<iostream>
#include<cstdlib>
#include<cassert>
#include"Typeextraction.h"
template<typename T>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
public:	Vector()
		:_start(NULL)
		, _finish(NULL)
		, _endofstroage(NULL)
	{
	}
	~Vector()
	{
		if (_start)
		{
			delete[] _start;
		}
	}
	size_t Size()
	{
		return _finish - _start;
	}
	void Erase(Iterator pos)
	{
		assert(pos<_finish&&pos>_start);
		for (Iterator i = _finish-1; i != pos; i--)
		{
			*(i-1) = *i;
		}
		_finish--;
	}
	void Insert(Iterator pos, const T& d)
	{
		CheckStorage();
		for (Iterator i = pos; i != _finish; i++)
		{
			*(i+1)=*i;
		}
			
		*pos = d;
		_finish++;
	}
	void PushBack(const T& d)
	{
		CheckStorage();
		Insert(End(),d);
	}
	void Popback()
	{
		assert(_finish != NULL);
		Erase(End()-1);
	}
	void CheckStorage()
	{
		if (_finish == _endofstroage)
		{
			size_t newsize = Size() * 2 + 3;
			Iterator Tmp = new T[newsize];
			if (typetraits<T>::__ispodtype().get())
			{
				memcpy(Tmp, _start, sizeof(T)*Size());
			}
			else
			{
				for (size_t i = 0; i < Size(); i++)
				{
					Tmp[i] = _start[i];
				}
			}
			_finish = Tmp + Size();
			delete[] _start;
			_start = Tmp;
			_endofstroage = _start + newsize;
		}
	}

	Iterator Begin()
	{
		return _start;
	}
	Iterator End()
	{
		return _finish;
	}
	ConstIterator Begin() const
	{
		return _start;
	}
	ConstIterator End() const
	{
		return _finish;
	}
		
protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endofstroage;
};

template<typename T>
void PrintVector(const Vector<T>& v)
{
	Vector<T>::ConstIterator it = v.Begin();
	while (it != v.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;
}