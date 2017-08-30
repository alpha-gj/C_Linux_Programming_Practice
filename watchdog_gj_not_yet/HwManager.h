#ifndef _HWMANAGER_H_
#define _HWMANAGER_H_
#include <stdio.h>
#include <stdlib.h>
#include "pib/pib.h"
#include "tinydb/tinydb.h"

class HwManager
{
	private:
		/* Deny constructor & copy constructor */
		HwManger();
		~HwManger();
		HwManger(const HwManager&);
		HwManager& operator=(const HwManger&);

		/* Follow Singleton Pattern */
		static int referCount;
		static HwManager *hw_manager;

		/* Access HW object from MAP */
		HwController *hw_controller;
		map<const char*, HwController *> map_hw_controller;
		map<const char*, HwController *>::iterator in_map_hw_controller;

	public:
		/* Follow Singleton Pattern */
		static HwManager *CreateHwManager();
		static int ReleaseHwManager();

		/* Operate HW object from API */
		bool init_hw_info_by_type(const char* hw_name);
		bool deinit_hw_info_by_type(const char* hw_name);
		int set_hw_info_by_type(const char* hw_name, void hw_struct);
		int get_hw_info_by_type(const char* hw_name, void hw_struct);
		int run_hw_info_detect_by_type(const char* hw_name);
}

#endif
