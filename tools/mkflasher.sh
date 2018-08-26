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

# Flashing of the ${PACKAGE} ${BOARD}, using avrdude installed like Fedora.
# Sometimes root access is required, depending on the system configuration.

# Usage:
# ${OUTFILE} [-n] [<PORT>]
#
# where -n is for boards with the "new" bootloader, called optiboot.
# <PORT> is the device name, default /dev/ttyUSB0.

if [ "\$1" = "-n" ] ; then
    BAUD=115200
    shift
else
    BAUD=57600
fi

if [ \$# -eq 1 ] ; then
    PORT=\$1
else
    PORT=/dev/ttyUSB0
fi

AVRDUDE=/usr/bin/avrdude
AVRDUDE_CONF=/etc/avrdude/avrdude.conf
PART=atmega328p
PROGRAMMER_ID=arduino

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

# Github rejects files ending with .sh
$ZIP ${OUTFILE}.zip ${OUTFILE}
$ZIP $PRODUCT-$VERSION-$BOARD.hex.zip $PRODUCT-$VERSION-$BOARD.hex

echo Created $OUTFILE and ${OUTFILE}.zip
