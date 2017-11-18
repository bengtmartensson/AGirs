# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra
ARDUINO_LIBS=$(HOME)/Arduino/libraries
ARDUINO_ROOT=/usr/local/arduino
INFRARED4ARDUINO=$(ARDUINO_LIBS)/Infrared

VPATH=tests

OBJS=\
IrNamedRemote.o  IrNamedRemoteSet.o

libIrNamedCommand.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -I. -I$(INFRARED4ARDUINO)/src -std=c++11 $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

test%: test%.o libIrNamedCommand.a
	$(CXX) -o $@ $< -L. -lIrNamedCommand -L$(INFRARED4ARDUINO)/Arduino/libraries/Infrared -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html test1

build-tests:

test: test1

