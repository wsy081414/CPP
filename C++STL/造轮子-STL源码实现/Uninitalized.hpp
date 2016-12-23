
#pragma once
#include"typetraits.hpp"
#include"Construct.hpp"
#include"Iterator.hpp"
#include<algorithm>

template<typename InputIterator, typename ForwardIterator>
void UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result)
{
	__UninitializedCopy(first, last, result, *(&result));

}

template<typename InputIterator, typename ForwardIterator,typename T>
void __UninitializedCopy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	__UninitializedCopyAux(first, last, result, TypeTraits<T>::__IsPODType());

}
template<typename InputIterator, typename ForwardIterator>
void __UninitializedCopyAux(InputIterator first, InputIterator last, ForwardIterator result, __FalseType)
{
	while (first!=last)
	{
		Construct(&(*result), *first);
		++first;
		++result;
	}
}

template<typename InputIterator, typename ForwardIterator>
void __UninitializedCopyAux(InputIterator first, InputIterator last, ForwardIterator result, __TrueType)
{
	copy(first, last, result);

}
