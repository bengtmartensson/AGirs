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
#include <avr/pgmspace.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#include <SPI.h>
#endif

// Girs modules to enable
#define TRANSMIT
#define RENDERER
#define CAPTURE
#define RECEIVE
#define DECODER
#define PARAMETERS
#define RESET
#define FREEMEM
#define LED

#ifdef RECEIVE
//#define IRRECEIVER_PIN 6
#define IRRECEIVER_PIN 5
#define IRRECEIVER_GND 6
#define IRRECEIVER_VSS 7
#endif

#ifdef LED
#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_2_GND A0
#define SIGNAL_LED_3 A3
#define SIGNAL_LED_3_GND A2
#define SIGNAL_LED_4 A5
#define SIGNAL_LED_4_GND A4
//#define SIGNAL_LED_1 2
//#define SIGNAL_LED_2 5
//#define SIGNAL_LED_3 13
#endif

#ifdef CAPTURE
#define SENSOR_GND 9
#define SENSOR_VSS 10
#endif

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

#ifdef RECEIVE
#include "IrReceiver.h"
#endif

#ifdef TRANSMIT
#include "IRLib.h"
#endif

#ifdef RENDERER
#include "IRLib.h"
#include "IrSignal.h"
#include "Nec1Renderer.h"
#include "Rc5Renderer.h"
#endif

#ifdef CAPTURE
#include "IrWidgetAggregating.h"
//#include "IrWidgetRaw.h"
#endif

#ifdef DECODER
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"
#endif

#ifdef TRANSMIT
#define TRANSMIT_NAME Transmit
#else
#define TRANSMIT_NAME
#endif

#ifdef CAPTURE
#define CAPTURE_NAME Capture
#else
#define CAPTURE_NAME
#endif

#ifdef RECEIVE
#define RECEIVE_NAME Receive
#else
#define RECEIVE_NAME
#endif

#ifdef RENDERER
#define RENDERER_NAME Renderer
#else
#define RENDERER_NAME
#endif

#ifdef DECODER
#define DECODER_NAME Decoder
#else
#define DECODER_NAME
#endif

#ifdef LED
#define LED_NAME Led
#else
#define LED_NAME
#endif

#ifdef PARAMETERS
#define PARAMETERS_NAME Parameters
#else
#define PARAMETERS_NAME
#endif

#define QUOTE(str) #str
#define EXPAND_AND_QUOTE(str) QUOTE(str)

#ifdef RECEIVE
void capture(IrReceiver *irReceiver, Stream& stream /***/) {
        while (!(/*stream != NULL &&*/ stream.available()) && !irReceiver->hasContent())
            ;
    }
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

#ifdef RENDERER
void sendIrSignal(IRsendRaw *irSender, unsigned int noSends, const IrSignal *signal) {
    sendIrSignal(irSender, noSends, signal->getFrequency(),
            signal->getLengthIntro(), signal->getLengthRepeat(), signal->getLengthEnding(),
        (unsigned int*) signal->getIntro(), (unsigned int*) signal->getRepeat(), (unsigned int*) signal->getEnding());
}
#endif
#endif

static const char modulesSupported[] PROGMEM = EXPAND_AND_QUOTE(Base TRANSMIT_NAME CAPTURE_NAME RENDERER_NAME RECEIVE_NAME DECODER_NAME LED_NAME PARAMETERS_NAME);
static const char versionString[] PROGMEM = "ArduinoGirsLite 2015-04-24";
static const char welcomeString[] PROGMEM = "Welcome to ArduinoGirs";
static const char okString[] PROGMEM = "OK";
static const char errorString[] PROGMEM = "ERROR";
static /*const*/ char separatorString[] = " ";
//static const char fEqualsString[] PROGMEM = "f=";
#ifdef RECEIVE
IrReceiver *irReceiver = NULL;
//IRrecv *irReceiver = NULL;
//unsigned int receiveSize = 201;
#endif
#ifdef TRANSMIT
IRsendRaw *irSender = NULL;
#endif
#ifdef CAPTURE
IrWidget *irWidget = NULL;
unsigned int captureSize = 201;
#endif
#if defined(RECEIVE) || defined(CAPTURE)
unsigned long endingTimeout =
#ifdef DECODER
// If using the decoder, be sure to end a capture before the repeat sequence.
33333L;
#else
111111L;
#endif
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

#ifdef FREEMEM
// http://playground.arduino.cc/Code/AvailableMemory#.U0EnzKogTzs
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

