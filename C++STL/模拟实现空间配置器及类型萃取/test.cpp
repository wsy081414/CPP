#pragma once
#include"MyAlloc.hpp"
#include<vector>
#include<time.h>
#include"list.hpp"


void test()
{
	

	Vector<int > v;
	v.PushBack(1);
	v.PushBack(12);
	v.PushBack(14);
	v.PushBack(14123);
	v.PushBack(143);
	v.PushBack(112);


	auto it = v.Begin();
	while (it != v.End())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	cout << Distance(v.Begin(),v.End()) << endl;
	auto iv = v.Begin();

	Advance(iv, 3);



}
int main()
{
	test();
	system("pasue");

	return 0;
}