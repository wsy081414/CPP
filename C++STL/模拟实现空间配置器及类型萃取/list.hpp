#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include"MyAlloc.hpp"
#include"Construct.hpp"
#include"Vector.hpp"
#include"Iterator.hpp"
using namespace std;

template<typename T>
struct ListNode
{
	T _data;
	ListNode* _next;
	ListNode* _prev;
	ListNode(const T& d)
		:_data(d)
		, _next(NULL)
		, _prev(NULL)
	{}
};


template<typename T,typename Ref,typename Ptr>
struct ListIterator
{
	typedef ListNode<T> Node;
	typedef ListIterator<T, const T&, const T*> ConstIterator;

	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptr Pointer;
	typedef Ref Reference;
	typedef int DifferenceType;

	typedef ListIterator<T, Ref, Ptr> Self;
	ListIterator(Node* node)
		:_node(node)
	{
	}

	Ref operator *()
	{
		return _node->_data;
	}

	Ptr operator ->()
	{
		return &(_node->_data);
	}

	Self& operator ++()
	{
		_node = _node->_next;
		return *this;
	}
	
	Self operator ++(int)
	{
		Node* tmp = _node;
		_node = _node->_next;
		return tmp;

	}
	Self& operator --()
	{
		_node = _node->_prev;
		return *this;
	}

	Self operator --(int)
	{
		Node* tmp = _node;
		_node = _node->_prev;
		return tmp;

	}
	bool operator !=(Self & s)
	{
		return _node != s._node;
	}


	Node* _node;
};
template<typename T,typename _ALLoc = alloc>
class List
{
public:
	typedef ListNode<T> Node;
	typedef SimpleAlloc<Node, _ALLoc> Alloc;

	typedef ListIterator<T, T&, T*> Iterator;
	typedef ListIterator<T, const T&, const T*> ConstIterator;

	

	Node* BuyNode(const T& x)
	{
		Node *p = Alloc::Allocate();
		Construct(p,x);
		return p;
	}

	void DestoryNode( Node* p)
	{
		Destroy(p);
		Alloc::Deallocate(p);
	}

	List()
		:_head(BuyNode(T()))
	{
		_head->_next = _head;
		_head->_prev = _head;
	}

	~List()
	{
		Clear();
		DestoryNode(End()._node);
	}
	void Clear()
	{
		Iterator it = Begin();
		while (it!=End())
		{
			Node* del = it._node;
			++it;
			DestoryNode(del);
		}
	}

	ConstIterator Begin() const
	{
		return _head->_next;
	}

	Iterator Begin()
	{
		return _head->_next;
	}

	Iterator End()
	{
		return _head;
	}

	ConstIterator End() const
	{
		return _head;
	}

	//ÏòÇ°²åÈë
	void Insert(Iterator& it, const T& x)
	{
		Node* cur = it._node;
		Node* prev = cur->_prev;

		Node* tmp = BuyNode(x);

		prev->_next = tmp;
		tmp->_prev = prev;

		tmp->_next = cur;
		cur->_prev = tmp;
	}

	void PushBack(const T& x)
	{
		return Insert(Begin(), x);
	}

	void PopBack()
	{
		Node* end = End()._node;
		Node* cur = end->_prev;
		Node* prev = cur->_prev;
		prev->_next = end;
		end->_prev = prev;
		DestoryNode(cur);
	}
	void PushFront(const T& x)
	{
		Node* tmp = BuyNode(x);
		Node* prev = End()._node;
		Node* next = Begin()._node;
		
		prev->_next = tmp;
		tmp->_prev = prev;

		tmp->_next = next;
		next->_prev = prev;
	}
	void PopFront()
	{
		Node* prev = End()._node;
		Node* Cur = Begin()._node;
		Node* next = Cur->_next;

		prev->_next = next;
		next->_prev = prev;

		DestoryNode(Cur);
	}
protected:
	Node* _head;
};


void PrintList(const List<int> & l)
{
	auto begin = l.Begin();
	while (begin != l.End())
	{
		cout << *begin << endl;
		++begin;
	}
	cout << endl;
}

