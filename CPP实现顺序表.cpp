#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdlib>
#include<cassert>

using namespace std;


typedef int DataType;
class SeqList
{
	friend ostream& operator <<(ostream &os, SeqList &s);
public:
	SeqList()
		:_sz(0)
		, _capacity(0)
	
	{
		checkcapacity();
	}
	~SeqList()
	{
		delete[] _data;
		_sz = 0;
		_capacity = 0;
	}
	void push_back(DataType tmp)
	{
		checkcapacity();
		_data[_sz] = tmp;
		_sz++;
	}
	void push_front(DataType tmp)
	{
		checkcapacity();
		memmove(_data+1,_data,_sz*sizeof(DataType));
		_data[0] = tmp;
		_sz++;
	}

	void pop_front()
	{
		memmove(_data, _data + 1, (_sz - 1)*sizeof(DataType));
		_sz--;
	}
	void pop_back()
	{
		_sz--;
	}
	int find(const DataType& s)
	{
		for (int i = 0; i < _sz; i++)
		{
			if (s == _data[i])
				return i;
		}
		return -1;
	}
	void insert(int pos,const DataType& d)
	{
		assert(pos>0 && pos <= _sz);
		checkcapacity();
		memmove(_data + pos , _data + pos-1, (_sz - pos+1)*sizeof(DataType));
		_data[pos-1] = d;
		_sz++;
	
	}
	void remove(const DataType & x)
	{
		if (_sz == 0)
		{
			return;
		}
		for (int i = 0; i < _sz; i++)
		{
			if (_data[i] == x)
			{
				memmove(_data + i, _data + i +1, _sz - i+1*sizeof(DataType));
				_sz--;
				return;
			}
		}
	}
	void removeall(int x)
	{

		if (_sz == 0)
		{
			return;
		}
		for (int i = 0; i < _sz; i++)
		{
			if (_data[i] == x)
			{
				memmove(_data + i, _data + i + 1, _sz - i + 1 * sizeof(DataType));
				_sz--;
				i--;
				
			}
		}
	}
	void sort()
	{
		int flag = 0;
		for (int i = 0; i < _sz; i++)
		{
			flag = 1;
			for (int j = 0; j < _sz - i - 1; j++)
			{
				if (_data[j]>_data[j + 1])
				{
					std::swap(_data[j], _data[j + 1]);
				}
			}
			if (flag)
			{
				break;
			}
		}
	}
	int seq_size()
	{
		return _sz;
	}

	int seq_capacity()
	{
		return _capacity;
	}
private:
	void checkcapacity()
	{
		if (_sz == _capacity)
		{
			DataType* tmp = new DataType[2 * _capacity+4];
			memcpy(tmp, _data, _sz*sizeof(DataType));
			delete[] _data;
			_data = tmp;
			_capacity += (2 * _capacity+4);
		}
	
	}

private:
	int _sz;
	int _capacity;
	DataType *_data;
};

ostream& operator <<(ostream &os, SeqList &s)
{
	for (int i = 0; i < s._sz; i++)
	{
		os << s._data[i] << " ";
	
	}
	return os;
}
void test()
{
	SeqList mylist;
	mylist.push_back(5);
	mylist.push_back(8);
	mylist.push_back(9);
	mylist.push_back(11);
	
	mylist.Pushfront(16);
	cout << mylist << endl;
	cout << mylist.seq_size() << endl;
	cout << mylist.seq_capacity() << endl;
	mylist.insert(4, 78);
	mylist.insert(1, 199);
	
	cout << mylist << endl;
	cout << mylist.seq_size() << endl;
	cout << mylist.seq_capacity() << endl;

	mylist.sort();
	mylist.remove(11);
	cout << mylist << endl;
	cout << mylist.seq_size() << endl;
	cout << mylist.seq_capacity() << endl;

}
void test1()
{
	SeqList mylist;
	mylist.push_back(5);
	
	mylist.push_back(8);
	
	mylist.Pushfront(16);


	cout << mylist << endl;

}
int main()
{
	test1();
	system("pause");
	return 0;
}