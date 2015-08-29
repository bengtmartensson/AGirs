/*
Copyright (C) 2015 Bengt Martensson.

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
#include <Tokenizer.h>
#include <IrSenderNonMod.h>

#if defined(RECEIVE) & defined(CAPTURE)
#error only one of RECEIVE and CAPTURE may be defined!
#endif

#ifdef DECODER
#ifndef RECEIVE
#error DECODER without RECEIVE is nonsensical, aborting.
#endif
#include <MultiDecoder.h>
#endif

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

#ifdef TRANSMIT
#include <IrSenderPwm.h>
#endif

#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

LCD_DEFINE(lcd);
#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

#ifdef LED
#ifdef TRANSMITLED
static LED_PARAMETER_CONST pin_t transmitled = TRANSMITLED;
#endif

#ifdef RECEIVELED
static LED_PARAMETER_CONST pin_t receiveled = RECEIVELED;
#endif

#ifdef CAPTURELED
static LED_PARAMETER_CONST pin_t captureled = CAPTURELED;
#endif

#ifdef COMMANDLED
static LED_PARAMETER_CONST pin_t commandled = COMMANDLED;
#endif
#endif // LED

static const pin_t receiverNo = 1;

static PARAMETER_CONST unsigned long beginTimeout = DEFAULT_BEGINTIMEOUT; // milliseconds
#if defined(RECEIVE) || defined(CAPTURE)
static PARAMETER_CONST unsigned long endingTimeout = DEFAULT_ENDINGTIMEOUT; // milliseconds
#endif

#if defined(RECEIVE) | defined(CAPTURE)
static PARAMETER_CONST uint16_t captureSize = DEFAULT_CAPTURESIZE;
#endif

#define stream STREAM

#ifdef TRANSMIT

void sendIrSignal(uint16_t noSends, frequency_t frequency,
        uint16_t introLength, uint16_t repeatLength, uint16_t endingLength,
        const microseconds_t intro[], const microseconds_t repeat[], const microseconds_t ending[]) {
#ifdef TRANSMITLED
    setLogicLed(transmitled, HIGH);
#endif
    IrSender *irSender =
#ifdef NON_MOD
            (frequency == 0) ? (IrSender*) new IrSenderNonMod(NON_MOD_PIN) :
#endif
            (IrSender*) IrSenderPwm::getInstance(true);

    if (introLength > 0)
        irSender->send(intro, introLength, frequency);
    for (unsigned int i = 0; i < noSends - ((introLength > 0) ? 1U : 0U); i++)
        irSender->send(repeat, repeatLength, frequency);
    if (endingLength > 0)
        irSender->send(ending, endingLength, frequency);

#ifdef NON_MOD
    if (frequency == 0)
        delete irSender;
    else
#endif
        IrSenderPwm::deleteInstance();

#ifdef TRANSMITLED
    setLogicLed(transmitled, LOW);
#endif
}
#endif // TRANSMIT

#define modulesSupported EXPAND_AND_QUOTE(Base CAPTURE_NAME RECEIVE_NAME TRANSMIT_NAME TRANSMITTERS_NAME LCD_NAME LED_NAME)
#define PROGNAME "Girs4Lirc"
#define VERSION "2015-08-28"
#define okString "OK"
#define errorString "ERROR"
#define timeoutString "."

void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}

#ifdef LED
// First time, turn off all but commandled (was the self test)
static uint8_t initialized = 0;

static void allLedsOff() {
    for (uint8_t i = 1; i <= MAX_LED; i++) {
        if (!initialized)
            setLogicLed(i, LOW);
    }
    initialized = 1;
}
#endif

#ifdef RECEIVE

void receive() {
    IrReceiverSampler *irReceiver = IrReceiverSampler::getInstance();
    if (irReceiver == NULL)
        irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, RECEIVER2PIN(receiverNo), IRRECEIVER_PULLUP_VALUE(receiverNo));
    irReceiver->setEndingTimeout(endingTimeout);
    irReceiver->setBeginningTimeout(beginTimeout);
    irReceiver->setMarkExcess(IRRECEIVER_MARK_EXCESS);
    flushIn(stream);
#ifdef RECEIVELED
    setLogicLed(receiveled, HIGH);
#endif
    boolean interrupted = false;
    irReceiver->enable();
    while (!irReceiver->isReady() && !interrupted) {
        checkTurnoff();
        interrupted = stream.available();
    }
    irReceiver->disable();
#ifdef RECEIVELED
     setLogicLed(receiveled, LOW);
#endif
     if (interrupted)
	     return;

     if (irReceiver->getDataLength() < 2) {
         stream.println(F(timeoutString));
         return;
     }
     irReceiver->dump(stream);

#if defined(DECODER) & defined(LCD)
    MultiDecoder multiDecoder(*irReceiver);
    if (multiDecoder.getType() > MultiDecoder::noise) {
        lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
                ? F(".") : multiDecoder.getDecode(),
                multiDecoder.getType() != MultiDecoder::nec_ditto);
        if (multiDecoder.getType() == MultiDecoder::nec)
            lcd.setCursor(0, 1); // prepare for dittos
    }
#endif
}
#endif // RECEIVE

void setup() {
    DEFINE_IRRECEIVER;
    DEFINE_IRSENSOR;
    DEFINE_LEDS;
#if defined(TRANSMIT) | defined(RECEIVE)
    // Make sure that sender is quiet
    IrSenderPwm::getInstance(true)->mute();
#endif
    BLINK_ALL_LEDS; // as self test
#ifdef LCD
    LCD_INIT(lcd);
    lcdPrint(F(PROGNAME), true, 0, 0);
    lcdPrint(F(VERSION), false, 0, 1);
#endif

    stream.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    stream.println(F(PROGNAME " " VERSION));
    stream.setTimeout(serialTimeout);
}

// Process one command.
void loop() {
#ifdef COMMANDLED
    setLogicLed(commandled, HIGH);
#endif
    if (stream.available() == 0) {
#ifdef LED
        checkTurnoff();
	return;
#endif
    }
    String line = stream.readStringUntil(EOLCHAR);
#if defined(LCD) & defined(DEBUG)
    lcdPrint(F("Received:"), true, 0, 0);
    lcdPrint(line, false, 0, 1);
#endif
    Tokenizer tokenizer(line);
    String cmd = tokenizer.getToken();
#ifdef COMMANDLED
    setLogicLed(commandled, LOW);
#endif

    if (cmd.length() == 0) {
        // ok, do nothing
        stream.println(F(okString));
        return;
    }

    switch (cmd.charAt(0)) {

#if defined(LCD) | defined(LED)
        case 'l':
#ifdef LCD
            if (cmd.equals("lcd")) {
                lcdPrint(tokenizer.getLine(), true, 0, 0);
		lcdPrint(tokenizer.getLine(), false, 0, 1);
                stream.println(F(okString));
            } else
#endif // LCD

#ifdef LED
                if (cmd.equals("led")) {
                pin_t no = (pin_t) tokenizer.getInt();
                int8_t value = (int8_t) tokenizer.getInt();
                setLogicLed(no, value);
                stream.println(F(okString));
            } else
#endif // LED

                stream.println(F(errorString));
            break;
#endif

#ifdef RECEIVE
	case 'r':
	case 'a': // just if someone runs it from IrScrutinizer
		receive();
		break;
#endif // ! RECEIVE

	case 'm':
		stream.println(F(modulesSupported));
		break;

#ifdef TRANSMIT
        case 's': // send
            // TODO: handle unparsable data gracefully
        {
            uint16_t noSends = (uint16_t) tokenizer.getInt();
            frequency_t frequency = tokenizer.getFrequency();
            uint16_t introLength = (uint16_t) tokenizer.getInt();
            uint16_t repeatLength = (uint16_t) tokenizer.getInt();
            uint16_t endingLength = (uint16_t) tokenizer.getInt();
            microseconds_t intro[introLength];
            microseconds_t repeat[repeatLength];
            microseconds_t ending[endingLength];
            for (uint16_t i = 0; i < introLength; i++)
                intro[i] = tokenizer.getMicroseconds();
            for (uint16_t i = 0; i < repeatLength; i++)
                repeat[i] = tokenizer.getMicroseconds();
            for (uint16_t i = 0; i < endingLength; i++)
                ending[i] = tokenizer.getMicroseconds();
            sendIrSignal(noSends, frequency, introLength, repeatLength, endingLength, intro, repeat, ending); // waits
            stream.println(okString);
        }
            break;
#endif

#ifdef TRANSMITTERS
            // Just dummy "implementation"
        case 't':
            stream.println(okString);
            break;
#endif
        case 'v': // version
            stream.println(F(PROGNAME " " VERSION));
            break;
        default:
            stream.println(F(errorString));
    }
}
