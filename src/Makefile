# Arduino NetBeans plugin (http://arduino-netbeans.googlecode.com)
# ------------------------------------------------------------------------------
# Configure the values of COM_PORT, ARDUINO_BASE_DIR and platform (Uno or Mega).
# Uncomment the declarations of the libraries that you want to use.
# For Arduino Duemilanove you must change the BAUD_RATE to 57600 bps.
# Based on: http://playground.arduino.cc/Code/Netbeans

ifeq ($(BOARD),nano)
COM_PORT = /dev/ttyUSB0
#BAUD_RATE = 115200
BAUD_RATE = 57600
else
COM_PORT = /dev/ttyACM0
BAUD_RATE = 115200
endif
#COM_PORT = COM23
#BAUD_RATE = 115200
ARDUINO_VERSION = 105
ARDUINO_BASE_DIR = /local/arduino-1.0.5
ARDUINO_CORE_DIR = ${ARDUINO_BASE_DIR}/hardware/arduino/cores/arduino
ARDUINO_LIB_DIR = ${ARDUINO_BASE_DIR}/libraries
LIB_CORE_DIR = lib/core
LIB_LIBS_DIR = lib/libs
ARDUINO_LIB_CORE = ${LIB_CORE_DIR}/arduinocore.a
ARDUINO_LIB_LIBS = ${LIB_LIBS_DIR}/arduinolibs.a

# Arduino Uno/Nano:
ARDUINO_MODEL = atmega328p
ARDUINO_PROGRAMMER = arduino
ARDUINO_PINS_DIR = ${ARDUINO_BASE_DIR}/hardware/arduino/variants/standard

# Arduino Mega 2560: Broken
#ARDUINO_MODEL = atmega2560
#ARDUINO_PROGRAMMER = wiring
#ARDUINO_PINS_DIR = ${ARDUINO_BASE_DIR}/hardware/arduino/variants/mega

# Arduino Duemilanove:
#BAUD_RATE = 57600

# Include the libraries that you want. This are subfolders of "arduino-1.0.4/libraries" folder:
INCLUDE_LIBS=Ethernet;Ethernet/utility;SPI;
#INCLUDE_LIBS=EEPROM;Esplora;Ethernet;Ethernet/utility;Firmata;GSMSHIELD;\
LiquidCrystal;MemoryFree;RTClib;SD;SD/utility;Servo;SoftwareSerial;SPI;\
Stepper;WiFi;WiFi/utility;Wire;Wire/utility;\

# Select the Windows or Linux config of AVR_DUDE (upload utility):
AVR_DUDE_WINDOWS = ${ARDUINO_BASE_DIR}/hardware/tools/avr/bin/avrdude -C ${ARDUINO_BASE_DIR}/hardware/tools/avr/etc/avrdude.conf
AVR_DUDE_LINUX = ${ARDUINO_BASE_DIR}/hardware/tools/avrdude -C ${ARDUINO_BASE_DIR}/hardware/tools/avrdude.conf
AVR_DUDE = ${AVR_DUDE_LINUX}

#
############################# END OF USER CHANGES #############################
#

INCLUDE = -c -g \
-I${ARDUINO_CORE_DIR} \
-I${ARDUINO_PINS_DIR} \
$(patsubst %,-I${ARDUINO_LIB_DIR}/%,$(subst ;, ,$(INCLUDE_LIBS)))

FLAGS_GCC = -c -g -Os -Wall -ffunction-sections -fdata-sections -mmcu=${ARDUINO_MODEL} -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=${ARDUINO_VERSION}
FLAGS_GPP = ${FLAGS_GCC} -fno-exceptions
FLAGS_LINKER = ${ARDUINO_LIB_CORE} ${ARDUINO_LIB_LIBS} -Os -Wl,--gc-sections,--relax -mmcu=${ARDUINO_MODEL} -lm
CMD_AVR_GCC = avr-gcc ${FLAGS_GCC} ${INCLUDE}
CMD_AVR_GPP = avr-g++ ${FLAGS_GPP} ${INCLUDE}
CMD_AVR_AR = avr-ar rcs

CORE_CPP_SOURCES = $(wildcard ${ARDUINO_CORE_DIR}/*.cpp)
CORE_C_SOURCES = $(wildcard ${ARDUINO_CORE_DIR}/*.c)
COBJECTS=$(CORE_CPP_SOURCES:.cpp=.cpp.o) $(CORE_C_SOURCES:.c=.c.o)
CORE_OBJECTS=$(subst ${ARDUINO_CORE_DIR},${LIB_CORE_DIR},${COBJECTS})

LIB_CPP_SOURCES = $(wildcard $(patsubst %,${ARDUINO_LIB_DIR}/%/*.cpp,$(subst ;, ,$(INCLUDE_LIBS))))
LIB_C_SOURCES = $(wildcard $(patsubst %,${ARDUINO_LIB_DIR}/%/*.c,$(subst ;, ,$(INCLUDE_LIBS))) )
LOBJECTS=$(LIB_CPP_SOURCES:.cpp=.cpp.o) $(LIB_C_SOURCES:.c=.c.o)
LIB_OBJECTS=$(subst ${ARDUINO_LIB_DIR},${LIB_LIBS_DIR},${LOBJECTS})

# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin

# build
build: .build-post
${ARDUINO_LIB_CORE}: ${CORE_OBJECTS}
	${CMD_AVR_AR} ${ARDUINO_LIB_CORE} ${CORE_OBJECTS}

${ARDUINO_LIB_LIBS}: ${LIB_OBJECTS}
	${CMD_AVR_AR} ${ARDUINO_LIB_LIBS} ${CORE_OBJECTS} ${LIB_OBJECTS}

libraries: ${ARDUINO_LIB_CORE} ${ARDUINO_LIB_LIBS}

${LIB_CORE_DIR}/%.cpp.o: ${ARDUINO_CORE_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CMD_AVR_GPP} $< -o $@

${LIB_CORE_DIR}/%.c.o: ${ARDUINO_CORE_DIR}/%.c
	mkdir -p $(dir $@)
	${CMD_AVR_GCC} $< -o $@

${LIB_LIBS_DIR}/%.cpp.o: ${ARDUINO_LIB_DIR}/%.cpp
	mkdir -p $(dir $@)
	${CMD_AVR_GPP} $< -o $@

${LIB_LIBS_DIR}/%.c.o: ${ARDUINO_LIB_DIR}/%.c
	mkdir -p $(dir $@)
	${CMD_AVR_GCC} $< -o $@

.build-pre: .build-pre-pre libraries

#Unconditional Build
.build-pre-pre:

.build-post: .build-impl
	avr-objcopy -O ihex ${CND_ARTIFACT_PATH_${CONF}} ${CND_ARTIFACT_PATH_${CONF}}.hex
	avr-size --mcu=${ARDUINO_MODEL} -C ${CND_ARTIFACT_PATH_${CONF}}

#upload: .build-post
upload:
	${AVR_DUDE} -v -p${ARDUINO_MODEL} -c${ARDUINO_PROGRAMMER} -P ${COM_PORT} -b${BAUD_RATE} -D -Uflash:w:${CND_ARTIFACT_PATH_${CONF}}.hex:i


# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...

.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre: build-tests
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...


# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
