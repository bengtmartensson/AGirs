/*
Copyright (C) 2018 Bengt Martensson.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see http://www.gnu.org/licenses/.
*/

#include "config.h"
#include "GirsLib/LedLcdManager.h"
#include "GirsLib/GirsUtils.h"
#include <IrReceiverSampler.h>
#include <IrSenderPwm.h>
#include <Nec1Decoder.h>
#include <Rc5Renderer.h>

#ifdef DEBUG
Stream& stream = Serial;
#endif

static IrReceiver *irReceiver = NULL;
static IrSender *sender = NULL;


#ifndef PROGNAME
#define PROGNAME "Translator"
#endif // ! PROGNAME
#ifndef VERSION
#include "GirsLib/version.h"
#endif // VERSION

static void transmit(const char *name, unsigned int D, unsigned int F, unsigned int count = 1) {
    LedLcdManager::lcdPrint("Command: " + String(name));
#ifdef DEBUG
    stream.println("Command: " + String(name));
#endif
    const IrSignal *signal = Rc5Renderer::newIrSignal(D, F);
#ifdef TRANSMITLED
    LedLcdManager::setLogicLed(TRANSMITLED, LedLcdManager::on);
#endif
    sender->sendIrSignal(*signal, count);
#ifdef TRANSMITLED
    LedLcdManager::setLogicLed(TRANSMITLED, LedLcdManager::off);
#endif
    delete(signal);
}

static void action() {
    Nec1Decoder decoder(*irReceiver);
    if (decoder.isValid() && !decoder.isDitto()) {
        int D = decoder.getD();
        int S = decoder.getS();
        int F = decoder.getF();
#ifdef DEBUG
        stream.println("NEC1 " + String(D) + "/" + String(S) + "/" + String(F));
#endif
        if (D == RECV_NEC1_DEVICE && S == RECV_NEC1_SUBDEVICE) {
            switch (decoder.getF()) {
                case RECV_VOLUME_UP_CMD:
                    transmit("UP", SEND_DEVICE, SEND_VOLUME_UP_CMD);
                    break;
                case RECV_VOLUME_DOWN_CMD:
                    transmit("DOWN", SEND_DEVICE, SEND_VOLUME_DOWN_CMD);
                    break;
                default:
                    LedLcdManager::lcdPrint("Unknown: " + String(decoder.getF()));
#ifdef DEBUG
                    stream.println("Unknown command: " + String(decoder.getF()));
#endif
                    break;
            }
        }
    }
}

void setup() {
    LedLcdManager::setupLedGroundPins();
    GirsUtils::setupReceivers();
    GirsUtils::setupLeds();
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(F(PROGNAME " " VERSION));
    LedLcdManager::lcdPrint(F("*************"), false, 0, 1);
#ifdef LED
    LedLcdManager::setupShouldTimeout(RECEIVELED, false);
    LedLcdManager::setupShouldTimeout(TRANSMITLED, false);
#endif

#ifdef DEBUG
    Serial.begin(SERIALBAUD);
    Serial.setTimeout(SERIALTIMEOUT);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    stream.println(F(PROGNAME " " VERSION));
#endif // DEBUG
    irReceiver = IrReceiverSampler::newIrReceiverSampler(CAPTURESIZE, IRRECEIVER_1_PIN,
            IRRECEIVER_1_PULLUP_VALUE, IRRECEIVER_MARK_EXCESS, BEGINTIMEOUT, ENDINGTIMEOUT);
    irReceiver->setEndingTimeout(ENDINGTIMEOUT);
    irReceiver->setBeginningTimeout(BEGINTIMEOUT);
    sender = IrSenderPwm::getInstance(true);
}

static void read() {
#ifdef RECEIVELED
    LedLcdManager::setLogicLed(RECEIVELED, LedLcdManager::on);
#endif
    irReceiver->enable();
    while (!irReceiver->isReady())
        LedLcdManager::checkTurnoff();
    irReceiver->disable();
#ifdef RECEIVELED
    LedLcdManager::setLogicLed(RECEIVELED, LedLcdManager::off);
#endif
}

static void loop() {
    read();
    action();
}
