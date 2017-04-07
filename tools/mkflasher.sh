#!/bin/sh

# This script creates another script,
# that will flash an Arduino nano
# on a system with avrdude installed.

ARDUINO=arduino
TMPDIR=/tmp/nanobuild
SKETCH=GirsLite
FILE=src/$SKETCH/$SKETCH.ino
PACKAGE=arduino
ARCHITECTURE=avr
BOARD=nano
CPU=atmega328
ZIP=zip
#VERBOSE=--verbose

VERSION=`grep 'define VERSION' src/$SKETCH/$SKETCH.cpp | sed -e 's/#define VERSION //' -e 's/"//g'`

OUTFILE=$SKETCH-$VERSION-$BOARD-flasher.sh

rm -rf $TMPDIR $OUTFILE

$ARDUINO  --verify --pref build.path=$TMPDIR ${VERBOSE} \
	  --board ${PACKAGE}:${ARCHITECTURE}:${BOARD}:cpu=${CPU} \
	  $FILE

cat > ${OUTFILE} <<EOF1
#!/bin/sh

# Flashing of the ${PACKAGE} ${BOARD}, using avrdude installed like Fedora.

if [ \$# -eq 1 ] ; then
    PORT=\$1
else
    PORT=/dev/ttyUSB0
fi

AVRDUDE=/usr/bin/avrdude
AVRDUDE_CONF=/etc/avrdude/avrdude.conf
PART=atmega328p
PROGRAMMER_ID=arduino
BAUD=57600

HEXFILE=/tmp/flasher\$\$

cat > \${HEXFILE} <<EOF
EOF1

cat $TMPDIR/$SKETCH.ino.hex >> ${OUTFILE}

cat >> ${OUTFILE} <<EOF2
EOF

\${AVRDUDE} -C\${AVRDUDE_CONF} \\
     -v -p\${PART} -c\${PROGRAMMER_ID} -P\${PORT} -b\${BAUD} -D \\
     -Uflash:w:\${HEXFILE}:i

rm -f \${HEXFILE}
EOF2

chmod +x ${OUTFILE}
cp $TMPDIR/$SKETCH.ino.hex $SKETCH-$VERSION-$BOARD.hex

# Github rejects files ending with .sh
$ZIP ${OUTFILE}.zip ${OUTFILE}
$ZIP $SKETCH-$VERSION-$BOARD.hex.zip $SKETCH-$VERSION-$BOARD.hex

echo Created $OUTFILE and ${OUTFILE}.zip
