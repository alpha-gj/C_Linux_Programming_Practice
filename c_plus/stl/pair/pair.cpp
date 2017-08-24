#include <iostream>
#include <utility>
#include <vector>
#include <vector>

using namespace std;

int main()
{
	pair<string, string> anon;
	anon.first = "AA";
	anon.second = "BB";
	pair<string, int> word_count;
	pair<string, vector<int> > line;

	pair<string, string> author("James", "Joyce");

	typedef pair<string, string> Author;
	Author author_v1("jack", "bob");
	Author author_v2("jack2", "bob2");

	string firstBook;

	firstBook = author_v1.first;
	cout << firstBook << endl;
	cout << anon.first << endl;
	cout << anon.second << endl;
}
