#ifndef _COLOR_H_
#define _COLOR_H_
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <string>
#include <syslog.h>
#include "command.h"
#include "str/str.h"
using namespace std;

#define INTERACTIVE_MODE_PERCENT	1000

typedef enum {
	OFF,
	SOLID,
	BREATHING,
	BLINK,
}LED_WORKING_MODE;

typedef enum {
	LED_WHITE,
	LED_ORANGE,
	LED_BLUE,
	LED_GREEN,
	LED_RED,
	LED_YELLOW,
	LED_OFF,
}COLOR_PATTERN;


class Color {
	public:
		Color();
		virtual ~Color();

		void StoreColor(Color c);

	public:

	private:
};

#endif