// given a PROGMEM string, use Stream.print() to send it out
void streamPrintProgStr(Stream &stream, const char PROGMEM str[]) {
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
#ifdef IRRECEIVER_GND
    pinMode(IRRECEIVER_GND, OUTPUT);
    digitalWrite(IRRECEIVER_GND, LOW);
#endif
#ifdef IRRECEIVER_VSS
    pinMode(IRRECEIVER_VSS, OUTPUT);
    digitalWrite(IRRECEIVER_VSS, HIGH);
#endif
#ifdef SENSOR_GND
     pinMode(SENSOR_GND, OUTPUT);
     digitalWrite(SENSOR_GND, LOW);
#endif
#ifdef SENSOR_VSS
     pinMode(SENSOR_VSS, OUTPUT);
     digitalWrite(SENSOR_VSS, HIGH);
#endif
#ifdef SIGNAL_LED_1
     pinMode(SIGNAL_LED_1, OUTPUT);
     digitalWrite(SIGNAL_LED_1, LOW);
#endif
#ifdef SIGNAL_LED_1_GND
     pinMode(SIGNAL_LED_1_GND, OUTPUT);
     digitalWrite(SIGNAL_LED_1_GND, LOW);
#endif
#ifdef SIGNAL_LED_2
     pinMode(SIGNAL_LED_2, OUTPUT);
     digitalWrite(SIGNAL_LED_2, LOW);
#endif
#ifdef SIGNAL_LED_2_GND
     pinMode(SIGNAL_LED_2_GND, OUTPUT);
     digitalWrite(SIGNAL_LED_2_GND, LOW);
#endif
#ifdef SIGNAL_LED_3
     pinMode(SIGNAL_LED_3, OUTPUT);
     digitalWrite(SIGNAL_LED_3, LOW);
#endif
#ifdef SIGNAL_LED_3_GND
     pinMode(SIGNAL_LED_3_GND, OUTPUT);
     digitalWrite(SIGNAL_LED_3_GND, LOW);
#endif
#ifdef SIGNAL_LED_4
     pinMode(SIGNAL_LED_4, OUTPUT);
     digitalWrite(SIGNAL_LED_4, LOW);
#endif
#ifdef SIGNAL_LED_4_GND
     pinMode(SIGNAL_LED_4_GND, OUTPUT);
     digitalWrite(SIGNAL_LED_4_GND, LOW);
#endif
     

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

#ifdef FREEMEM
        case 'i': // info
            gobble(stream);
            stream.println(freeRam());
            stream.println(RAMEND);
            break;
#endif

#ifdef LED
        case 'l': // led
        {

            stream.find(separatorString);
            int no = stream.parseInt();
            int value = stream.parseInt();
            gobble(stream);
            digitalWrite(
#ifdef SIGNAL_LED_4
              no == 4 ? SIGNAL_LED_4 :
#endif
#ifdef SIGNAL_LED_3
              no == 3 ? SIGNAL_LED_3 :
#endif
#ifdef SIGNAL_LED_2
              no == 2 ? SIGNAL_LED_2 :
#endif
              SIGNAL_LED_1, value ? HIGH : LOW);
        }
            break;
#endif // LED

#ifdef PARAMETERS
        case 'p': // parameter
        {
            stream.find(separatorString);
            String variableName = stream.readStringUntil(' ');
            long value = stream.parseInt();
            Serial.println(variableName);
            Serial.println(value);
            gobble(stream);
#if defined(RECEIVE) || defined(CAPTURE)
            if (variableName == "endingtimeout")
                endingTimeout = value;
            else
#endif
#ifdef CAPTURE
            if (variableName == "capturesize")
                captureSize = value;
            else
#endif
#ifdef RECEIVE
            //if (variableName == "receivesize")
            //    receiveSize = value;
            //else
#endif
            {
                streamPrintProgStr(stream, errorString);
                stream.println();
            }
        }
            break;
#endif // PARAMETERS

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

#ifdef RENDERER
        case 't': // transmit
            stream.find(separatorString);
        {
            IrSignal* signal = NULL;
            // TODO: handle silly data gracefully
            uint16_t noSends = stream.parseInt();
            stream.find(separatorString);
            String protocol = stream.readStringUntil(' ');
            protocol.toLowerCase();
            if (protocol == "nec1") {
                unsigned int D = stream.parseInt();
                unsigned int S;// = stream.parseInt();
                unsigned int F = stream.parseInt();
                S = 255-D;
                signal = Nec1Renderer::render(D, S, F);
            } else if (protocol == "rc5") {
                unsigned int D = stream.parseInt();
                unsigned int F = stream.parseInt();
                unsigned int T = 0U;//stream.parseInt();
                signal = Rc5Renderer::render(D, F, T);
            } else {
                streamPrintProgStr(stream, errorString);
                stream.println();
                signal = NULL;
            }
            gobble(stream);
            signal->dump(stream);
            if (signal != NULL) {
                if (irSender == NULL)
                    irSender = new IRsendRaw();

                //signal->dump(stream);

                sendIrSignal(irSender, noSends, signal);
            }
        }
            streamPrintProgStr(stream, okString);
            stream.println();
            break;
#endif // RENDERER

#ifdef RECEIVE
        case 'r': // receive
            gobble(stream);
        {
#ifdef CAPTURE
            if (irWidget != NULL) {
                delete irWidget;
                irWidget = NULL;
            }
#endif // CAPTURE
            if (irReceiver == NULL) {
                //irReceiver = new IrReceiver(IRRECEIVER_PIN, receiveSize, &stream);
                irReceiver = new IrReceiver(IRRECEIVER_PIN);
                irReceiver->setEndingTimeout(endingTimeout);
                irReceiver->Mark_Excess = 0;
                irReceiver->enableIRIn();
            } else {
                irReceiver->resume();
            }
            //irReceiver->reset();

            capture(irReceiver, stream);
            if (irReceiver->hasContent()) {
                irReceiver->dump(stream);
		stream.println();		
#ifdef DECODER
                Nec1Decoder nec1Decoder(*irReceiver);
                if (nec1Decoder.isValid())
                    stream.println(nec1Decoder.toString());
                else {
                    Rc5Decoder rc5Decoder(*irReceiver);
                    if (rc5Decoder.isValid())
                        stream.println(rc5Decoder.toString());
                    else
                        stream.println(F("No decode"));
                }
#endif
            } else
                stream.println(F("null (no content)"));
        }
            break;
#endif // RECEIVE

#ifdef CAPTURE
        case 'a': // analyze
            gobble(stream);
        {
#ifdef RECEIVE
            if (irReceiver != NULL) {
                delete irReceiver;
                irReceiver = NULL;
            }
#endif
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
        }
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
