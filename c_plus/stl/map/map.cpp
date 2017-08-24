#include <iostream>
#include <map>
using namespace std;

int main()
{
	map <string, int> word_count;

/* pirntf word */
	/* printf one */
	word_count["one"] = 1;
	cout << word_count["one"] << endl;

	/* printf ++one */
	++word_count["one"];
	cout << word_count["one"] << endl;

	/* printf ++one */
	++word_count["one"];
	cout << word_count["one"] << endl;

	/* printf one */
	word_count["one"] = 1;
	cout << word_count["one"] << endl;



/* printf new word*/

	/* printf new word */
	cout << word_count["two"] << endl;

	/* printf new ++word */
	++word_count["two"];
	cout << word_count["two"] << endl;

	return 0;
}
