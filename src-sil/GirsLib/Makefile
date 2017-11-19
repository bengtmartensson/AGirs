# Makefile for creating stuff on host.
# Use Arduino IDE for compiling for Arduino

CXX=g++
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra
#ARDUINO_LIBS=$(HOME)/Arduino/libraries
INFRARED4ARDUINO_DIR=../../../Infrared4Arduino
#ARDUINO_ROOT=/usr/local/arduino
INCLUDES=-I. -I$(INFRARED4ARDUINO_DIR)/src -I../../src/GirsLib -I../LcdI2C-sim -I../Girs

#VPATH=tests:../LcdI2C-sim

OBJS=\
Tokenizer.o LedLcdManager.o LiquidCrystal_I2C.o GirsUtils.o

#libGirsLib.a: $(OBJS)
#	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -std=c++11 $(INCLUDES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

Tokenizer: Tokenizer.o ../../libGirs.a
	$(CXX) -o $@ $< -L../.. -lGirs -L$(INFRARED4ARDUINO_DIR) -lInfrared
	./$@

test: test1

test1: Tokenizer

clean:
	rm -rf *.a *.o html test1

build-tests:
