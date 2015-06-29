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
#ifdef ETHER_ENC28J60
#error ETHER_ENC28J60 not yet supported, only W5100.
#else
#include <Ethernet.h>
#endif
#endif // ETHERNET
#if !defined(ETHERNET) | defined(SERIAL_DEBUG)
#define serialBaud 115200
#define serialTimeout 5000L
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

#if defined(RECEIVE) | defined(TRANSMIT)
#include <IRLib.h>
#endif
#if defined(RECEIVE)
#include <TrivialDecoder.h>
#endif

#ifdef CAPTURE
#include "IrWidgetAggregating.h"
#endif

#ifdef DECODER
#include <MultiDecoder.h>
#endif

#ifdef RENDERER
#include <IrSignal.h>
#include <Nec1Renderer.h>
#include <Rc5Renderer.h>
#endif

LCD_DEFINE(lcd);

#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

static PARAMETER_CONST unsigned long beginTimeout = 10000000UL;
#if defined(RECEIVE) || defined(CAPTURE)
static PARAMETER_CONST unsigned long endingTimeout =
#ifdef DECODER
// If using the decoder, be sure to end a capture before the repeat sequence.
30000L;
#else
100000L;
#endif
#endif

#ifdef RECEIVE
IRrecv *irReceiver = NULL;
TrivialIrDecoder decoder;
#endif
#if defined(TRANSMIT) | defined(RENDERER)
IRsendRaw *irSender = NULL;
#endif
#ifdef CAPTURE
IrWidget *irWidget = NULL;
static PARAMETER_CONST unsigned int captureSize = 201;
#endif

#ifdef ETHERNET
#ifdef USEUDP
EthernetUDP udp;
#else // !USEUDP
#ifdef SERVER
EthernetServer server(PORT);
#else
IPAddress peer(PEER_IP);
#endif
#endif // !USEUDP
#endif // ETHERNET

#if defined(TRANSMIT) | defined(RENDERER)

void sendIrSignal(IRsendRaw *irSender, unsigned int noSends, unsigned int frequency,
        unsigned int introLength, unsigned int repeatLength, unsigned int endingLength,
        /*const*/ unsigned int intro[], /*const*/ unsigned int repeat[], /*const*/ unsigned int ending[]) {
#ifdef TRANSMITLED
    setLogicLed(TRANSMITLED, HIGH);
#endif
    if (introLength > 0)
        irSender->send(intro, introLength, frequency / 1000);
    for (unsigned int i = 0; i < noSends - (introLength > 0); i++)
        irSender->send(repeat, repeatLength, frequency / 1000);
    if (endingLength > 0)
        irSender->send(ending, endingLength, frequency / 1000);
#ifdef TRANSMITLED
    setLogicLed(TRANSMITLED, LOW);
#endif
}
#endif

#ifdef RENDERER
void sendIrSignal(IRsendRaw *irSender, unsigned int noSends, const IrSignal *signal) {
    sendIrSignal(irSender, noSends, signal->getFrequency(),
            signal->getLengthIntro(), signal->getLengthRepeat(), signal->getLengthEnding(),
        (unsigned int*) signal->getIntro(), (unsigned int*) signal->getRepeat(), (unsigned int*) signal->getEnding());
}
#endif

#define modulesSupported EXPAND_AND_QUOTE(Base TRANSMIT_NAME CAPTURE_NAME RENDERER_NAME RECEIVE_NAME DECODER_NAME LED_NAME LCD_NAME PARAMETERS_NAME)
#define PROGNAME "ArduinoGirs"
#define VERSION "2015-06-29"
#define welcomeString "Welcome to " PROGNAME
#define okString "OK"
#define errorString "ERROR"
#define timeoutString "."
#define versionString PROGNAME " " VERSION
static /*const*/ char separatorString[] = " ";

void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}

#ifdef FREEMEM
// http://playground.arduino.cc/Code/AvailableMemory#.U0EnzKogTzs
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

#ifdef RESET
// TODO: While this works on atmega386 and atmega2560,
// I suspect that it does not on atmega32/Leonardo/Micro. Verify.

boolean reset = false;

// Restarts program from beginning but does not reset the peripherals and registers
void softwareReset() {
    asm volatile("  jmp 0");
}
#endif

#ifdef RECEIVE

void dump(IRdecodeBase& decoder, Stream& stream) {
    // First entry is introductory silence, therefore start at 1, not 0
    for (unsigned int i = 1; i < decoder.rawlen; i++) {
        stream.write((i & 0x01) ? '+' : '-');
        stream.print(decoder.rawbuf[i], DEC);
        stream.print(" ");
    }
    stream.println();
}

