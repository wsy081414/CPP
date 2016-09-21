
#pragma once
#include<iostream>
using namespace std;
//¿‡–Õ›Õ»°
struct __truetype
{
	bool get()
	{
		return true;
	}
};

struct __falsetype
{
	bool get()
	{
		return false;
	}

};


template<typename T>

struct typetraits
{
	typedef __falsetype __ispodtype;

};

template<>
struct typetraits<int >
{
	typedef __truetype __ispodtype;
};

template<>
struct typetraits<char >
{
	typedef __truetype __ispodtype;
};

template<>
struct typetraits<short >
{
	typedef __truetype __ispodtype;
};

template<>
struct typetraits<bool >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<unsigned int >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<unsigned short >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<unsigned long >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<long >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<long long >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<unsigned long long >
{
	typedef __truetype __ispodtype;
};

template<>
struct typetraits<long double >
{
	typedef __truetype __ispodtype;
};
template<>
struct typetraits<double >
{
	typedef __truetype __ispodtype;
};

template<>
struct typetraits<float >
{
	typedef __truetype __ispodtype;
};

