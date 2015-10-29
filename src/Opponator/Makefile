# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
BROWSER=firefox
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra
ARDUINO_LIBS=$(HOME)/Arduino/libraries
ARDUINO_ROOT=/local/arduino-1.6.5/
INCLUDES=-I. -I$(ARDUINO_LIBS)/Infrared -I../LcdI2C-sim -I../GirsLib

VPATH=tests:../LcdI2C-sim

#OBJS=\
#Tokenizer.o LedLcdManager.o LiquidCrystal_I2C.o

#libGirsLib.a: $(OBJS)
#	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -std=c++11 $(INCLUDES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

Opponator: Opponator.o
	$(CXX) -o $@ $< -L../GirsLib -lGirsLib -L$(HOME)/Arduino/libraries/Infrared -lInfrared
	./$@

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html test1

build-tests:

test: test1

