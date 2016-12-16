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
			//�µĴ�СҪô��ԭ����С��2����Ҫô���Ǵ�0�ӵ�1.
			const size_t newsize = oldsize != 0��2*oldsize:1;
			//���ÿռ����������з����ڴ�
			Iterator newstart = Alloc::Allocate(newsize);
			
			Iterator newfinish = newstart;
			
			//���������п�������
			try
			{
				//���ж�δ��ʼ���Ľ��п�����Ҳ���ǿ���֮ǰ�ĵ�newstart���棬
				//������ǰ�[_start,it)�Ŀ�����[newstart,newstart+(it-_start))������
				newfinish = uninitialized_copy(_start, it, newstart);

				//�����µ�Ԫ��
				Construct(newfinish, x);
				newfinish++;

				//��ʱ���й���ľ��ǲ�����Ժ�ĵ��������䡣
				//��[it,_finish)�Ŀ�����[newfinish,newfinish+(_finish-it))�����ϡ�
				newfinish = uninitialized_copy(it, _finish, newfinish);
			}
			catch (...)
			{
				//������������з���ʧ�ܣ�����uninitialized���ƣ��������ǰ������������
				//�����ͷŵ���һ��ռ�
				Destroy(newstart, newfinish);
				Alloc::Deallocate(newstart,newsize);
				throw;
			}
			//�ͷ�ԭ����vector
			Destroy(Begin(), End());

			//����vector
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
			//��ɾ��λ��֮��Ŀ�����ǰ������[pos+1,end)�����
			copy(pos + 1, End(), pos);
			--_finish;
			Destroy(_finish);
			retrun pos;
		}
	}

	//������������䡣
	Iterator Erase(Iterator first, Iterator last)
	{
		//����[last,_finish)�����Ԫ�ص�[first,first+(_finish-last));
		Iterator it = copy(last, _finish, first);

		//�������ص������һ��λ��
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
			//���ÿռ���������ĸ���
			if (_endofstorage - _finish >= n)
			{
				T x_copy = x;
				const size_t behindleft = _finish - pos;
				Iterator old_finish = _finish;

				//��������Ԫ��С�ڲ����֮�����е�
				if (behindleft > n)
				{
					//������[_finish-n,_finsh)�ƶ���_finish֮��Ȼ����������乹��n��x��
					uninitialized_copy_n(_finish - n, _finish, _finish);
					_finish += n;
					//�Ӻ���ǰ���п���������������[pos,old_finish-n),��ʼλ����old_finish��
					copy_backward(pos, old_finish - n, old_finish);
					//�������[pos,old_finish)��x_copy������䡣
					fill(pos, old_finish, x_copy);


					//�ܽ������������Ȱ�n��finish֮ǰ��Ԫ�ؿ�����old_finish֮��Ȼ���[pos,_finish-n)֮���Ԫ�ؿ�����old_finish֮ǰ
				}
				//��������Ԫ�ش��ڵ�������Ԫ��
				else
				{
					//��[_finish,_finish+n-behindleft)��ʼ��x_copy
					uninitialized_fill_n(_finish, n - behindleft, x_copy);
					
					//
					_finish += n - behindleft;

					//����[pos,old_finish),��[_finish,_finish+(old_finish-pos)).
					uninitialized_copy(pos, old_finish, _finish);

					//����_finish
					_finish += behindleft;

					//���x_copy
					fill(pos, old_finish, x_copy);
				}
			}
			//��ʱ���۱��ÿռ�С������Ԫ�ظ���
			else
			{
				size_t oldsize = size();
				size_t newsize = oldsize + max(oldsize, n);
				Iterator newstart = Alloc::Allocate(newsize);
				Iterator newfinish = newstart + newsize;
				try
				{
					//���ȿ���ǰһ�ε����ݡ�
					newfinish = uninitialized_copy(_start, pos, newstart);

					//Ȼ�����������������n��Ԫ��
					newfinish = uninitialized_fill_n(newfinish, n, x);

					//���Ѻ��ε����ݲ����ȥ
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