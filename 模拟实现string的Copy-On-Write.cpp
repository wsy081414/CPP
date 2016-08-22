#define _CRT_SECURE_NO_WARNINGS 1



#include<iostream>

#include<cstdlib>
#include<cstring>
#include<cassert>
using namespace std;
class String
{
	friend ostream& operator <<(ostream &os, String &str);
public:
	String(const char * str)
		:_str(new char [strlen(str)+1+4])
	{
		cout << "¹¹Ôìº¯Êý" << endl;
		_str = _str + 4;
		GetCount() = 1;
		strcpy(_str, str);
	}
	String(const String &str)
		:_str(str._str)
	{
		++GetCount();
	}
	~String()
	{
		release();
	}
	String& operator=(String &str)
	{
		if (this != &str)
		{
			release();
			_str = str._str;
			++GetCount();
		}
		return *this;
	}
	char &operator[](int index)
	{
		assert(index >= 0);
		assert(index<(int)strlen(_str));
		
		if (GetCount ()> 1)
		{
			--GetCount();
			char *tmp = new char[strlen(_str) + 1 + 4];
			strcpy(tmp + 4, _str);
			_str = tmp + 4;
			GetCount()=1 ;
		}
		return _str[index];
	}
	char *c_str()
	{
		return _str;
	}
private:
	int& GetCount()
	{
		return *(int *)(_str - 4);
	}
	void release()
	{
		if (--GetCount() == 0)
		{
			delete[](_str - 4);
			_str = NULL;
		}
	}
private:
	char *_str;
};

ostream& operator <<(ostream &os, String &str)
{
	os << str._str;
	return os;
}


void test1()
{
	String str1("12334");
	String str2(str1);
	String str3("world");
	String str4(str3);
	str3[3] = 'q';
	str2 = str3;
	cout << str1 << endl;
	cout << str3 << endl;

}
int main()
{
	test1();
	system("pause");
	return 0;
}

