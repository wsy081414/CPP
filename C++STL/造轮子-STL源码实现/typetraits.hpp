
#pragma once


struct __TrueType
{
	bool Get()
	{
		return true;
	}
};

struct __FalseType
{
	bool Get()
	{
		return false;
	}
};

template <class _Tp>
struct TypeTraits
{
	typedef __FalseType    HasTrivialDefaultConstructor;
	typedef __FalseType    HasTrivialDopyConstructor;
	typedef __FalseType    HasTrivialDssignmentOperator;
	typedef __FalseType    HasTrivialDestructor;
	typedef __FalseType   __IsPODType;
};

template <>
struct TypeTraits< bool>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< char>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< unsigned char >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< short>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< unsigned short >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< int>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< unsigned int >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< long>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< unsigned long >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< long long >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< unsigned long long>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< float>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< double>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <>
struct TypeTraits< long double >
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};

template <class _Tp>
struct TypeTraits< _Tp*>
{
	typedef __TrueType    HasTrivialDefaultConstructor;
	typedef __TrueType    HasTrivialDopyConstructor;
	typedef __TrueType    HasTrivialDssignmentOperator;
	typedef __TrueType    HasTrivialDestructor;
	typedef __TrueType     __IsPODType;
};