void receive(Stream& stream) {
#ifdef CAPTURE
    if (irWidget != NULL) {
        delete irWidget;
        irWidget = NULL;
    }
#endif // CAPTURE
    if (irReceiver == NULL)
        irReceiver = new IRrecv(IRRECEIVER_PIN);
    irReceiver->setEndingTimeout(endingTimeout);
    irReceiver->setBeginningTimeout(beginTimeout);
    irReceiver->Mark_Excess = 50;
    irReceiver->enableIRIn();
    //irReceiver->resume();
#ifdef RECEIVELED
    setLogicLed(RECEIVELED, HIGH);
#endif
    while (!(irReceiver->GetResults(&decoder))) {
        checkTurnoff();
        //if (stream.available()) {
        //    return false;
        //}
    }
#ifdef RECEIVELED
     setLogicLed(RECEIVELED, LOW);
#endif
    // Setup decoder
    decoder.decode();
#if defined(DECODER)
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
    switch (multiDecoder.getType()) {
        case MultiDecoder::noise:
            // ignore
            break;
        case MultiDecoder::undecoded:
            dump(decoder, stream); // report data of undecoded signals
            break;
        default:
            stream.println(multiDecoder.getDecode()); // also for timeout
            break;
    }
    irReceiver->disableIRIn();
#else // ! DECODE
    dump(decoder, stream);
#endif // !DECODE
}
#endif // RECEIVE

#ifdef CAPTURE

void capture(Stream& stream) {
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
#ifdef CAPTURELED
    setLogicLed(CAPTURELED, HIGH);
#endif
    irWidget->capture();
#ifdef CAPTURELED
    setLogicLed(CAPTURELED, LOW); // FIXME
#endif
    if (irWidget->hasContent()) {
        // No point in decoding, that is what "receive" is for.
        irWidget->dump(stream);
    } else
        stream.println(F("null"));
}
#endif // CAPTURE

void setup() {
    DEFINE_IRRECEIVER;
    DEFINE_IRSENSOR;
    DEFINE_LEDS;
    BLINK_ALL_LEDS; // as self test
#ifdef LCD
    LCD_INIT(lcd);
    lcdPrint(F(PROGNAME), true, 0, 0);
    lcdPrint(F(VERSION), false, 0, 1);
#ifdef ETHERNET
#ifdef USEUDP
    lcdPrint(F("UDP"), false, 0, 2);
#else
    lcdPrint(F("TCP"), false, 0, 2);
#endif
#ifdef SERVER
    lcdPrint(F(" Srv"), false);
#else
    lcdPrint(" " + String(peer[0], DEC) + "." + String(peer[1], DEC) + "."
            + String(peer[2], DEC) + "." + String(peer[3], DEC) + "@" + String(PEER_PORT), false);
#endif
#ifdef SERIAL_DEBUG
    lcdPrint(F(" SerialDbg"), false);
#endif
#else // ! ETHERNET
    lcdPrint(F("Serial"), false, 0, 2);
#endif // ! ETHERNET
#endif // LCD

#if defined(TRANSMIT) | defined(RECEIVE)
    // Make sure that sender is quiet
    IRsendBase::No_Output();
#endif

#ifdef ETHERNET
#ifdef SDCARD_ON_ETHERSHIELD
    // disable the SD card, as recommended in the doc
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
#endif
#ifdef ARDUINO_AVR_MEGA2560
    //pinMode(53, OUTPUT);
    //digitalWrite(53, LOW);
#endif
    byte mac[] = {MACADDRESS};
#ifdef DHCP
    Ethernet.begin(mac);
#else // !DHCP
    Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

#ifdef USEUDP
#ifndef SERVER
#error Client mode for UDP not implementd
#endif
    udp.begin(PORT);
#else
#ifdef SERVER
    server.begin();
#endif // SERVER
#endif // USEUDP
#endif // ETHERNET

#if !defined(ETHERNET) | defined(SERIAL_DEBUG)

    Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.println(F(versionString));
    Serial.setTimeout(serialTimeout);
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)
}

