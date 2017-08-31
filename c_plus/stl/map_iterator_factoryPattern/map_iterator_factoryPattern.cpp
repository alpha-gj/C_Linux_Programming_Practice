#include <iostream>
#include <map>
#include <string>
#include "Status.h"

using namespace std;


class Holder
{
	private:
		map<const char*, Status *> map_status;
		map<const char*, Status *>::iterator in_map_status;

	public:
		Holder() {
		}
		~Holder() {

		}
		int GetStatusBySelect(const char* status) {
			in_map_status = map_status.find(status);
			Status *status_getter;

			if ( in_map_status == map_status.end())
			{
				cout << "Not found" << endl;
				return -1;
			}
			status_getter = in_map_status->second;
			return status_getter->GetStatus();
		}
		bool CreateAllStatus() {
			map_status.insert(make_pair("WPS", new WpsStatus()));
			map_status.insert(make_pair("NETWORK", new NetworkStatus()));
		}
		bool ReleaseAllStatus() {
			for (in_map_status = map_status.begin(); in_map_status != map_status.end(); in_map_status++) {
				delete in_map_status->second;
				in_map_status->second = NULL;
				//TODO Need to erase map element?
				/*
				   cout << in_release_map_status->first << endl;
				   map_status.erase(in_release_map_status);
				   cout << in_release_map_status->first << endl;
				 */
			}
		}
		/*
		bool CheckAllStatus() {
			//in_map_status = map_status.begin();
			//TODO for loop to map_status.end()

			in_map_status = map_status.find("WPS");
			status_getter = in_map_status->second;
			status_getter->CheckStatus();

			in_map_status = map_status.find("NETWORK");
			status_getter = in_map_status->second;
			status_getter->CheckStatus();

			return false;;

		}
		*/

};

int main()
{
	
	/* Create resource */
	Holder *holder = new Holder();
	holder->CreateAllStatus();

	/* GetStatusBySelect("...") */
	cout <<	"WPS is " << holder->GetStatusBySelect("WPS") << endl;;
	cout <<	"NETWORK is " << holder->GetStatusBySelect("NETWORK") << endl;

	/* Release resource */
	holder->ReleaseAllStatus();
	delete holder;

	return 0;
}
