# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra
ARDUINO_LIBS=$(HOME)/Arduino/libraries
ARDUINO_ROOT=/usr/local/arduino
INFRARED4ARDUINO=$(ARDUINO_LIBS)/Infrared
INCLUDES=-I. -I$(INFRARED4ARDUINO)/src -I../LcdI2C-sim -I../GirsLib

VPATH=tests:../LcdI2C-sim

#OBJS=\
#Tokenizer.o LedLcdManager.o LiquidCrystal_I2C.o

#libGirsLib.a: $(OBJS)
#	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -std=c++11 $(INCLUDES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

Listener: Listener.o
	$(CXX) -o $@ $< -L../GirsLib -lGirsLib -L$(HOME)/Arduino/libraries/Infrared -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html test1

build-tests:

test: test1

