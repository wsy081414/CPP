#include<iostream>
#include<cstdlib>
#include<cassert>

using namespace std;

class Base
{
public:
	static Base*  Getvalue(const int & a)
	{
		return new Base(a);
	}
	static void Destory(Base *b)
	{
		delete b;
		b = NULL;
	}
protected:
	int _b;
	Base(const int & b)
		:_b(b)
	{
	}
	~Base()
	{
	}
};
int main()
{
	Base *p = Base::Getvalue(44);

	Base::Destory(p);

	system("pause");

	return 0;
}