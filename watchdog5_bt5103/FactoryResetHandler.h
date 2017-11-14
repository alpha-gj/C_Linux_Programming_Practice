#ifndef _FACTORYRESETHANDLER_H_
#define _FACTORYRESETHANDLER_H_
#include "IPCHandler.h"

class FactoryResetHandler : public IPCHandler
{
	private:
		void check_associate();

	public:
		FactoryResetHandler ();
		virtual ~FactoryResetHandler ();
		virtual int init();
		virtual int deinit();
		virtual int handle_ipc_depend_on_status(IpcCommand cmd);
		//TODO 
		/* virtual MAINSTATES GetMainHandlerState(); */
};

#endif
