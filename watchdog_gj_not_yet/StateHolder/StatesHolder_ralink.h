#ifndef _STATESHOLDER_H_
#define _STATESHOLDER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Color.h"
#include "tinydb/tinydb.h"
#include "cam/cam.h"
#include "todo.h"

enum MAINSTATES {
	POWERUP,
	POWERUPWITHBUTTON,
	SETUP,
	NORMAL,
	TROUBLE_SHOOTING,
	TWT,
	PRIVACY,
	QUIET,
	REBOOT,
	RESET,
	REPORT,
	GENERIC_LED,
	TWT_QUIET,
	MAINSTATES_COUNT,
};

enum WPSSTATES {
	WPS_OFF,
	WPS_PAIRING,
	WPS_FAIL,
	WPS_OK,
	WPSSTATES_COUNT,
};

enum TWTSTATES {
	CALL_OFF,
	CALLING_OUT,
	CALL_FROM_OUTSIDE,
	TALKING,
};

enum NFCSTATES {
	NFC_OFF,
	NFC_WORKING,
	NFC_SUCCESS,
};

struct button_states {
	bool buttonOn;
	struct timeval buttonOnTime;
	char reserve[3];
};

struct net_states {
	bool loseConnectionNeedWait;
	bool needHandleACK;
	bool panelACK;
	bool needHandleAssociated;
	bool associated;
};

class StatesHolder
{
public:
	virtual ~StatesHolder ();
	static StatesHolder *CreateStatesHolder();
	static void releaseStatesHolder();

public:
	void SetMainStates(MAINSTATES s);
	void SetMainStatesByCameraMode();
	MAINSTATES GetMainStates();
	MAINSTATES GetOldMainStates();

	void SetWpsStates(WPSSTATES s);
	WPSSTATES GetWpsStates();

	void SetButtonStates(bool b, struct timeval t);
	button_states GetButtonStates();

	void SetTwtStates(TWTSTATES s);
	TWTSTATES GetTwtStates();

	void SetLastColor1(Color c);
	Color GetLastColor1();

	bool GetNeedHandleACK();
	bool GetNeedHandleAssociated();
	void CleanNeedHandleACK();
	void CleanNeedHandleAssociated();
	bool UnderLoseConnectionNeedWait();
	void SetUnderLoseConnectionNeedWait(bool lose);
	bool IsPanelACK();
	void SetPanelACK(bool b);
	bool IsAssociated();
	void SetAssociated(bool b);
	void SetTwtEnableFromDB();
	bool GetTwtEnable();
	void SetNfcStates(NFCSTATES s);
	NFCSTATES GetNfcStates();

	void SetTwtBeginEndSetting(twt_beginend_setting &set);
	twt_beginend_setting GetTwtBeginEndSetting();
	void SetStandbySetting(standby_setting &set);
	standby_setting GetStandbySetting();
private:
	StatesHolder ();

private:
	pthread_mutex_t mainStatesLock;
	MAINSTATES mainStates;
	MAINSTATES oldMainStates;

	pthread_mutex_t wpsStatesLock;
	WPSSTATES wpsStates;

	pthread_mutex_t buttonStatesLock;
	button_states buttonStates;

	pthread_mutex_t twtStatesLock;
	TWTSTATES twtStates;

	Color lastColor1;

	net_states net;

	bool twtEnable;

	pthread_mutex_t nfcStatesLock;
	NFCSTATES nfcStates;

	twt_beginend_setting twtBeginEndSetting;
	standby_setting standbySetting;
};

#endif
