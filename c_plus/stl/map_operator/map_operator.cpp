#include <iostream>
#include <map>
#include <string>

using namespace std;


int main()
{
	map <string, int> word_count;
	bool ret;
	ret = word_count.insert(map<string, int>::value_type("GJ", 1)).second;
	cout << "ret is " << ret << " and" <<  word_count["GJ"] << endl;
	ret = word_count.insert(map<string, int>::value_type("GJ", 1)).second;
	cout << "ret is " << ret << " and" <<  word_count["GJ"] << endl;

	map < string, int>::iterator map_int = word_count.begin();
	ret = word_count.insert(map<string, int>::value_type("GJ2", 1)).second;
	ret = word_count.insert(map<string, int>::value_type("GJ3", 1)).second;
	map_int = word_count.insert(map<string, int>::value_type("GJ4", 1000)).first;

	cout <<	map_int->first  <<endl;
	cout << map_int->second << endl;

	bool b_count;

	b_count = word_count.count("eric");
	if (b_count) {
		cout << "we cout " <<word_count["GJ"] << endl;
	} else {
		cout <<"we can't count " << endl;
	}

	map_int = word_count.find("GJ");
	if(map_int != word_count.end())
	{
		cout << map_int->first << " " << map_int->second;
	}


	/*
	cout << word_count.insert(map<string, int>::value_type("Anna", 3)).second << endl;
	cout << word_count.insert(map<string, int>::value_type("Anna", 3)).second << endl;
	cout << word_count.insert(map<string, int>::value_type("Anna", 3)).second << endl;
	cout << word_count["Anna"]  << endl;
	++word_count["Anna"];
	cout << word_count["Anna"]  << endl;

	map < string, int>::iterator map_it = word_count.begin();
	cout << map_it -> first << endl;
	cout << map_it -> second << endl;
	word_count.insert(map<string, int>::value_type("Jack", 3));
	map_it++;
	cout << map_it -> first << endl;
	cout << map_it -> second << endl;
	*/
	return 0;
}
