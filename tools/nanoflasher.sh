#!/bin/sh

# Flashing of the Arduino Nano, using avrdude installed like Fedora.

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
BAUD=57600

HEXFILE=$1

${AVRDUDE} -C${AVRDUDE_CONF} \
     -v -p${PART} -c${PROGRAMMER_ID} -P${PORT} -b${BAUD} -D \
     -Uflash:w:${HEXFILE}:i
