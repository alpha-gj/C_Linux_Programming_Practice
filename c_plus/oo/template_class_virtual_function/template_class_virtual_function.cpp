#include <iostream>
#include <stdio.h>

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

template <class T> class Statusholder 
{
	protected:
		T status;

	public:
		Statusholder(T status);
		virtual	bool CheckStatus(T) = 0;
		virtual	T GetStatus() = 0;
		//virtual	void SetStatus(T,T) = 0;
		virtual	~Statusholder();

};

template <class T> Statusholder<T>::Statusholder(T s):status(s)
{
	cout << "Statusholder is constructing" << endl;
}
template <class T> Statusholder<T>::~Statusholder()
{
	cout << "Statusholder is destructing" << endl;
}

template <class T> class WPS_Statusholder:public Statusholder<WPS_STATUS>	//need to public Statusholder<T>, not just public Statusholder
{
	public:
		WPS_Statusholder();
		virtual~WPS_Statusholder();
		virtual bool CheckStatus(T);
		T GetStatus();

};

template <class T> WPS_Statusholder<T>::WPS_Statusholder():Statusholder((WPS_STATUS) IDLE)
{
	cout <<"WPS_Statusholder is constructing" << endl;
}
template <class T> WPS_Statusholder<T>::~WPS_Statusholder()
{
	cout <<"WPS_Statusholder is destructing" << endl;
}

template <class T> bool WPS_Statusholder<T>::CheckStatus(T type) 
{	
	status = (WPS_STATUS) OK;
	return true;
}

template <class T> T WPS_Statusholder<T>::GetStatus()
{
	return status;
}



int main()
{
	/*
	WPS_Statusholder<WPS_STATUS> wps;
	//wps.CheckStatus(what type did you want to search);  /
	cout << wps.GetStatus() << endl;
	*/

	//Like this
	holder->CheckStatus(WPS_STATUS);
	holder->GetStatus(WPS_STATUS);

	return 0;
}
