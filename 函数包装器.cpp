
#include<iostream>
#include<cstdlib>
#include<functional>
#include<cstring>
//函数包装器，T数据类型，F一个函数
template <typename T,typename F>
T run(T v1,T v2, F f)
{
	static int count = 0;
	count++;
	std::cout << "进行" << count << "次" << std::endl;

	return f(v1,v2);//函数传入参数。
}
template <typename T, typename F>
T run(T v1,  F f)
{
	static int count = 0;
	count++;
	std::cout << "函数包装器进行" << count << "次" << std::endl;
	if (count > 1)//限定函数执行的次数。
	{
		T vx(0);
		return vx;
	}
	return f(v1);//函数传入参数。
}

int mul(int a, int b)
{

	return a*b;
}

int main()
{
	double db(12.9);//double*2
	int num1 = 19;
	int num2 = 20;
	//游戏开发用途多
	//fun1本质上是一个函数指针。
	std::function<double(double)>fun1 = [](double u)
	{
		return u * 2; 
	};

	//[]实现开辟函数。
	std::function<double(double)>fun2 = [](double x)
	{
		return x*x;
	};
	std::function<int(int, int)>fun3 = [](int m,int n)
	{
		return n + m;
	};
	std::function<int(int, int)>fun4 = mul;
	//fun4本质上还是一个函数指针，我们可以让它指向mul函数。
	//<int(int, int)>代表的是函数指针的类型。


	//function是一个通用的函数，double是返回值，（）里面的是参数
	std::cout << run(num1, num2, fun3) << std::endl;
	std::cout << run(db, fun1) <<std::endl;
	std::cout << run(db, fun2) << std::endl;
	std::cout << run(num1, num2, fun4) << std::endl;
	std::cin.get();  //等价于getchar
	system("pause");
	return 0;
}