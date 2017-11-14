#include <iostream>
using namespace std;

typedef enum wps_status {
	IDLE = 0,
	SCAN,
	PAIR,
	OK,
	FAIL,
} WPS_STATUS;

typedef enum network_status {
	ASSOCIATED,
	DEASSOCIATED,
} NETWORK_STATUS;

class Status 
{
	protected:
		int status;

	public:
		Status();
		virtual ~Status();
		virtual int GetStatus() = 0;
		virtual bool CheckStatus() = 0;
};

Status::Status()
{
	cout<<"Status constructor" << endl;
}

Status::~Status()
{
	cout<<"Status destructor" << endl;
}

class WpsStatus : public Status
{
	public:
		WpsStatus();
		virtual ~WpsStatus();
		virtual int GetStatus() {
			CheckStatus();
			cout << "GetStatus WPS_STATUS" << endl;
			return (WPS_STATUS) status;
		}
		virtual bool CheckStatus() {
			cout << "CheckStatus WPS_STATUS" << endl;
			status = (WPS_STATUS) PAIR;
			return true;
		}
};

WpsStatus::WpsStatus()
{
	cout<<"WpsStatus constructor" << endl;
}
WpsStatus::~WpsStatus()
{
	cout<<"WpsStatus destructor" << endl;
}

class NetworkStatus : public Status
{
	public:
		NetworkStatus();
		virtual ~NetworkStatus();
		virtual int GetStatus() {
			CheckStatus();
			cout << "GetStatus NETWORK_STATUS" << endl;
			return (NETWORK_STATUS) status;
		}
		virtual bool CheckStatus() {
			cout << "CheckStatus NETWORK_STATUS" << endl;
			status = (NETWORK_STATUS) DEASSOCIATED;
			return true;
		}
};

NetworkStatus::NetworkStatus()
{
	cout<<"NetworkStatus constructor" << endl;
}
NetworkStatus::~NetworkStatus()
{
	cout<<"NetworkStatus destructor" << endl;
}
