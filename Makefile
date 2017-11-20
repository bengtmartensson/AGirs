# Makefile for creating stuff on host.
# Not used for "normal" build of the software for the Arduino.
# Use Arduino IDE (https://www.arduino.cc/en/Main/Software) for that.

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

# External programs, change if necessary or desired.
CXX=g++
BROWSER=firefox
DOXYGEN := doxygen
XSLTPROC := xsltproc

DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Wextra
BOARD=nano

DOXYFILE := tools/keywords_txt_generator.doxy
TRANSFORMATION := tools/doxygen2keywords.xsl

# Should poiint to the directory where the Infrared4Arduino
# (https://github.com/bengtmartensson/Infrared4Arduino)
# sources are located. Only used for SIL test.
INFRARED4ARDUINO_DIR=../Infrared4Arduino

# Get VERSION from the version in library.properties
VERSION=$(subst version=,,$(shell grep version= library.properties))

FLASHER=GirsLite-$(VERSION)-$(BOARD)-flasher.sh
HEXFILE=GirsLite-$(VERSION)-$(BOARD).hex

all: version keywords flasher api-doc/index.html test

flasher: $(FLASHER)
hex: $(HEXFILE)

$(FLASHER) $(HEXFILE):
	tools/mkflasher.sh
	
# Flash an Arduino nano on /dev/ttyUSB0, without questions etc. May require root.
# Warning: overwrites without question! Be careful!
flash-nano: $(FLASHER)
	./$(FLASHER)

keywords: keywords.txt

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< > $@

xml/index.xml: $(DOXYFILE)
	$(DOXYGEN) $(DOXYFILE)

lib: libGirs.a

INCLUDES=-I$(INFRARED4ARDUINO_DIR)/src -Isrc/config
VPATH=src src/GirsLib src/IrNamedCommand

#.PRECIOUS: test1

OBJS=Girs.o GirsUtils.o LedLcdManager.o Tokenizer.o

libGirs.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -std=c++11 $(INCLUDES) $(BOARDDEFINES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

#test%: test%.o libInfrared.a
#	$(CXX) -o $@ $< -L. -lInfrared
#	./$@

doc: api-doc/index.html
	$(BROWSER) $<
	
api-doc/index.html:
	$(DOXYGEN)

version: src/GirsLib/version.h
	
src/GirsLib/version.h: library.properties
	echo "// This file was automatically generated from $<; do not edit." >  $@
	echo "#define VERSION \"$(VERSION)\""  >> $@

clean:
	rm -rf *.a *.o *.hex *.zip xml test1 $(FLASHER)
	
distclean: clean
	rm -rf api-doc

# Remove all products. Do not use before commit.
spotless: distclean
	rm -f keywords.txt src/GirsLib/version.h

build-tests:

test: lib

.PHONY: clean distclean spotless version keywords flasher flash-nano lib
