
#include<iostream>
#include<cstdlib>
#include<functional>
#include<cstring>
//������װ����T�������ͣ�Fһ������
template <typename T,typename F>
T run(T v1,T v2, F f)
{
	static int count = 0;
	count++;
	std::cout << "����" << count << "��" << std::endl;

	return f(v1,v2);//�������������
}
template <typename T, typename F>
T run(T v1,  F f)
{
	static int count = 0;
	count++;
	std::cout << "������װ������" << count << "��" << std::endl;
	if (count > 1)//�޶�����ִ�еĴ�����
	{
		T vx(0);
		return vx;
	}
	return f(v1);//�������������
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
	//��Ϸ������;��
	//fun1��������һ������ָ�롣
	std::function<double(double)>fun1 = [](double u)
	{
		return u * 2; 
	};

	//[]ʵ�ֿ��ٺ�����
	std::function<double(double)>fun2 = [](double x)
	{
		return x*x;
	};
	std::function<int(int, int)>fun3 = [](int m,int n)
	{
		return n + m;
	};
	std::function<int(int, int)>fun4 = mul;
	//fun4�����ϻ���һ������ָ�룬���ǿ�������ָ��mul������
	//<int(int, int)>������Ǻ���ָ������͡�


	//function��һ��ͨ�õĺ�����double�Ƿ���ֵ������������ǲ���
	std::cout << run(num1, num2, fun3) << std::endl;
	std::cout << run(db, fun1) <<std::endl;
	std::cout << run(db, fun2) << std::endl;
	std::cout << run(num1, num2, fun4) << std::endl;
	std::cin.get();  //�ȼ���getchar
	system("pause");
	return 0;
}