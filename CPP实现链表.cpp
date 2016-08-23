#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>
#include<cassert>

using namespace std;

typedef int DataType;

struct Node
{
	Node(const DataType& d)
	:_next(NULL)
	, _data(d)
	{
	}
	Node *_next;
	DataType _data;
};
class LinkList
{
	friend ostream& operator<<(ostream &os, LinkList &list);
public:
	LinkList()
		:_head(NULL)
		, _tail(NULL)
	{
	}
	~LinkList()
	{
		Node *cur = _head;
		while (cur != NULL)
		{
			Node *del = cur;
			cur = cur->_next;
			delete del;
		}
	}
	LinkList(const LinkList & list)
		:_head(NULL)
		, _tail(NULL)
	{
		Node *cur = list._head;
		while (cur != NULL)
		{
			push_back(cur->_data);
			cur = cur->_next;
		}

	}
	LinkList& operator=(const LinkList list)
	{
		std::swap((Node *)list._head, (Node *)_head);
		std::swap((Node *)list._tail, (Node *)_tail);
		return *this;
	}
	void push_back(const DataType& data)
	{
		if (_head == NULL)
		{
			_head = new Node(data);
			_tail = _head;
		}
		else
		{
			Node *tmp = new Node(data);
			_tail->_next = tmp;
			_tail = tmp;
		}

	}
	void push_front(const DataType& data)
	{
		Node *tmp = new Node(data);
		Node *cur = _head;
		_head = tmp;
		tmp->_next = cur;
	}
	void pop_back()
	{
		Node *cur = _head;
		Node *del = NULL;
		if (_head == NULL)
		{
			return;
		}
		else if (_head == _tail)
		{
			del = _head;
			delete del;
			_head = _tail = NULL;
		}
		else
		{
			while (cur->_next != _tail)
			{
				cur = cur->_next;
			}
			delete _tail;
			_tail = cur;
			_tail->_next = NULL;

		}
	}
	void pop_front()
	{
		if (_head == NULL)
		{
			return;
		}
		else
		{
			Node* cur = _head;
			Node* del = NULL;
			del = _head;
			_head = _head->_next;
			delete del;
			del = NULL;
		}
	}
	Node* find(DataType data)
	{
		Node* cur = _head;
		while (cur)
		{
			if (cur->_data == data)
			{
				return cur;
			}
			cur = cur->_next;
		}
		return NULL;
	}
	void insert(Node* pos, DataType data)
	{
		if (pos == NULL)
		{
			return;
		}
		Node* newnode = new Node(data);
		Node* cur = _head;
		Node* prev = NULL;
		while (cur != NULL)
		{
			if (pos == cur)
			{
				break;
			}
			prev = cur;
			cur = cur->_next;
		}
		if (prev == NULL)
		{
			_head->_next = newnode;
			newnode->_next = cur;
		}
		else
		{
			prev->_next = newnode;
			newnode->_next = cur;
		}
	}

	void erase(Node *pos)
	{
		if (_head == NULL)
		{
			return ;
		}
		Node *cur = _head;
		Node *del = NULL;
		Node *prev = _head;
		while(cur != NULL)
		{
			if (pos == cur)
			{
				if (pos == _head)
				{
					del = pos;
					_head = cur->_next;
					delete del;
				
					del = NULL;
				}
				else
				{
					del = pos;
					prev->_next = cur->_next;
					delete del;
					del = NULL;
				}
				return;
			}
			else
			{
				prev = cur;
				cur = cur->_next;
			}


		}
	}
	void sort()
	{
		if ((_head == NULL) || (_head->_next == NULL))
		{
			return;
		}
		Node* cur = _head;
		Node* tail = NULL;
		while (cur != tail)
		{
			while (cur->_next != tail)
			{
				if (cur->_data > cur->_next->_data)
				{
					std::swap(cur->_data, cur->_next->_data);
				}
				cur=cur->_next;
			}
			tail = cur;
			cur = _head;
		}
	}
	void remove(const DataType& x)
	{
		if (_head == NULL)
		{
			return;
		}
		Node *cur = _head;
		Node *prev = NULL;
		Node *del = NULL;
		while (cur != NULL)
		{
			if (cur->_data == x)
			{
				if (cur == _head)
				{
					del = cur;
					_head = cur->_next;
					delete del;
					del = NULL;
				}
				else
				{
					del = cur;
					prev->_next = cur->_next;
					delete del;
					del = NULL;
				}
				return;
			}
			else
			{
				prev = cur;
				cur = cur->_next;
			}
		}
	}
	void removeall(const DataType& data)
	{
		if (_head == NULL)
		{
			return;
		}
		Node *cur = _head;
		Node *del = NULL;
		Node *prev = cur;
		while(cur!=NULL)
		{
			if (cur->_data == data)
			{
				if (cur == _head)
				{
					del = cur;
					prev = cur->_next;
					_head = cur->_next;
					delete del;
					del = NULL;
				}
				else
				{
					del = cur;
					prev->_next = cur->_next;
					delete del;
					del = NULL;
				}
				cur = prev;
			}
			else
			{
				prev = cur;
				cur = cur->_next;
			}
		}
	}

	int getsize()
	{
		Node*cur = _head;
		int count = 0;
		while (cur != NULL)
		{
			count++;
			cur = cur->_next;
		}
		return count;
	}
private:
	Node *_head;
	Node *_tail;
};


ostream& operator<<(ostream &os, LinkList &list)
{
	Node *cur = list._head;
	while (cur)
	{
		os << cur->_data << " ";
		cur = cur->_next;
	}
	return os;
}

void test1()
{
	LinkList mylist;
	mylist.push_back(1);
	mylist.push_back(2);
	mylist.push_back(3);
	LinkList mylist2(mylist);
	LinkList mylist3;
	mylist3 = mylist2;
	cout << mylist2 << endl;
	cout << mylist << endl;
	cout << mylist3 << endl;


}
void test2()
{
	LinkList mylist;
	mylist.push_back(1);
	mylist.push_back(34);
	mylist.push_back(394);
	mylist.push_back(304);
	mylist.push_back(34);
	mylist.push_back(3094);
	mylist.insert(mylist.find(394), 78);

	mylist.erase(mylist.find(3094));
	cout<<mylist.getsize() << endl;
	cout << mylist << endl;
}

void test3()
{
	LinkList mylist;
	mylist.push_back(3094);
	mylist.push_back(1);
	mylist.push_back(34);
	mylist.push_back(394);
	mylist.push_back(304);
	mylist.push_back(34);

	
	mylist.push_back(34);
	mylist.removeall(34);
	mylist.sort();
	mylist.erase(mylist.find(1));
	cout << mylist << endl;
	cout << mylist.getsize() << endl;

}
int main()
{
	test3();
	system("pause");
	return 0;
}