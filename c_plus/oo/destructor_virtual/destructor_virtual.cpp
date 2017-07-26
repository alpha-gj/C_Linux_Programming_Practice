#include <iostream>

using namespace std;

class Animal
{
	public:
		Animal (int w = 0, int a =0);
		virtual ~Animal();
		//~Animal();   if use this one, Animal *pa which pointed to Cat obeject won't do Cat destructor, just do Animal destructor.
	private:
		int weight, age;
};

/* Animal */
Animal::Animal(int w, int a)
{
	cout << "Animal constructor is running..." << endl;
	weight = w;
	age = a;
}

Animal::~Animal()
{
	cout << "Animal destructor is running..." << endl;
}

/* Cat */
class Cat:public Animal
{
	public:
		Cat(int w = 0, int a = 0);
		~Cat();

};

Cat::Cat(int w, int a):Animal (w,a)
{
	cout << "Cat constructor is running..." << endl;
}

Cat::~Cat()
{
	cout << "Cat destructor is running..." << endl;
}

int main()
{
	Animal *pa = new Cat(2, 1);
	Cat *pc = new Cat(2, 4);
	cout << "=== Delete ===" << endl;
	cout << "Delete pa:" << endl;
	delete pa;
	cout << "Delete pc:" << endl;
	delete pc;
	return 0;
}

