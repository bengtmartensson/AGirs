# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra

VPATH=tests

OBJS=\
IrNamedRemote.o  IrNamedRemoteSet.o

libIrNamedCommand.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -I. -I$(HOME)/Arduino/libraries/Infrared -std=c++11 $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

test%: test%.o libIrNamedCommand.a
	$(CXX) -o $@ $< -L. -lIrNamedCommand -L$(HOME)/Arduino/libraries/Infrared -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html test1

build-tests:

test: test1

