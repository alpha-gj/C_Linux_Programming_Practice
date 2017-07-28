#include"student.h"

class Undergraduate:public student
{
	public:
		Undergraduate();
		Undergraduate(const char*, int, int, int);
		double score();
		void setGPA(double g);
		bool isAdult();

	protected:
		double GPA;
};

Undergraduate::Undergraduate()
{
	cout << "Undergraduate constructer without parameter is running..." << endl;
}

Undergraduate::Undergraduate(const char* c, int a, int h, int w):student(c, a ,h ,w)
{
	cout << "Undergraduate constructer with parameter is running..." << endl;
}

double Undergraduate::score()
{
	return GPA;
}

void Undergraduate::setGPA(double g)
{
	GPA = g;
	return;
}

bool Undergraduate::isAdult()
{
	return age >= 18?true:false;
}
