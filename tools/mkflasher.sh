#!/bin/sh

# This script creates another script,
# that will flash an Arduino nano
# on a system with avrdude installed.

ARDUINO=arduino
TMPDIR=/tmp/nanobuild
SKETCH=Girs
PRODUCT=GirsLite
FILE=examples/$SKETCH/$SKETCH.ino
PACKAGE=arduino
ARCHITECTURE=avr
BOARD=nano
CPU=atmega328
ZIP=zip
#VERBOSE=--verbose

VERSION=`grep 'version=' library.properties | sed -e 's/version=//'`

OUTFILE=$PRODUCT-$VERSION-$BOARD-flasher.sh
OUTFILE_BAT=$PRODUCT-$VERSION-$BOARD-flasher.bat

rm -rf $TMPDIR $OUTFILE

# I _think_ that this command requires AGirs at least to be accessible
# in the standard Arduino path, for example ~/Arduino/libraries.
$ARDUINO  --verify --pref build.path=$TMPDIR ${VERBOSE} \
	  --board ${PACKAGE}:${ARCHITECTURE}:${BOARD}:cpu=${CPU} \
	  $FILE

if [ "$?" -ne 0 ] ; then
    exit $?
fi

cat > ${OUTFILE} <<EOF1
#!/bin/sh

# Flashing of the ${PACKAGE} ${BOARD}, using avrdude.
# Sometimes root access is required, depending on the system configuration.

# Usage:
# ${OUTFILE} [-o] [<PORT>]
#
# where -o is for boards with the old bootloader.
# <PORT> is the device name, default /dev/ttyUSB0.

if [ "\$1" = "-o" ] ; then
    BAUD=57600
    shift
else
    BAUD=115200
fi

if [ \$# -eq 1 ] ; then
    PORT=\$1
else
    PORT=/dev/ttyUSB0
fi

# Linux defaults
AVRDUDE=/usr/bin/avrdude
AVRDUDE_CONF=/etc/avrdude/avrdude.conf
PART=atmega328p
PROGRAMMER_ID=arduino

# Mac defaults
if [ $(uname -s) == Darwin ]; then
  AVRDUDE=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude
  AVRDUDE_CONF=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf
fi

HEXFILE=/tmp/flasher\$\$

cat > \${HEXFILE} <<EOF
EOF1

cat $TMPDIR/$SKETCH.ino.hex >> ${OUTFILE}

cat >> ${OUTFILE} <<EOF2
EOF

if [ -f \${AVRDUDE_CONF} ] ; then
   AVRDUDE_CONF_OPTION=-C\${AVRDUDE_CONF}
else
   AVRDUDE_CONF_OPTION=
fi

\${AVRDUDE} \${AVRDUDE_CONF_OPTION} \\
     -v -p\${PART} -c\${PROGRAMMER_ID} -P\${PORT} -b\${BAUD} -D \\
     -Uflash:w:\${HEXFILE}:i

rm -f \${HEXFILE}
EOF2

chmod +x ${OUTFILE}
cp $TMPDIR/$SKETCH.ino.hex $PRODUCT-$VERSION-$BOARD.hex

cat > ${OUTFILE_BAT} << EOF3
@echo off
REM This bat-file can be used for flashing of $PRODUCT-$VERSION
REM to the Arduino Nano.
REM It uses avrdude, for example installed by Arduino.

REM Use the actual port as argument to the file, or enter it at the prompt.

REM Usage:
REM ${OUTFILE_BAT} [<PORT>]

REM Use 115200 for the new bootloader ("optiboot"), 57600 for old bootloader.
set BAUD=115200
REM set BAUD=57600
set PORT=

if "%1" == "" (
   set /p PORT="Enter port for the Arduino (default COM5): "
) else (
   set PORT=%1
)

if "%PORT%" == "" (
    set PORT=COM5
)

set HEXFILE=%TEMP%\data.hex

> %HEXFILE% (
EOF3

while read LINE; do
    echo echo `echo $LINE | sed -e 's/\\r//'`>> ${OUTFILE_BAT}
done < $TMPDIR/$SKETCH.ino.hex

cat >> ${OUTFILE_BAT} << EOF4
)

set ARDUINO_ROOT=C:\Program Files\Arduino
set TOOLS_ROOT=%ARDUINO_ROOT%\hardware\tools\avr
set AVRDUDE=%TOOLS_ROOT%\bin\avrdude
set AVRDUDE_CONF=%TOOLS_ROOT%\etc\avrdude.conf
set PART=atmega328p
set PROGRAMMER_ID=arduino

"%AVRDUDE%" -C "%AVRDUDE_CONF%" -v -p %PART% -c %PROGRAMMER_ID% -P %PORT% -b %BAUD% -D -U flash:w:"%HEXFILE%":i

pause
del "%HEXFILE%"
EOF4

# Github rejects files ending with .sh
$ZIP ${OUTFILE}.zip ${OUTFILE}
$ZIP ${OUTFILE_BAT}.zip ${OUTFILE_BAT}
$ZIP $PRODUCT-$VERSION-$BOARD.hex.zip $PRODUCT-$VERSION-$BOARD.hex

echo Created $OUTFILE, ${OUTFILE}.zip, ${OUTFILE_BAT} and ${OUTFILE_BAT}.zip
