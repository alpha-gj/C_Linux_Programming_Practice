#include <iostream>
#include <stdio.h>

using namespace std;

template <class T> class Statusholder 
{
	private:
		T num;

	public:
		Statusholder();
		T GetStatus();
		~Statusholder();

};

template <class T> Statusholder<T>::Statusholder():num(1.23)
{
	printf("template <class T> Statusholder is constructer\n");
	cout << num << endl; 
}
template <class T> Statusholder<T>::~Statusholder()
{
	printf("template <class T> Statusholder is destructer\n");
	cout << num << endl; 
}

template <class T> T Statusholder<T>::GetStatus()
{
	return num;
}

int main()
{
	Statusholder<double> holder_double;
	cout << "double is " << holder_double.GetStatus() << endl;
	
	Statusholder<int> holder_int;
	cout << "int is " << holder_int.GetStatus() << endl;
	
	return 0;
}
