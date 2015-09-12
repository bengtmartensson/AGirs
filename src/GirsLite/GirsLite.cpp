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
#include <Tokenizer.h>
#include <IrSenderNonMod.h>

#if defined(RECEIVE) & defined(CAPTURE)
#error only one of RECEIVE and CAPTURE may be defined!
#endif

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

#ifdef TRANSMIT
#include <IrSenderPwm.h>
#endif

#ifdef CAPTURE
#include <IrWidgetAggregating.h>
#endif

#include <LedFuncs.inc> // Must come after lcd

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

#define modulesSupported EXPAND_AND_QUOTE(Base Capture TRANSMIT_NAME)
#define PROGNAME "GirsLite"
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
        if (!(
#ifdef TRANSMITLED
                (initialized && i == transmitled) ||
#endif
#ifdef CAPTURELED
                (initialized && i == captureled) ||
#endif
#ifdef RECEIVELED
                (initialized && i == receiveled) ||
#endif
#ifdef COMMANDLED
                i == commandled ||
#endif
                false))
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
     IrReceiverSampler::deleteInstance();
}
#endif // RECEIVE

#ifdef CAPTURE

void capture() {
    IrWidget *irWidget = IrWidgetAggregating::getInstance();
    if (irWidget == NULL)
        irWidget = IrWidgetAggregating::newIrWidgetAggregating(captureSize, stream);
    irWidget->setEndingTimeout(endingTimeout);
    irWidget->setBeginningTimeout(beginTimeout);
    irWidget->reset();
#ifdef CAPTURELED
    setLogicLed(captureled, HIGH);
#endif
    flushIn(stream);
    irWidget->capture();
#ifdef CAPTURELED
    setLogicLed(captureled, LOW); // FIXME
#endif
    if (irWidget->isReady())
        irWidget->dump(stream);
    else
        stream.println(F(timeoutString));
}
#endif // CAPTURE

void setup() {
    DEFINE_IRRECEIVER;
    DEFINE_IRSENSOR;
    DEFINE_LEDS;
#if defined(TRANSMIT) | defined(RECEIVE)
    // Make sure that sender is quiet
    IrSenderPwm::getInstance(true)->mute();
#endif
    BLINK_ALL_LEDS; // as self test

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
        case 'a':
        case 'c':
        case 'r':
#ifdef RECEIVE
            receive();
#else
            capture();
#endif // ! RECEIVE
            break;
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
        case 'v': // version
            stream.println(F(PROGNAME " " VERSION));
            break;
        default:
            stream.println(F(errorString));
    }
}
