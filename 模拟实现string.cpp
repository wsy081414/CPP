//实现库中的string
class String
{

	friend ostream& operator<<(ostream& os, String& str);
public:

	String(const char *str = "")
		:_sz(strlen(str))
		, _capacity(strlen(str) + 1)
		, _str(new char[_capacity])
	{
		strcpy(_str, str);
	}

	String(const String &str)
		:_capacity(strlen(str._str) + 1)
		, _sz(str._sz)
		,_str(new char[strlen(str._str) + 1])
	{
		strcpy(_str, str._str);
		
	}

	~String()
	{
		if (_str != NULL)
		{
			delete[] _str;
			_sz = 0;
			_capacity = 0;
			_str = NULL;
		}
	}

	String& operator=(String str)
	{
		std::swap(_str, str._str);
		std::swap(_sz, str._sz);
		std::swap(_capacity, str._capacity);
		return *this;
	
	}

	char& operator[](int count)
	{
		return _str[count];
	}
	//返回C风格的字符指针的函数。
	char *c_str()
	{
		return _str;
	}
	
	void Push_Back(char ch)
	{
		CheckCapacity(1);
		_str[_sz++] = ch;
		_str[_sz] = '\0';

	}
	String& insert(size_t pos1, const String& str)
	{
		CheckCapacity(str._sz);
		memmove(_str+str._sz, _str + pos1, _sz - pos1);
		memmove(_str+pos1,str._str,str._sz-1);
		_sz += str._sz;
		
		return *this;
	}


private:
	void CheckCapacity(int count)
	{
		if (_sz + count >= _capacity)
		{
			int newcapacity =
				(2 * _capacity) > (_capacity + count)
				? (2 * _capacity) : (_capacity + count);
			char *tmp = new char[newcapacity];
			strcpy(tmp, _str);
			delete[] _str;
			_str = tmp;
			_capacity = newcapacity;
		}
	}

private:
	int _sz;
	int _capacity;
	char *_str;
	
};

ostream& operator<<(ostream& os, String& str)
{
	os << str._str;
	return os;
}




void test()
{
	String str1("123456");
	String str2("herllo");
	
	str1.insert(3, str2);
	cout << str1 << endl;

}
int main()
{

	test();
	system("pause");
	return 0;
}
