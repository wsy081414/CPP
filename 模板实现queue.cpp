#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>
#include<cassert>

using namespace std;
template <typename T>
struct QueueNode
{
public:
	QueueNode()
		:_next(NULL)
		, data(0)
	{}

	QueueNode *_next;
	T data;
};
template<typename T>
class Queue
{
public:
	Queue()
		:_head(NULL)
		, _tail(NULL)
	{}
	~Queue()
	{
		QueueNode<T>* cur = _head;
		while (cur != NULL)
		{
			QueueNode<T>* del=cur;
			cur = cur->_next;
			delete del;
			del = NULL;
		}
	}
	bool empty()
	{
		if (_head == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	size_t size()
	{
		size_t count(0);
		QueueNode<T> *cur = _head;
		while (cur != NULL)
		{
			count++;
			cur = cur->_next;
		}
		return count;
	}
	T& front()
	{
		assert(_head != NULL); 
		return _head->data;
	}
	T& back()
	{	
		assert(_tail != NULL);
		return _tail->data;
	}
	void push(const T& d)
	{
		if (_head == NULL)
		{
			_head = new QueueNode<T>;
			_head->data = d;
			_tail = _head;
		}
		else
		{
			QueueNode<T> *tmp = new QueueNode <T>;
			tmp->data = d;
			tmp->_next = NULL;
			_tail->_next = tmp;
			_tail = _tail->_next;
		}
	}
	void pop()
	{
		assert(_head != NULL);
		QueueNode<T> *del = _head;
		_head = _head->_next;
		delete del;
		del = NULL;

	}
protected:
	QueueNode<T>* _head;
	QueueNode<T>* _tail;
};