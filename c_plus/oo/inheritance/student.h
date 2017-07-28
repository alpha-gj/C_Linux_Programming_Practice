#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

class student
{
	public:
		student(const char *n, int a, int h, int w);
		student();
		void set(const char *n, int a,int h, int w);
		const char *sname();
		int sage();
		int sheight();
		int sweight();

	protected:
		const char *name;
		int age;
		int height;
		int weight;

	private:
		int test;
};

const char *student::sname()
{	
	return name;
}

int student::sage()
{
	return age;
}

int student::sheight()
{
	return height;
}

int student::sweight()
{
	return weight;
}

void student::set(const char *n, int a, int h, int w)
{
	/*
	int i;
	for (i = 0; n[i]!='\0'; i++)
	{
		name[i] = n[i];
	}
	name[i] = '\0';
	
	name[sizeof(name)-1] = '\0';								//don't use this 
	printf("sizeof of name is %d, i is %d\n", sizeof(name), i);
	*/
	name = n;
	age = a;
	height = h;
	weight = w;
	return;
}

/*
student::student(char *n, int a, int h, int w):name(n), age(a), height(h), weight(w)
{
	//doing nothing
}
*/

#if 0
student::student(const char *n, int a, int h, int w)
{
	cout << "Constructing a student with parameter..." << endl;
	set(n,a,h,w);
}
#else
student::student(const char * n, int a, int h, int w):name(n), age(a), height(h), weight(w)
{
	cout << "Constructing a student with parameter (member initializtion)..." << endl;
}
#endif

student::student()
{
	cout << "Constructing a student without parameter..." << endl;
}
