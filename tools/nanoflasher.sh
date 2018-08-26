#!/bin/sh

# Flashing of the Arduino Nano, using avrdude installed like Fedora.

# Usage:
# ${OUTFILE} [-n] [<PORT>]
#
# where -n is for boards with the "new" bootloader, called optiboot.
# <PORT> is the device name, default /dev/ttyUSB0.

if [ "$1" = "-n" ] ; then
    BAUD=115200
    shift
else
    BAUD=57600
fi

if [ $# -eq 1 ] ; then
    PORT=/dev/ttyUSB0
elif [ $# -eq 2 ] ; then
    PORT=$2
else
    echo -e usage: \\n\\t$0 hexfile [device]
    exit 1
fi

AVRDUDE=/usr/bin/avrdude
AVRDUDE_CONF=/etc/avrdude/avrdude.conf
PART=atmega328p
PROGRAMMER_ID=arduino

HEXFILE=$1

if [ -f ${AVRDUDE_CONF} ] ; then
   AVRDUDE_CONF_OPTION=-C${AVRDUDE_CONF}
fi

${AVRDUDE} ${AVRDUDE_CONF_OPTION} \
     -v -p${PART} -c${PROGRAMMER_ID} -P${PORT} -b${BAUD} -D \
     -Uflash:w:${HEXFILE}:i
