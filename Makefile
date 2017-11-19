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
WARNINGFLAGS=-Wall -Werror -Wextra
BOARD=nano

DOXYFILE := tools/keywords_txt_generator.doxy
TRANSFORMATION := tools/doxygen2keywords.xsl

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

#VPATH=tests src

#.PRECIOUS: test1

OBJS=\

libGirs.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp
	$(CXX) -Isrc -std=c++11 $(BOARDDEFINES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

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

test:

.PHONY: clean distclean spotless version keywords flasher flash-nano
