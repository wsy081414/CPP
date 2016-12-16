#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>

using namespace std;

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