// Process one command.
boolean work(Stream& stream) {
#ifdef ETHERNET_SESSION
    boolean quit = false;
#endif
    char ch = -1;
#ifdef COMMANDLED
    setLogicLed(COMMANDLED, HIGH);
#endif
    while (ch == -1) {
#ifdef LED
        checkTurnoff();
#endif
        ch = stream.read(); // Blocks... FIXME
    }
#ifdef COMMANDLED
    setLogicLed(COMMANDLED, LOW);
#endif

    switch (ch) {

#ifdef CAPTURE
        case 'a': // analyze
            flushIn(stream);
            capture(stream);
            break;
#endif // CAPTURE

#ifdef FREEMEM
        case 'i': // info
            flushIn(stream);
            stream.println(freeRam());
            stream.println(RAMEND);
            break;
#endif

#ifdef LCD
        case 'L': //LCD
        {
            stream.find(separatorString);
            String s = stream.readStringUntil('\n');
            s.trim();
            lcdPrint(s, true, 0, 0);
            flushIn(stream);
        }
            break;
#endif // LCD

#ifdef LED
        case 'l': // led
            // TODO: syntax
        {
            stream.find(separatorString);
            uint8_t no = stream.parseInt();
            int value = stream.parseInt();
            flushIn(stream);
            setLogicLed(no, value);
        }
            break;
#endif // LED

#ifdef LISTEN
        case '@': // Listen
            flushIn(stream);
            do {
                receive(stream);
            } while (true); // ???
            break;
#endif // LISTEN

        case 'm': // modules
            flushIn(stream);
            stream.println(F(modulesSupported));
            break;

#ifdef PARAMETERS
        case 'p': // parameter
        {
            stream.find(separatorString);
            String variableName = stream.readStringUntil(' ');
            long value = stream.parseInt();
            stream.println(variableName);
            stream.println(value);
            flushIn(stream);
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
            if (variableName == "begintimeout")
                beginTimeout = value;
            else
#endif
#ifdef LED
            if (variableName == "blinktime")
                blinkTime = value;
            else
#endif
            {
	      stream.println(errorString);
            }
        }
            break;
#endif // PARAMETERS

#ifdef ETHERNET_SESSION
        case 'q': // quit
            flushIn(stream);
            quit = true;
            break;
#endif

#ifdef RECEIVE
        // TODO: force no-decode
        case 'r': // receive
            flushIn(stream);
            receive(stream);
            break;
#endif // RECEIVE

#ifdef RESET
        case 'R': // reset
            flushIn(stream);
            reset = true;
            break;
#endif



#ifdef TRANSMIT
        case 's': // send
        {
            stream.find(separatorString);
            // TODO: handle unparsable data gracefully
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
            flushIn(stream);
            if (irSender == NULL)
                irSender = new IRsendRaw();
            sendIrSignal(irSender, noSends, frequency, introLength, repeatLength, endingLength, intro, repeat, ending); // waits
            stream.println(okString);
        }
            break;
#endif // TRANSMIT

#ifdef RENDERER
        case 't': // transmit
        {
            stream.find(separatorString);
            // TODO: handle unparseable data gracefully
            uint16_t noSends = stream.parseInt();
            stream.find(separatorString);
            String protocol = stream.readStringUntil(' ');
            protocol.toLowerCase();
            IrSignal* signal = NULL;
            if (protocol == "nec1") {
                unsigned int D = stream.parseInt();
                unsigned int S = stream.parseInt();// TODO: Implement default S = 255-D;
                unsigned int F = stream.parseInt();
                signal = Nec1Renderer::render(D, S, F);
            } else if (protocol == "rc5") {
                unsigned int D = stream.parseInt();
                unsigned int F = stream.parseInt();
                unsigned int T = 0U; // FIXME?
                signal = Rc5Renderer::render(D, F, T);
            } else {
                stream.println(errorString);
                signal = NULL;
            }
            flushIn(stream);
            if (signal != NULL) {
                if (irSender == NULL)
                    irSender = new IRsendRaw();
                sendIrSignal(irSender, noSends, signal); // waits, blinks
            }
            stream.println(okString);
        }
	break;
#endif // RENDERER

        case 'v': // version
            flushIn(stream);
            stream.println(F(versionString));
            break;

        case '\n':
        case '\r':
            stream.println(F(okString));
            break;

        default:
            flushIn(stream);
            stream.println(F(errorString));
    }

    flushIn(stream);
#ifdef RESET
    if (reset)
        return false;
#endif

#ifdef ETHERNET_SESSION
    if (quit)
        return false;
#endif

    return true;
}

void loop() {
#ifdef ETHERNET
#ifdef USEUDP
#ifdef SERVER
    int packetSize = udp.parsePacket();
    if (packetSize) {
        IPAddress remote = udp.remoteIP();
#ifdef SERIAL_DEBUG
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        Serial.print(remote);
        Serial.print(", port ");
        Serial.println(udp.remotePort());
#endif
        //lcdPrint("UDP: ", true, 0, 0);
        for (int i = 0; i < 3; i++)
            lcdPrint(String(remote[i], DEC) + ".", false);
        lcdPrint(String(remote[3], DEC) + "@" + String(udp.remotePort(), DEC), false);
        lcdPrint(String(char(udp.peek())), false, 0, 3);

        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        work(udp);
        udp.endPacket();
    } else {
        delay(10);
        checkTurnoff();
    }
#endif // SERVER
#else // !USEUDP
    checkTurnoff();
#ifdef SERVER
    EthernetClient client = server.available();
    if (!client)
        return;
#ifdef LCD
    lcdPrint(F("Connection!"), true, 0, 0);
#endif
    client.println(F(PROGNAME));

    while (client.read() != -1)
        checkTurnoff();
#ifdef ETHERNET_SESSION
    while
#endif
        (work(client))
        ;
#ifdef LCD
    lcdPrint(F("Connection closed!"), true, 0, 0);
#endif
    client.println(F("Bye"));
#else // !SERVER
    IPAddress peer(PEER_IP);
    EthernetClient client;
    boolean status = client.connect(peer, PEER_PORT);
    if (!status)
        return;
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection!"));
#endif
#ifdef LCD
    lcdPrint(F("Connection!"), true, 0, 0);
#endif

#ifdef ETHERNET_SESSION
    while
#endif
        (work(client))
        ;
#endif // !SERVER
    client.flush();
    client.stop();

#endif // !USEUDP
#else // ! ETHERNET
    work(Serial);
#endif

#ifdef RESET
    if (reset)
        softwareReset();
#endif
}
