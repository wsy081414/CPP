
//一个简单的C++实现的复数类

#include<iostream>
#include<cstdlib>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               


using namespace std;

class Complex
{
public:
	Complex(double real=0.0,double image=0.0)
	{
		m_data = real;
		m_image = image;
	}

	~Complex()
	{
	}

public:
	void print()
	{
		if (m_image >= 0)
		{
			cout << m_data << "+" << m_image << "i" << endl;
		}
		else
		{
			cout << m_data << m_image << "i" << endl;
		}
	
	}
	Complex operator+(Complex &c)
	{
		Complex tmp;
		tmp.m_data = m_data + c.m_data;
		tmp.m_image = m_image + c.m_image;
		return tmp;
	}
	Complex operator-(Complex &c)
	{
		Complex tmp;
		tmp.m_data = m_data - c.m_data;
		tmp.m_image = m_image - c.m_image;
		return tmp;
	}

	//(a+bi)(c+di)=(ac-bd)+(bc+ad)i.
	Complex operator*(Complex &c)
	{
		Complex tmp;
		tmp.m_data = m_data*c.m_data-m_image*c.m_image;
		tmp.m_image =m_image*c.m_data+m_data*c.m_image;
		return tmp;
	}
	// x=(ac+bd)/(c^2+d^2) y=(bc-ad)/(c^2+d^2)
	Complex operator/(Complex &c)
	{
		Complex tmp;
		tmp.m_data =(m_data*c.m_data+m_image*c.m_image)/(c.m_data*c.m_data+c.m_image*c.m_image);
		tmp.m_image = (m_image*c.m_data-m_data*c.m_image)/(c.m_data*c.m_data+c.m_image*c.m_image);
		return tmp;
	}
	Complex& operator+=(Complex &c)
	{
	
		*this = *this + c;
		return *this;
	}
	Complex& operator-=(Complex &c)
	{
		*this = *this - c;
		return *this;
	}
	Complex& operator*=(Complex &c)
	{
		*this = *this * c;
		return *this;
	}
	Complex& operator/=(Complex &c)
	{
		*this = *this / c;
		return *this;
	}
private:
	double m_data;
	double m_image;

};
int main()
{
	Complex c1(1,3);
	Complex c2(2, 4);
	Complex c3 = c1 - c2;
	c3.print();
	system("pause");
	return 0;
}