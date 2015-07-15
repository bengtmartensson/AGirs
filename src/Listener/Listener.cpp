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
#include <IrReceiverSampler.h>

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

#ifdef DECODER
#include <MultiDecoder.h>
#endif

LCD_DEFINE(lcd);

#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

static const milliseconds_t beginTimeout = 10000U;
static const milliseconds_t endingTimeout = 35U;
static const int16_t captureSize = 200U;
static const microseconds_t markExcess = 50U;

IrReceiverSampler *irReceiver = NULL;

#define PROGNAME "Listener"
#define VERSION "2015-07-15"

void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}

#ifdef LED

static void allLedsOff() {
    for (uint8_t i = 1; i <= MAX_LED; i++)
        setLogicLed(i, LOW);
}
#endif

void receive(Stream& stream) {
    irReceiver->enable();
    // Wait until something arrives
    while (!irReceiver->isReady())
        checkTurnoff();

    MultiDecoder multiDecoder(*irReceiver);
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
    irReceiver->disable();
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

    irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, IRRECEIVER_PIN,
            IRRECEIVER_PIN_PULLUP_VALUE, markExcess, beginTimeout, endingTimeout);
}

// Read one IR signal.
void loop() {
    receive(Serial);
}
