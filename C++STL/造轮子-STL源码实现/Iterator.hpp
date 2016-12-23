#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<cstdlib>

using namespace std;

struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};


template <class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct iterator {
	typedef Category  IteratorCategory;
	typedef T         ValueType;
	typedef Distance  DifferenceType;
	typedef Pointer   Pointer;
	typedef Reference Reference;
};
template<typename T>
struct IteratorTraits
{
	typedef typename T::IteratorCategory IteratorCategory;
	typedef typename T::ValueType ValueType;
	typedef typename T::DifferenceType DifferenceType;
	typedef typename T::Pointer Pointer;
	typedef typename T::Reference Reference;
};

template<typename T>
struct IteratorTraits<T*>
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef int DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;
};

template<typename T>
struct IteratorTraits<const T*>
{
	typedef typename RandomAccessIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef int DifferenceType;
	typedef const T* Pointer;
	typedef const T& Reference;
};


template<typename InputIterator>
inline void Advance(InputIterator & x, size_t n)
{
	_advance(x, n, IteratorTraits<InputIterator>::IteratorCategory());
}

//配置单向迭代器
template<typename InputIterator>
inline void _advance(InputIterator &i, size_t n, InputIteratorTag)
{
	while (n--) ++i;
}
//配置双向迭代器
template<typename InputIterator>
inline void _advance(InputIterator &i, size_t n, BidirectionalIteratorTag)
{
	if (n >= 0)
	while (n--) ++i;
	else
	while (n++) --i;
}
//配置随机迭代器
template<typename InputIterator>
inline void _advance(InputIterator &i, size_t n, RandomAccessIteratorTag)
{
	i += n;
}

template<typename InputIterator>
inline size_t Distance(InputIterator first, InputIterator second)
{

	return _distance(first, second, IteratorTraits<InputIterator>::IteratorCategory());
}

template<typename InputIterator>
inline size_t _distance(InputIterator first, InputIterator second, RandomAccessIteratorTag)
{
	return second - first;
}
template<typename InputIterator>
inline size_t _distance(InputIterator first, InputIterator second, InputIteratorTag)
{
	size_t n = 0;
	while (first!=second)
	{
		++first;
		++n;
	}
	return n;
}




//反向迭代器
template<typename Iterator>
class ReverseIterator
{
public:
	typedef typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory;
	typedef typename IteratorTraits<Iterator>::ValueType ValueType;
	typedef typename IteratorTraits<Iterator>::DifferenceType DifferenceType;
	typedef typename IteratorTraits<Iterator>::Pointer Pointer;
	typedef typename IteratorTraits<Iterator>::Reference Reference;

	typedef ReverseIterator<Iterator> Self;

	//反向迭代器不允许隐式转换成正向迭代器
	explicit ReverseIterator(Iterator cur)
		:_current(cur)
	{
	}
	
	//调用反向迭代器前一个迭代器的正向迭代器的operator*
	Reference operator *()
	{
		Iterator tmp = _current;
		return *(--tmp);
	}

	Pointer operator ->()
	{
		return &(*operator*());
	}

	Self& operator ++()
	{
		--_current;
		return *this;
	}

	Self operator ++(int)
	{
		Self tmp = *this;
		--_current;
		return tmp;
	}

	Self& operator --()
	{
		++_current;
		return *this;
	}

	Self operator --(int)
	{
		Self tmp = *this;
		++_current;
		return tmp;
	}

	bool operator ==(const Self& r)
	{
		return r._current == _current;
	}

	bool operator !=(const Self& r)
	{
		return r._current != _current;
	}
protected:
	Iterator _current;
};