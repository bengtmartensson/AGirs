/*
Copyright (C) 2014,2015 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include <Arduino.h>
#include "config.h"
#include <GirsMacros.h>

#ifdef ETHERNET
#error Ethernet not supported (yet?)
#endif

#define serialBaud 115200
#define serialTimeout 5000L

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

#include <IRLib.h>
#include <TrivialDecoder.h>

#ifdef DECODER
#include <MultiDecoder.h>
#endif

LCD_DEFINE(lcd);

#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

static const unsigned long beginTimeout = 10000000UL;
static const unsigned long endingTimeout = 35000UL;

IRrecv *irReceiver = NULL;
TrivialIrDecoder decoder;

#define PROGNAME "Listener"
#define VERSION "2015-07-05"

void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}

void receive(Stream& stream) {
    irReceiver->enableIRIn();
    // Wait until something arrives
    while (!(irReceiver->GetResults(&decoder))) {
        checkTurnoff();
    }
    // Setup decoder
    decoder.decode();
    // Do actual decode
    MultiDecoder multiDecoder(decoder);
#ifdef LCD
    if (multiDecoder.getType() > MultiDecoder::noise) {
        lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
                ? F(".") : multiDecoder.getDecode(),
                multiDecoder.getType() != MultiDecoder::nec_ditto);
        if (multiDecoder.getType() == MultiDecoder::nec)
            lcd.setCursor(0, 1); // prepare for dittos
    }
#endif
#ifdef DECODELED
    setLogicLed(DECODELED(multiDecoder.getType()), BLINK);
#endif
    stream.println(multiDecoder.getDecode());
    irReceiver->disableIRIn();
}

void setup() {
    DEFINE_IRRECEIVER;
    DEFINE_LEDS;
    BLINK_ALL_LEDS; // as self test
#ifdef LCD
    LCD_INIT(lcd);
    lcdPrint(F(PROGNAME), true, 0, 0);
    lcdPrint(F(VERSION), false, 0, 1);
#endif

    Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.println(F(PROGNAME " " VERSION));
    Serial.setTimeout(serialTimeout);

    irReceiver = new IRrecv(IRRECEIVER_PIN);
    irReceiver->setEndingTimeout(endingTimeout);
    irReceiver->setBeginningTimeout(beginTimeout);
    irReceiver->Mark_Excess = 50;
}

// Read one IR signal.
void loop() {
    receive(Serial);
}
