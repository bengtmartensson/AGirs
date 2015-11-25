# Makefile for creating stuff on host.
# To compile for Arduino, use the Arduino IDE

BOARDDEFINES=
CXX=g++
DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Werror -Wextra
ARDUINO_LIBS=$(HOME)/Arduino/libraries
ARDUINO_ROOT=/local/arduino-1.6.5/
INFRARED4ARDUINO=../../../Infrared4Arduino
INCLUDES=-I. -I$(INFRARED4ARDUINO) -I../LcdI2C-sim -I../GirsLib -I../IrNamedCommand

VPATH=tests:../LcdI2C-sim

OBJS=\
Girs.o my_named_remotes.o

%.o: %.cpp
	$(CXX) -std=c++11 $(BOARDDEFINES) $(INCLUDES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

Girs: $(OBJS) ../IrNamedCommand/libIrNamedCommand.a ../GirsLib/libGirsLib.a ../../../Infrared4Arduino/libInfrared.a
	$(CXX) -o $@ $(OBJS) -L../GirsLib -lGirsLib -L../IrNamedCommand -lIrNamedCommand -L$(INFRARED4ARDUINO) -lInfrared

GirsLite: $(OBJS) ../IrNamedCommand/libIrNamedCommand.a ../GirsLib/libGirsLib.a ../../../Infrared4Arduino/libInfrared.a
	$(CXX) -o $@ GirsLite.o -L../GirsLib -lGirsLib -L../IrNamedCommand -lIrNamedCommand -L$(INFRARED4ARDUINO) -lInfrared

../IrNamedCommand/libIrNamedCommand.a ../GirsLib/libGirsLib.a $(INFRARED4ARDUINO)/libInfrared.a:
	$(MAKE) -C $(dir $@)

doc:
	doxygen
	$(BROWSER) html/index.html

clean:
	rm -rf *.a *.o html Girs

build-tests:

test: Girs
	./Girs

