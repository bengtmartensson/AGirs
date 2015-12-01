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

#include "config.h"
#include <defineMissingStuff.h>
#include <GirsUtils.h>
#include <IrReceiverSampler.h>

#ifdef ARDUINO

#else // ! ARDUINO

// Define some dummy stuff to be able to compile and test
#if 0
#define SIGNAL_LED_1     13
#define SIGNAL_LED_2     101
#define SIGNAL_LED_3     102
#define SIGNAL_LED_4     103

#define SIGNAL_LED_5_GND 22
#define SIGNAL_LED_5     23
#define SIGNAL_LED_6_GND 24
#define SIGNAL_LED_6     25
#define SIGNAL_LED_7_GND 26
#define SIGNAL_LED_7     27
#define SIGNAL_LED_8_GND 28
#define SIGNAL_LED_8     29
#define IRRECEIVER_1_PIN 5
#define IRRECEIVER_1_GND 6
#define IRRECEIVER_1_VSS 7
#endif

#endif // ! ARDUINO

#ifdef ETHERNET
#error ETHERNET not yet supported
#endif

static const long serialBaud = 115200L;
static const long serialTimeout = 5000L;
static const milliseconds_t beginTimeout = 10000U;
static const milliseconds_t endingTimeout = 35U;
static const int16_t captureSize = 200U;
static const microseconds_t markExcess = 50U;

IrReceiver *irReceiver = NULL;

#define PROGNAME "Listener"
#define VERSION "2015-12-01"

#ifdef ARDUINO
void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}
Stream& stream = Serial;
#else
Stream stream(std::cout);
#endif

// Read and process one signal (or timeout).
void loop() {
    irReceiver->enable();
    // Wait until something arrives
    while (!irReceiver->isReady())
        LedLcdManager::checkTurnoff();

    MultiDecoder multiDecoder(*irReceiver);
#ifdef LCD
    if (multiDecoder.getType() > MultiDecoder::noise) {
        LedLcdManager::lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
                ? "." : multiDecoder.getDecode(),
                multiDecoder.getType() != MultiDecoder::nec_ditto);
        if (multiDecoder.getType() == MultiDecoder::nec)
            LedLcdManager::lcdSetCursor(0, 1); // prepare for dittos
    }
#endif
    LedLcdManager::setLogicLed(GirsUtils::decode2logicalLed(multiDecoder.getType()), LedLcdManager::blink);
    stream.println(multiDecoder.getDecode());
    irReceiver->disable();
}

void setup() {
    GirsUtils::setupReceivers();
    GirsUtils::setupLeds();
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(F(PROGNAME "\n" VERSION));

#ifdef ARDUINO
    Serial.begin(serialBaud);
    Serial.setTimeout(serialTimeout);
#endif
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    stream.println(F(PROGNAME " " VERSION));

    irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, IRRECEIVER_1_PIN,
            IRRECEIVER_1_PULLUP_VALUE, markExcess, beginTimeout, endingTimeout);
}

#ifndef ARDUINO
int main() {
    setup();
    while (true)
        loop();
}
#endif
