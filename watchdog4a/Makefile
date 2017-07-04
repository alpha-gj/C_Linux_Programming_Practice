DAEMON = Jobber
TARGETS = $(DAEMON)

SRCS = $(DAEMON).cpp IPCHandler.cpp StatesHolder.cpp PowerUpHandler.cpp

PREFIX = $(PJ_PREFIX)
CROSS_COMPILE = $(PJ_TARGET)-

CFLAGS    = $(PLATFORM_CFLAGS) -Wall -W -D$(subst -,_,$(PJ_MODEL)) #-DDEBUG_FPRINTF
CXXFLAGS  = $(CFLAGS)
CPPFLAGS  = $(PLATFORM_CPPFLAGS) -D_REENTRANT -D_LINUX -I$(PREFIX)/include \
			-I../include -I$(PJ_KERNEL_SOURCE)
LDFLAGS   = $(PLATFORM_LDFLAGS) -s -Wl,-rpath-link=$(PJ_RPATH)
LOADLIBES = $(PLATFORM_LOADLIBES) -L$(PREFIX)/lib
LDLIBS    = $(PLATFORM_LDLIBS) -lcam -ltinydb -lsensor -lMulCast

CC        = $(CROSS_COMPILE)gcc
CPP       = $(CROSS_COMPILE)gcc -E
CXX       = $(CROSS_COMPILE)g++
AS        = $(CROSS_COMPILE)as
AR        = $(CROSS_COMPILE)ar
LD        = $(CROSS_COMPILE)ld
NM        = $(CROSS_COMPILE)nm
OBJCOPY   = $(CROSS_COMPILE)objcopy
OBJDUMP   = $(CROSS_COMPILE)objdump
RANLIB    = $(CROSS_COMPILE)ranlib
READELF   = $(CROSS_COMPILE)readelf
SIZE      = $(CROSS_COMPILE)size
STRINGS   = $(CROSS_COMPILE)strings
STRIP     = $(CROSS_COMPILE)strip

include config.mk

.PHONY: all clean
all: .options $(TARGETS) $(UTEST)
clean:
	rm -f *.o *.d *~ $(VERDOR_DEPS) $(VENDOR_OBJS)
	rm -f $(TARGETS) $(UTEST)

include ../common_apps.mk
install: .options $(TARGETS)
	# test PREFIX
	[ "$(PREFIX)" != "" ]
	install -d -m 755 $(PREFIX_SBINDIR)/
	install -m 755 $(TARGETS) $(PREFIX_SBINDIR)/
	install -d -m 755 $(PREFIX_INITDIR)/
	install -m 755 watchdog.sh $(PREFIX_INITDIR)/
	install -d -m 755 $(PREFIX_BINDIR)/

OPTIONS = $(SRCS) $(CPPFLAGS) $(LDLIBS)
.PHONY: .options
.options:
	[ -f .options ] || touch .options
	if [ "$$(md5sum .options | cut -b1-32)" != "$$(echo '$(OPTIONS)' | md5sum | cut -b1-32)" ] ; then \
		make clean && echo '$(OPTIONS)' > .options; \
	fi

%.d: %.c
	$(CC) -c -MM $(CPPFLAGS) $< | sed -re 's,^.*:,$*.o $*.d:,g' > $@
%.d: %.cpp
	$(CC) -c -MM $(CPPFLAGS) $< | sed -re 's,^.*:,$*.o $*.d:,g' > $@
DEPENDS := $(SRCS:%.c=%.d)
DEPENDS := $(DEPENDS:%.cpp=%.d)
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

$(DAEMON): $(DAEMON).o IPCHandler.o StatesHolder.o PowerUpHandler.o
