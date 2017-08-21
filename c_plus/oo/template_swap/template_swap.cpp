#include <iostream>
#include <stdio.h>

using namespace std;

template <class T> void Swap(T &a, T &b);


int main()
{
	int a = 3, b = 2;
	char c ='a', d = 'b';

	Swap(a, b);
	printf("a is %d, b is %d\n", a, b);
	
	Swap(c, d);
	printf("c = %c, d is %c\n", c, d);
}

template <class T> void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
	return;
}
