#ifndef _HWMANAGER_H_
#define _HWMANAGER_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include "pib/pib.h"
#include "tinydb/tinydb.h"
#include "HwController.h"

class HwManager
{
	private:
		/* Deny constructor & copy constructor */
		HwManager();
		~HwManager();
		HwManager(const HwManager&);
		HwManager& operator=(const HwManager&);

		/* Follow Singleton Pattern */
		static HwManager *hw_manager;
		static int referCount;

		/* Access HW object from MAP */
		map<const char*, HwController *> map_hw_controller;
		HwController *ReturnHwControllerObjectByType(const char* hw_name);

	public:
		/* Follow Singleton Pattern */
		static HwManager *CreateHwManager();
		static int ReleaseHwManager();

		/* Operate HW object from API */
		bool init_hw_info_by_type(const char* hw_name);
		bool deinit_hw_info_by_type(const char* hw_name);
		int set_hw_info_by_type(const char* hw_name, void* hw_struct);
		int get_hw_info_by_type(const char* hw_name, void* hw_struct);
		int run_hw_info_detect_by_type(const char* hw_name);
};

#endif
