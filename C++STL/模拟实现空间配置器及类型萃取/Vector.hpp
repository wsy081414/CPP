#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include"MyAlloc.hpp"
#include"Construct.hpp"

using namespace std;

template<typename T, typename _Alloc = alloc>
class Vector
{
public:
	typedef T* Iterator;
	typedef const T* ConstIterator;
	typedef T& Ref;
	typedef SimpleAlloc<T, _Alloc> Alloc;
	Vector()
		:_start(0)
		, _finish(0)
		, _endofstorage(0)
		{}
	~Vector()
	{}
	
	size_t Size()
	{
		return End() - Begin();
	}
	size_t Capacity()
	{
		return size_t(_endofstorage - Begin());
	}
	bool Empty()
	{
		return Begin() == End();
	}
	
	Ref operator [](size_t index)
	{
		return *(Begin() + index);
	}
	Ref Front()
	{
		return *Begin();
	}
	Ref Back()
	{
		return *(End() - 1);
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
	ConstIterator End()	const
	{
		return _finish;
	}
	void PushBack(const T& d)
	{
		/*if (finish == _endofstorage)
		{
			InsertAux(End(), d);
		}
		else
		{*/
		CheckStorage();
		*_finish = d;
		++_finish;
		//}
	}

	void InsertAUX(Iterator it,const T& d)
	{
		if (_endofstorage == _finish)
		{
			const size_t oldsize = Size();
			//新的大小要么是原来大小的2倍，要么就是从0加到1.
			const size_t newsize = oldsize != 0？2*oldsize:1;
			//调用空间配置器进行分配内存
			Iterator newstart = Alloc::Allocate(newsize);
			
			Iterator newfinish = newstart;
			
			//接下来进行拷贝内容
			try
			{
				//进行对未初始化的进行拷贝，也就是拷贝之前的到newstart上面，
				//这个就是把[_start,it)的拷贝到[newstart,newstart+(it-_start))区间上
				newfinish = uninitialized_copy(_start, it, newstart);

				//构造新的元素
				Construct(newfinish, x);
				newfinish++;

				//此时进行构造的就是插入点以后的迭代器区间。
				//把[it,_finish)的拷贝到[newfinish,newfinish+(_finish-it))区间上。
				newfinish = uninitialized_copy(it, _finish, newfinish);
			}
			catch (...)
			{
				//如果拷贝过程中发生失败，利用uninitialized机制，必须把以前拷贝的析构掉
				//并且释放掉这一块空间
				Destroy(newstart, newfinish);
				Alloc::Deallocate(newstart,newsize);
				throw;
			}
			//释放原来的vector
			Destroy(Begin(), End());

			//调整vector
			_start = newstart;
			_finish = newfinish;
			_endofstorage = _start + size;
		}
		
	}

	void PopBack()
	{
		--_finish;
		Destroy(_finish);
	}

	Iterator Erase(Iterator pos)
	{
		if (pos + 1 != End())
		{
			//把删除位置之后的拷贝到前面来，[pos+1,end)区间的
			copy(pos + 1, End(), pos);
			--_finish;
			Destroy(_finish);
			retrun pos;
		}
	}

	//清楚迭代器区间。
	Iterator Erase(Iterator first, Iterator last)
	{
		//拷贝[last,_finish)区间的元素到[first,first+(_finish-last));
		Iterator it = copy(last, _finish, first);

		//拷贝返回的是最后一个位置
		Destroy(it,_finish);
		
		_finish -= (last - first);
		return first;
	}

	void Clear()
	{
		Erase(Begin(), End());
	}

	void Insert(Iterator pos,size_t n,const T& x)
	{
		if (n != 0)
		{
			//备用空间大于新增的个数
			if (_endofstorage - _finish >= n)
			{
				T x_copy = x;
				const size_t behindleft = _finish - pos;
				Iterator old_finish = _finish;

				//考虑新增元素小于插入点之后现有的
				if (behindleft > n)
				{
					//把区间[_finish-n,_finsh)移动到_finish之后，然后再这个区间构造n个x。
					uninitialized_copy_n(_finish - n, _finish, _finish);
					_finish += n;
					//从后向前进行拷贝，拷贝区间是[pos,old_finish-n),开始位置是old_finish。
					copy_backward(pos, old_finish - n, old_finish);
					//填充区间[pos,old_finish)用x_copy进行填充。
					fill(pos, old_finish, x_copy);


					//总结下来就是首先把n个finish之前的元素拷贝到old_finish之后，然后把[pos,_finish-n)之间的元素拷贝到old_finish之前
				}
				//考虑新增元素大于等于新增元素
				else
				{
					//在[_finish,_finish+n-behindleft)初始化x_copy
					uninitialized_fill_n(_finish, n - behindleft, x_copy);
					
					//
					_finish += n - behindleft;

					//拷贝[pos,old_finish),到[_finish,_finish+(old_finish-pos)).
					uninitialized_copy(pos, old_finish, _finish);

					//修正_finish
					_finish += behindleft;

					//填充x_copy
					fill(pos, old_finish, x_copy);
				}
			}
			//此时讨论备用空间小于新增元素个数
			else
			{
				size_t oldsize = size();
				size_t newsize = oldsize + max(oldsize, n);
				Iterator newstart = Alloc::Allocate(newsize);
				Iterator newfinish = newstart + newsize;
				try
				{
					//首先拷贝前一段的内容。
					newfinish = uninitialized_copy(_start, pos, newstart);

					//然后进行填充插入新增的n个元素
					newfinish = uninitialized_fill_n(newfinish, n, x);

					//最后把后半段的内容插入进去
					newfinish = uninitialized_copy( pos, _finish, newfinish);

				}
				catch (...)
				{
					Destroy(newstart, newfinish);
					Alloc::DeAllocate(newstart, newsize);
					throw;
				}
				Destroy(_start, _finish);
				Alloc::DeAllocate(_start, _finish);

				_start = newstart;
				_finish = newfinish;
				_endofstorage = _start + newsize;
			}
		} 
	
	}

	void CheckStorage()
	{
		if (_finish >= _endofstorage)
		{
			size_t oldSize = Size();
			size_t len = oldSize != 0 ? 2 * oldSize : 3;
			Iterator newstart = Alloc::Allocate(len);

			for (size_t i = 0; i < oldSize; i++)
			{
				newstart[i] = _start[i];
			}

			//Destory(_start, _finish);
			Alloc::Deallocate(_start, oldSize);

			_start = newstart;
			_finish = _start+oldSize;
			_endofstorage = _start + len;
		}
	}

protected:
	Iterator _start;
	Iterator _finish;
	Iterator _endofstorage;
};