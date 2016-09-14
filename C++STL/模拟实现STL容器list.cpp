#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>
#include<list>
using namespace std;

template<class T>
struct __ListNode
{
	__ListNode(const T& x)
	:_next(0)
	, _prev(0)
	, _data(x)
	{

	}
	__ListNode* _next;
	__ListNode* _prev;
	T _data;

};

template<class T, class Ref, class Ptr>
struct __List_Iterator
{
	typedef __List_Iterator<T, T&, T*> Iterator;
	typedef __List_Iterator<T, Ref, Ptr> Self;
	typedef __ListNode<T>* ListNode;
	typedef Ref Reference;


	__List_Iterator(ListNode  x)
		:_node(x)
	{
	}
	__List_Iterator()
	{
	}
	__List_Iterator(const Iterator& x)
		:_node(x._node)
	{
	}
	bool operator ==(const Iterator& x) const
	{
		return _node == x._node;
	}
	bool operator!=(const Iterator& x) const
	{
		return _node != x._node;
	}
	Self& operator ++()
	{
		_node = _node->_next;
		return *this;
	}
	Self operator ++(int)
	{
		ListNode tmp = _node;
		_node = _node->_next;
		return tmp;
	}
	Self& operator--()
	{
		_node = _node->_prev;
		return *this;
	}
	Self operator--(int)
	{
		ListNode tmp(_node);
		_node = _node->_prev;
		return tmp;
	}
	Ref operator*() const
	{
		return _node->_data;
	}
	Ptr operator->() const
	{

		return &((*node));
	}

	ListNode _node;
};
template<class T>
class List
{
protected:
	typedef  __ListNode<T>* Link_Type;
	typedef  __ListNode<T> Node_Type;
public:
	typedef __List_Iterator<T, T&, T*>  Iterator;
	typedef __List_Iterator<const T, const T&, const T*> Const_Iterator;
	typedef size_t Size_Type;
	typedef T& Reference;
public:
	Link_Type GetNode(const T& x)
	{
		Link_Type node = new Node_Type(x);

		return node;
	}

	List()
	{
		_node = GetNode(T());//在这里给一个匿名的对象，这样就可以创建一个匿名的节点。
		_node->_next = _node;
		_node->_prev = _node;
	}
	Iterator Begin()
	{
		return (Iterator)(_node->_next);
	}
	Iterator End()
	{
		return (Iterator)(_node);
	}
public:
	bool Empty() const
	{
		return _node->_next == _node;
	}
	Size_Type size() const
	{
		;
	}
	Size_Type Max_Size()
	{
		return (Size_Type)(-1);
	}
	Reference Front()
	{
		return *Begin();
	}
	Reference Back()
	{
		return *(End()--);
	}
	void Insert(Iterator Position, const T& x)
	{
		Link_Type tmp = GetNode(x);
		Link_Type cur = Position._node;
		Link_Type prev = cur->_prev;
		cur->_prev = tmp;
		tmp->_next = cur;

		prev->_next = tmp;
		tmp->_prev = prev;
	}
	void Push_Front(const T& x)
	{
		Insert(Begin(), x);
	}
	void Push_Back(const T& x)
	{
		Insert(End(), x);
	}
	void Erase(Iterator Position)
	{
		Link_Type Prev = Position._node->_prev;
		Link_Type Next = Position._node->_next;

		Prev->_next = Next;
		Next->_prev = Prev;

		delete Position._node;
	}
	void Pop_Back()
	{
		Iterator tmp = End();
		tmp--;
		Erase(tmp);
	}
	void Pop_Front()
	{
		Erase(Begin());
	}


	void resize(Size_Type n, const T& x)
	{
		;
	}
protected:
	Link_Type _node;//哨兵卫

};
void test1()
{
	List<int > l;
	//cout << l.Empty() << endl;
	l.Push_Back(1);
	l.Push_Back(2);
	l.Push_Back(3);
	l.Push_Back(4);

	List<int >::Iterator end = l.End();
	List<int >::Iterator begin = l.Begin();

	
	while (begin != end)
	{
		cout << *begin << endl;
		begin++;
	}
	
	
	cout << l.Empty() << endl;



}

int main()
{
	test1();


	system("pause");
	return 0;
}