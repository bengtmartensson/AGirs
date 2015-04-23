/*
Copyright (C) 2014 Bengt Martensson.

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
#include <Platform.h>
#include <avr/pgmspace.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <SPI.h>
#endif

// Girs modules to enable
#define TRANSMIT
#define CAPTURE
#define RESET

//#define ETHERNET
//#define DHCP

#ifdef ETHERNET
#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // DHCP
#define PORT       33333

#include <Ethernet.h>
#else
#define serialBaud 115200
//#define serialBaud 57600
#define serialTimeout 5000L
#endif

#ifdef TRANSMIT
#include "IRLib.h"
#endif

#ifdef CAPTURE
#include "IrWidgetAggregating.h"
#endif

#ifdef TRANSMIT
void sendIrSignal(IRsendRaw *irSender, unsigned int noSends, unsigned int frequency,
        unsigned int introLength, unsigned int repeatLength, unsigned int endingLength,
        /*const*/ unsigned int intro[], /*const*/ unsigned int repeat[], /*const*/ unsigned int ending[]) {
    if (introLength > 0)
        irSender->send(intro, introLength, frequency/1000);
    for (unsigned int i = 0; i < noSends - (introLength > 0); i++)
        irSender->send(repeat, repeatLength, frequency/1000);
    if (endingLength > 0)
        irSender->send(ending, endingLength, frequency/1000);
}
#endif

static const char modulesSupported[] PROGMEM = "Base Transmit Capture";
static const char versionString[] PROGMEM = "ArduinoGirsLite 2014-06-07";
static const char welcomeString[] PROGMEM = "Welcome to ArduinoGirs";
static const char okString[] PROGMEM = "OK";
static const char errorString[] PROGMEM = "ERROR";
static /*const*/ char separatorString[] = " ";

#ifdef TRANSMIT
IRsendRaw *irSender = NULL;
#endif
#ifdef CAPTURE
IrWidget *irWidget = NULL;
unsigned int captureSize = 201;
#endif
#if defined(RECEIVE) || defined(CAPTURE)
unsigned long endingTimeout = 100000;
#endif

#ifdef ETHERNET
EthernetServer server(PORT);
#endif

void gobble(Stream &stream) {
    char c;
    do {
        c = stream.read();
    } while (c != '\r' && c != '\n');
}

// given a PROGMEM string, use Stream.print() to send it out
void streamPrintProgStr(Stream &stream, const prog_char str[]) {
    if (!str)
        return;
    while (char c = pgm_read_byte(str++))
        stream.print(c);
}

#ifdef RESET
// Restarts program from beginning but does not reset the peripherals and registers
void softwareReset() {
    asm volatile("  jmp 0");
}
#endif

void setup() {
#ifdef ETHERNET
    byte mac[] = { MACADDRESS };
#ifdef DHCP
    Ethernet.begin(mac);
#else // !DHCP
    Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

    server.begin();
#else // !ETHERNET

    Serial.begin(serialBaud);
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
    streamPrintProgStr(Serial, welcomeString);
    Serial.println();
    Serial.setTimeout(serialTimeout);
#endif // !ETHERNET
}

boolean processCommand(Stream& stream) {
#ifdef ETHERNET
    boolean quit = false;
#endif
    char ch = -1;
    while (ch == -1)
        ch = stream.read();
    switch (ch) {
        case 'v': // version
            gobble(stream);
            streamPrintProgStr(stream, versionString);
            stream.println();
            break;

        case 'm': // modules
            gobble(stream);
            streamPrintProgStr(stream, modulesSupported);
            stream.println();
            break;

#ifdef RESET
        case 'R': // reset
            gobble(stream);
            softwareReset();
            break;
#endif

#ifdef ETHERNET
        case 'q': // quit
            gobble(stream);
            quit = true;
            break;
#endif

#ifdef TRANSMIT
        case 's': // send
            stream.find(separatorString);
        {
            // TODO: handle silly data gracefully
            uint16_t noSends = stream.parseInt();
            uint16_t frequency = stream.parseInt();
            uint16_t introLength = stream.parseInt();
            uint16_t repeatLength = stream.parseInt();
            uint16_t endingLength = stream.parseInt();
            uint16_t intro[introLength];
            uint16_t repeat[repeatLength];
            uint16_t ending[endingLength];
            // FIXME: Should take care of overflows, if a duration > 65536us
            for (uint16_t i = 0; i < introLength; i++)
                intro[i] = stream.parseInt();
            for (uint16_t i = 0; i < repeatLength; i++)
                repeat[i] = stream.parseInt();
            for (uint16_t i = 0; i < endingLength; i++)
                ending[i] = stream.parseInt();
            gobble(stream);
            if (irSender == NULL)
                irSender = new IRsendRaw();
            sendIrSignal(irSender, noSends, frequency, introLength, repeatLength, endingLength, intro, repeat, ending);
        }
            streamPrintProgStr(stream, okString);
            stream.println();
            break;
#endif // TRANSMIT

#ifdef CAPTURE
        case 'a': // analyze
            gobble(stream);
            if (irWidget == NULL) {
                irWidget = new IrWidgetAggregating(captureSize, &stream);
                irWidget->setEndingTimeout(endingTimeout);
            }
            irWidget->reset();
            irWidget->capture();

            if (irWidget->hasContent()) {
                irWidget->dump(stream);
                delay(10);
            } else
                stream.println(F("null"));
            break;
#endif // CAPTURE

        case '\n':
        case '\r':
            streamPrintProgStr(stream, okString);
            stream.println();
            break;

        default:
            gobble(stream);
            streamPrintProgStr(stream, errorString);
            stream.println();
    }

    // Junk pending line feeds
    char c;
    while (true) {
        c = stream.peek();
        if (c != '\n' && c != '\r')
            break;
        stream.read();
    }
#ifdef ETHERNET
    return !quit;
#else
    return true;
#endif
}

void loop() {
#ifdef ETHERNET
    EthernetClient client = server.available();
    if (!client)
        return;

    streamPrintProgStr(client, welcomeString);
    client.println();
    boolean goOn;
    do
        goOn = processCommand(client);
    while (goOn);
    client.println("Bye");
    client.flush();
    client.stop();
#else
    processCommand(Serial);
#endif
}