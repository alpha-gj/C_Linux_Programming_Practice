CC = g++
Target = $(shell find . -name *.c | cut -d/ -f2 | cut -d. -f1) 
OBJS = $(shell find . -name *.c | cut -d/ -f2 | cut -d. -f1).o 
# it add a space after $Target...I don't know why.
#OBJS = $(Target).o

.PHONY: all clean install

install: all

all: $(Target) 

#all:
#	@echo $(Target)
#	@echo $(OBJS)

%.o: %.c
	$(CC) -c -o $@ $<

$(Target): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -f $(Target) 
	rm -f *.o *.d *~
