/*
Copyright (C) 2015 Bengt Martensson.

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

/*
 * This is more of an example than a program that is expected to be useful as-is.
 * It is a sort-of IR to serial translator.
 * It listens to an IR receiver, and transmits serial commands to an OPPO BluRay (or DVD) player,
 * (see http://download.oppodigital.com/BDP103/BDP103_RS232_Protocol_v1.2.2.pdf)
 * It can also issue query commands, and display the result on an LCD screen.
 */

#include "config.h"
#include <Arduino.h>
#include <IrReceiverSampler.h>
#include <Nec1Decoder.h>
#include "GirsLib/LedLcdManager.h"
#include "GirsLib/GirsUtils.h"

#ifndef LCD
#error LCD is presently required
#endif

#ifdef ARDUINO
//#ifdef ARDUINO_AVR_MEGA2560
//Stream& stream = Serial1;
//#else  // !ARDUINO_AVR_MEGA2560
Stream& stream = Serial;
//#endif // ! ARDUINO_AVR_MEGA2560
#else  // ! ARDUINO
Stream stream(std::cout);
#endif // ! ARDUINO

#ifndef PROGNAME
#define PROGNAME "Opponator"
#endif
#ifndef VERSION
#include "GirsLib/version.h"
#endif

static IrReceiverSampler* irReceiver;

void send(String payload) {
    stream.print("#" + payload + "\r");
}

void sendReceiveDisplay(String payload, String title) {
    send(payload);
    LedLcdManager::lcdPrint(title, true);
#ifdef ARDUINO
    String answer = stream.readStringUntil('\r');
    answer.trim();
    LedLcdManager::lcdPrint(answer, false, 0, 1);
#endif
}

void action(IrReader *irReader) {
    Nec1Decoder decoder(*irReader);
    if (decoder.isValid() && ! decoder.isDitto()
            && decoder.getD() == selectedD
            && decoder.getS() == selectedS) {
        LedLcdManager::lcdPrint("Signal " +
#ifdef ARDUINO
                String(decoder.getF())
#else
                std::to_string(decoder.getF())
#endif
                );
        switch (decoder.getF()) {
            case 6: // Play
                send("PLA");
                break;
            case 86://14: // Power
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
    }
#ifdef DEBUG
    else {
        irReader->dump(stream);
        stream.println(decoder.isValid() ? "valid" : "invalid");
        stream.println(decoder.getD());
        stream.println(decoder.getS());
        stream.println(decoder.getF());
    }
#endif
}

void loop() {
    irReceiver->enable();
    while (!irReceiver->isReady()) {
        LedLcdManager::checkTurnoff();
    }
    irReceiver->disable();
    action(irReceiver);
}

void setup() {
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
    GirsUtils::setupReceivers();
    LedLcdManager::lcdPrint(F(PROGNAME));
    LedLcdManager::lcdPrint(F(VERSION), false, 0, 1);

#ifdef ARDUINO
    Serial.begin(serialBaud);
#endif

    irReceiver = IrReceiverSampler::newIrReceiverSampler(IrReader::defaultCaptureLength,
            IRRECEIVER_1_PIN, IRRECEIVER_1_PULLUP_VALUE, IRRECEIVER_MARK_EXCESS, DEFAULT_BEGINTIMEOUT, DEFAULT_ENDINGTIMEOUT);
}

#ifndef ARDUINO
int main() {
    setup();
    while (true)
        loop();
}
#endif
