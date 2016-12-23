#define _SCL_SECURE_NO_WARNINGS
#pragma once
#include"Iterator.hpp"
#include"typetraits.hpp"


template<typename T1,typename T2>
inline void Construct(T1* p,const T2& value)
{
	new(p)T1(value);
}


template<typename T>
inline void Destroy(T* p)
{
	p->~T();
}

template<typename ForwardIterator>
inline void Destory(ForwardIterator first, ForwardIterator last)
{
	__Destory(first, last, &(*first));
}

template<typename ForwardIterator,class T>
inline void __Destory(ForwardIterator first, ForwardIterator last, T*)
{
	__DestoryAux(first, last, TypeTraits<T>::HasTrivialDestructor());
}

template<typename ForwardIterator>
inline void __DestoryAux(ForwardIterator first, ForwardIterator last, __TrueType)
{
}

template<typename ForwardIterator>
inline void __DestoryAux(ForwardIterator first, ForwardIterator last, __FalseType)
{
	while (first < last)
	{
		Destory(&(*first));
		first++;
	}
}

inline void Destory(char * ,char *)
{}
inline void Destory(wchar_t *, wchar_t *)
{}


