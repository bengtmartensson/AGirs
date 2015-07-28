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

/*
 * This is more of an example than a program that is expected to be useful as-is.
 * It is a sort-of IR to serial translator.
 * It listens to an IR receiver, and transmits serial commands to an OPPO BluRay (or DVD) player,
 * (see http://download.oppodigital.com/BDP103/BDP103_RS232_Protocol_v1.2.2.pdf)
 * It can also issue query commands, and display the result on an LCD screen.
 */
#include <Arduino.h>
#include "config.h"
#include <GirsMacros.h>

#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

#include <IrReceiverSampler.h>
#include <Nec1Decoder.h>

#define SERIAL Serial1

#define selectedD 114
#define selectedS 205

LCD_DEFINE(lcd);

#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

#define PROGNAME "Opponator"
#define VERSION "2015-07-25"

static void allLedsOff() {}

void send(String payload) {
    SERIAL.print("#" + payload + "\r");
}

void sendReceiveDisplay(String payload, String title) {
    send(payload);
    String answer = SERIAL.readStringUntil('\r');
    answer.trim();
    lcdPrint(title, true, 0, 0);
    lcdPrint(answer, false, 0, 1);
}

void action(IrReader *irReader) {
    Nec1Decoder decoder(*irReader);
    if (decoder.isValid() && ! decoder.isDitto()
            && decoder.getD() == selectedD
            && decoder.getS() == selectedS) {
        lcdPrint("Signal " + String(decoder.getF()), true, 0, 0);
        switch (decoder.getF()) {
            case 6: // Play
                send("PLA");
                break;
            case 14: // Power
                send("POW");
                break;
            case 18: // Goto
                sendReceiveDisplay("QEL", "Tot elapsed");
                break;
            case 26: // Eject
                send("EJT");
                break;
            case 64: // Title
                sendReceiveDisplay("QTK", "Title playing");
                break;
            case 80: // Display
                sendReceiveDisplay("QVR", "FW version"); // Query firmware version
                break;
            case 92: // ENTER
                break;
            default:
                // ...
                break;
        }
    } /*else
        lcdPrint("Unknown signal", true, 0, 0);*/
}

void loop() {
    IrReceiverSampler *irReceiver = IrReceiverSampler::getInstance();
    irReceiver->enable();
    while (!irReceiver->isReady()) {
        checkTurnoff();
    }
    irReceiver->disable();
    action(irReceiver);
}

void setup() {
    DEFINE_IRRECEIVER;
#ifdef LCD
    LCD_INIT(lcd);
    lcdPrint(F(PROGNAME), true, 0, 0);
    lcdPrint(F(VERSION), false, 0, 1);
#endif
    SERIAL.begin(9600);
    IrReceiverSampler *irReceiver = IrReceiverSampler::newIrReceiverSampler(DEFAULT_CAPTURESIZE, IRRECEIVER_1_PIN, IRRECEIVER_1_PULLUP_VALUE);
    irReceiver->setEndingTimeout(DEFAULT_ENDINGTIMEOUT);
    irReceiver->setBeginningTimeout(DEFAULT_BEGINTIMEOUT);
    irReceiver->setMarkExcess(IRRECEIVER_MARK_EXCESS);
}
