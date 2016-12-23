#pragma warning(disable : 4996)

#pragma once
#include"MyAlloc.hpp"
#include<time.h>
#include"list.hpp"
#include"Vector.hpp"


void test()
{
	

	Vector<int > v;
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(4);
	v.PushBack(3);
	v.PushBack(4);
	v.PushBack(1);

	Vector<int >::ReverseIterator iv = v.RBegin();
	while (iv != v.REnd())
	{
		cout << *iv << " ";
		++iv;
	}
	cout << endl;



}
void test1()
{
	List<int >l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);
	auto it = l.RBegin();
	while (it != l.REnd())
	{
		cout << *it << endl;
		++it;
	}
	cout << endl; 


}

int main()
{
	test();
	system("pasue");

	return 0;
}