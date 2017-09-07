#ifndef _HWMANAGER_H_
#define _HWMANAGER_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include "pib/pib.h"
#include "tinydb/tinydb.h"
#include "HwController.h"
#include "ButtonController.h"
#include "WifiController.h"
#include "LEDController.h"

class HwManager
{
	private:
		/* Deny constructor & copy constructor by external */
		HwManager();
		~HwManager();
		HwManager(const HwManager&);
		HwManager& operator=(const HwManager&);

		/* Follow Singleton Pattern */
		static HwManager *hw_manager;
		static int referCount;

		/* For init & deint HwController object */
		int init();
		int deinit();

		/* 
			NOTE: Don't use const char* as primary key of STL map.
			It doesn't comapre "xxx", rather it comapres 0x12345678 
			by using find(...) of STL map. Please use string to replace it.
    	*/
		/* Access HW object from MAP */
		map<string, HwController *> map_hw_controller;
		HwController *ReturnHwControllerObjectByType(const char* hw_controller_name);

	public:
		/* Follow Singleton Pattern */
		static HwManager *CreateHwManager();
		static int ReleaseHwManager();

		/* Operate HwController object from API */
		int init_hw_info_by_type(const char* hw_name);
		int deinit_hw_info_by_type(const char* hw_name);
		int set_hw_info_by_type(const char* hw_name, void* hw_struct);
		int get_hw_info_by_type(const char* hw_name, void* hw_struct);
};

#endif
