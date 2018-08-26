# Makefile for creating stuff on host.
# Not used for "normal" build of the software for the Arduino.
# Use Arduino IDE (https://www.arduino.cc/en/Main/Software) for that.

# The functions for generating keywords.txt requires KeywordsTxtGenerator,
# https://github.com/bengtmartensson/KeywordsTxtGenerator, to be installed in
# KEYWORD_TXT_GENERATOR_DIR.

ifneq ($(ARDUINO),)
$(error This Makefile is not for compiling target code, for that, use the Arduino IDE.)
endif

KEYWORD_TXT_GENERATOR_DIR = ../KeywordsTxtGenerator

# External programs, change if necessary or desired.
CXX=g++
DOXYGEN := doxygen
DOXYFILE := Doxyfile
XSLTPROC := xsltproc
TRANSFORMATION := $(KEYWORD_TXT_GENERATOR_DIR)/doxygen2keywords.xsl
BROWSER=firefox

DEBUGFLAGS=-g
WARNINGFLAGS=-Wall -Wextra
BOARD=nano

# Should point to the directory where the Infrared4Arduino
# (https://github.com/bengtmartensson/Infrared4Arduino)
# sources are located. Only used for SIL test.
INFRARED4ARDUINO_DIR=../Infrared4Arduino

# Get VERSION from the version in library.properties
VERSION=$(subst version=,,$(shell grep version= library.properties))
ORIGINURL=$(shell git remote get-url origin)

FLASHER=GirsLite-$(VERSION)-$(BOARD)-flasher.sh
HEXFILE=GirsLite-$(VERSION)-$(BOARD).hex

default: all

VERSION_H := src/GirsLib/version.h

version: $(VERSION_H)

$(VERSION_H): library.properties Makefile
	echo "// This file was automatically generated from $<; do not edit." > $@
	echo "/**"                                                           >> $@
	echo " * Version of the current library."                            >> $@
	echo " * Taken from the version in $<."                              >> $@
	echo " */"                                                           >> $@
	echo "#define VERSION \"$(VERSION)\""                                >> $@

flasher: $(FLASHER)
hex: $(HEXFILE)

$(FLASHER) $(HEXFILE):
	tools/mkflasher.sh

# Flash an Arduino nano on /dev/ttyUSB0, without questions etc. May require root.
# Warning: overwrites without question! Be careful!
flash-nano: $(FLASHER)
	./$(FLASHER)

lib: libGirs.a

INCLUDES=-I$(INFRARED4ARDUINO_DIR)/src -Isrc -Iattic/src-sil/LcdI2C
VPATH=src src/GirsLib examples/Girs

#.PRECIOUS: test1

# TODO: compile Girs.o separately, in tests/Girs
OBJS=Girs.o GirsUtils.o LedLcdManager.o Tokenizer.o

libGirs.a: $(OBJS)
	$(AR) rs $@ $(OBJS)

%.o: %.cpp $(VERSION_H)
	$(CXX) -std=c++11 $(INCLUDES) $(BOARDDEFINES) $(WARNINGFLAGS) $(OPTIMIZEFLAGS) $(DEBUGFLAGS) -c $<

#test%: test%.o libInfrared.a
#	$(CXX) -o $@ $< -L. -lInfrared
#	./$@

doc: api-doc/index.html
	$(BROWSER) $<

api-doc/index.html xml/index.xml: $(wildcard src/* src/*/* examples/*/*) Doxyfile README.md $(VERSION_H)
	$(DOXYGEN) $(DOXYFILE) > /dev/null

gh-pages: api-doc/index.html
	rm -rf gh-pages
	git clone --depth 1 -b gh-pages $(ORIGINURL) gh-pages
	rm -rf gh-pages/*
	cp -rf api-doc/* gh-pages
	(cd gh-pages; git add . )
	(cd gh-pages; git commit -a -m "Update of API documentation for version $(VERSION)")
	@echo Now perform \"git push\" from gh-pages

tag:
	git checkout master
	git status
	git tag -a Version-$(VERSION) -m "Tagging Version-$(VERSION)"
	git push origin Version-$(VERSION)

clean:
	rm -rf *.a *.o *.hex *.zip api-doc xml gh-pages test1 $(FLASHER)

# Remove all products. Do not use before commit.
spotless: clean
	rm -f keywords.txt $(VERSION_H)

keywords: keywords.txt

keywords.txt: xml/index.xml
	$(XSLTPROC) $(TRANSFORMATION) $< | uniq > $@

build-tests:

test: lib

all: version keywords flasher api-doc/index.html test

.PHONY: clean distclean spotless version keywords flasher flash-nano lib
