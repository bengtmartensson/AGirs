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
#include "Tokenizer.h"

#ifdef ETHERNET
#ifdef ETHER_ENC28J60
#error ETHER_ENC28J60 not yet supported
#endif

#include <Ethernet.h>
#include <IPAddress.h>

#endif // ETHERNET

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

#ifdef TRANSMIT
#include <IRLib.h>
#endif
#if defined(RECEIVE)
#include <TrivialDecoder.h>
#endif

#ifdef CAPTURE
#include <IrWidgetAggregating.h>
#endif

#ifdef NON_MOD
#include <NonModIrSender.h>
#endif

#ifdef DECODER
#include <MultiDecoder.h>
#endif

#ifdef RENDERER
#ifndef TRANSMIT
#error RENDER without TRANSMIT is nonsensical, aborting.
#endif
#include <IrSignal.h>
#include <Nec1Renderer.h>
#include <Rc5Renderer.h>
#endif

LCD_DEFINE(lcd);

#include <LedFuncs.inc> // Must come after lcd
#include <LcdFuncs.inc>

#if defined(CONFIGURABLE_LEDS) & (! defined(PARAMETERS) | !defined(LED))
#error CONFIGURABLE_LEDS defined but not PARAMETERS and LED, aborting.
#endif

#ifdef LED
#ifdef TRANSMIT
static LED_PARAMETER_CONST uint8_t transmitled = TRANSMITLED;
#endif

#ifdef RECEIVE
static LED_PARAMETER_CONST uint8_t receiveled = RECEIVELED;
#endif

#ifdef CAPTURE
static LED_PARAMETER_CONST uint8_t captureled = CAPTURELED;
#endif

static LED_PARAMETER_CONST uint8_t commandled = COMMANDLED;
#endif // LED

static PARAMETER_CONST unsigned long beginTimeout = DEFAULT_BEGINTIMEOUT; // milliseconds
#if defined(RECEIVE) || defined(CAPTURE)
static PARAMETER_CONST unsigned long endingTimeout = DEFAULT_ENDINGTIMEOUT; // milliseconds
#endif

#ifdef RECEIVE
//IrReceiverSampler *irReceiver = NULL;
///IRrecv *irReceiver = NULL;
///TrivialIrDecoder decoder;
#endif
#if defined(TRANSMIT)
IRsendRaw *irSender = NULL;
#endif
#ifdef CAPTURE
IrWidget *irWidget = NULL;
static PARAMETER_CONST uint16_t captureSize = DEFAULT_CAPTURESIZE;
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

String ip2string(IPAddress ip) {
    String result;
    for (int i = 0; i < 4; i++) {
        result.concat(String(ip[i], DEC));
        if (i < 3)
            result.concat(".");
    }
    return result;
}
#endif // ETHERNET

#if defined(TRANSMIT)

inline unsigned int hz2khz(unsigned int freq) {
    // prefer speed over accuracy...
    return freq/1000;
}

void sendIrSignal(IRsendRaw *irSender, unsigned int noSends, unsigned int frequency,
        unsigned int introLength, unsigned int repeatLength, unsigned int endingLength,
        /*const*/ unsigned int intro[], /*const*/ unsigned int repeat[], /*const*/ unsigned int ending[]) {
#ifdef TRANSMITLED
    setLogicLed(transmitled, HIGH);
#endif
#ifdef NON_MOD
#ifndef NON_MOD_PIN
#error NON_MOD defined but not NON_MOD_PIN, aborting
#endif
    if (frequency == 0) {
        NonModIrSender irSender(NON_MOD_PIN);
        if (introLength > 0)
            irSender.send(intro, introLength);
        for (unsigned int i = 0; i < noSends - (introLength > 0); i++)
            irSender.send(repeat, repeatLength);
        if (endingLength > 0)
            irSender.send(ending, endingLength);
    } else {
#endif
        if (introLength > 0)
            irSender->send(intro, introLength, hz2khz(frequency));
        for (unsigned int i = 0; i < noSends - (introLength > 0); i++)
            irSender->send(repeat, repeatLength, hz2khz(frequency));
        if (endingLength > 0)
            irSender->send(ending, endingLength, hz2khz(frequency));
#ifdef NON_MOD
    }
#endif
#ifdef TRANSMITLED
    setLogicLed(transmitled, LOW);
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
#define PROGNAME "AGirs"
#define VERSION "2015-07-14"
#define welcomeString "Welcome to " PROGNAME
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
    initialized = 0;
}
#endif

#ifdef FREEMEM
// http://playground.arduino.cc/Code/AvailableMemory#.U0EnzKogTzs
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif

#ifdef RESET
// TODO: This is somewhat suspect.
// Works at least on atmega386 and atmega2560,
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
#warning RESET not working on this platform, ignored
#undef RESET
#else
boolean reset = false;

// Restarts program from beginning but does not reset the peripherals and registers
void softwareReset() {
    asm volatile("  jmp 0");
}
#endif
#endif

#ifdef RECEIVE

void dump(const IRdecodeBase& decoder, Stream& stream) {
    // First entry is introductory silence, therefore start at 1, not 0
    for (unsigned int i = 1; i < decoder.rawlen; i++) {
        stream.write((i & 0x01) ? '+' : '-');
        stream.print(decoder.rawbuf[i], DEC);
        stream.print(" ");
    }
    stream.println();
}

void dump(const IrReceiverSampler &irReceiverSampler, Stream& stream) {
    for (uint16_t i = 0; i < irReceiverSampler.rawlen; i++) {
        stream.write((i & 0x01) ? '-' : '+');
        stream.print(irReceiverSampler.getDuration(i), DEC);
        stream.print(" ");
    }
    stream.println();
}

//void dump(const MultiDecoder &decoder, Stream& stream) {
//    dump(decoder.getRawDataLength(), decoder.getRawData(), stream);
//}

void receive(Stream& stream) {
#ifdef CAPTURE
    if (irWidget != NULL) {
        delete irWidget;
        irWidget = NULL;
    }
#endif // CAPTURE
    IrReceiverSampler *irReceiver = IrReceiverSampler::getInstance();
    if (irReceiver == NULL)
        irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, IRRECEIVER_PIN, IRRECEIVER_PIN_PULLUP_VALUE);
    irReceiver->setEndingTimeout(endingTimeout);
    irReceiver->setBeginningTimeout(beginTimeout);
    irReceiver->setMarkExcess(IRRECEIVER_MARK_EXCESS);
    flushIn(stream);
#ifdef RECEIVELED
    setLogicLed(receiveled, HIGH);
#endif
    boolean interrupted = false;
    irReceiver->enableIrIn();
    while (!(irReceiver->isReady()/*Results(&decoder)*/) && !interrupted) {
        //Serial.println(irReceiver->rcvstate);
        checkTurnoff();
        interrupted = stream.available();
    }
    irReceiver->disableIrIn();
#ifdef RECEIVELED
     setLogicLed(receiveled, LOW);
#endif
     if (interrupted) {
         stream.println(F(timeoutString));
         return;
     }
    // Setup decoder
    //decoder.decode();
#ifdef DECODER
    // Do actual decode
    MultiDecoder multiDecoder(*irReceiver); // multiDecoder(decoder);
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
            dump(*irReceiver, stream); // report data of undecoded signals
            break;
        default:
            stream.println(multiDecoder.getDecode()); // also for timeout
            break;
    }
#else // ! DECODE
    dump(*irReceiver, stream);
#endif // !DECODE
}
#endif // RECEIVE

#ifdef CAPTURE

void capture(Stream& stream) {
#ifdef RECEIVE
    IrReceiverSampler::deleteInstance();
#endif
    if (irWidget == NULL)
        irWidget = new IrWidgetAggregating(captureSize, &stream);
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
    if (irWidget->isReady()) {
        // Do not try to decode, that is what "receive" is for.
        irWidget->dump(stream);
    } else
        stream.println(F(timeoutString));
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
#ifdef SDCARD_ON_ETHERSHIELD_PIN
    // disable the SD card, as recommended in the doc
    pinMode(SDCARD_ON_ETHERSHIELD_PIN, OUTPUT);
    digitalWrite(SDCARD_ON_ETHERSHIELD_PIN, LOW);
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

#ifdef LCD
    lcdPrint(ip2string(Ethernet.localIP()), false, 0, 3);
#endif

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
    Serial.println(F(PROGNAME " " VERSION));
    Serial.setTimeout(serialTimeout);

#ifdef ETHERNET
    Serial.println(Ethernet.localIP());
#endif
#endif // !defined(ETHERNET) | defined(SERIAL_DEBUG)
}

// Process one command.
boolean work(Stream& stream) {
#ifdef ETHERNET_SESSION
    boolean quit = false;
#endif
#ifdef COMMANDLED
    setLogicLed(commandled, HIGH);
#endif
    //stream.println(F(okString));
    flushIn(stream);
    while (stream.available() == 0) {
#ifdef LED
        checkTurnoff();
#endif
    }
    String line = stream.readStringUntil(EOLCHAR);
#if defined(DEBUG_CMD)
    lcdPrint(line, true, 0, 0);
#endif
    Tokenizer tokenizer(line);
    String cmd = tokenizer.getToken();
#ifdef COMMANDLED
    setLogicLed(commandled, LOW);
#endif

    if (cmd.length() == 0) {
        // ok, do nothing
        stream.println(F(okString));
    } else

#ifdef CAPTURE
        if (cmd.startsWith("a") || cmd.startsWith("c")) {
        capture(stream);
    } else
#endif // CAPTURE

#ifdef FREEMEM
        if (cmd.startsWith("i")) {
        stream.println(freeRam());
        stream.println(RAMEND);
    } else
#endif

#ifdef LISTEN
        if (cmd == "listen") {
        do {
            receive(stream);
        } while (stream.available() == 0);
        stream.println(F(okString));
    } else
#endif // LISTEN

#ifdef LCD
        if (cmd.startsWith("lc")) { //LCD
            lcdPrint(tokenizer.getRest(), true, 0, 0);
        } else
#endif // LCD

#ifdef LED
        if (cmd.startsWith("l")) {
        int8_t no = (int8_t) tokenizer.getInt();
        int8_t value = (int8_t) tokenizer.getInt();
        setLogicLed(no, value);
    } else
#endif // LED

            if (cmd.startsWith("m")) {
            stream.println(F(modulesSupported));
        } else

#ifdef PARAMETERS
            if (cmd.startsWith("p")) { // parameter
            String variableName = tokenizer.getToken();
                    long value = tokenizer.getInt();
            if (!value) // parse error
                    stream.println(errorString);
            else
#if defined(RECEIVE) || defined(CAPTURE)
                if (variableName.startsWith(F("end")))
                    endingTimeout = value;
            else if (variableName.startsWith(F("beg")))
                    beginTimeout = value;
            else
#endif
#ifdef CAPTURE
                if (variableName.startsWith(F("captures")))
                    captureSize = value;
            else
#endif
#ifdef LED
#ifdef CONFIGURABLE_LEDS
#ifdef TRANSMITLED
                if (variableName.startsWith(F("transmitl")))
                    transmitled = (uint8_t) value;
                else
#endif
#ifdef CAPTURELED
                if (variableName.startsWith(F("capturel")))
                    captureled = (uint8_t) value;
                else
#endif
#ifdef RECEIVELED
                if (variableName.startsWith(F("receivel")))
                    receiveled = (uint8_t) value;
                else
#endif
#ifdef COMMANDLED
                if (variableName.startsWith(F("commandl")))
                    commandled = (uint8_t) value;
                else
#endif
#endif
                if (variableName.startsWith(F("bli")))
                    blinkTime = value;
            else
#endif
                stream.println(errorString);
            } else
#endif // PARAMETERS

#ifdef ETHERNET_SESSION
                if (cmd.startsWith("q")) { // quit
            quit = true;
                } else
#endif

#ifdef RESET
            if (cmd == "reset") {
            reset = true;
        } else
#endif
#ifdef RECEIVE
            // TODO: force no-decode
            if (cmd.startsWith("r")) {

            // receive
            receive(stream);
        } else
#endif // RECEIVE

#ifdef TRANSMIT
            if (cmd.startsWith("s")) { // send
            // TODO: handle unparsable data gracefully
            uint16_t noSends = (uint16_t) tokenizer.getInt();
            uint16_t frequency = (uint16_t) tokenizer.getInt();
            uint16_t introLength = (uint16_t) tokenizer.getInt();
            uint16_t repeatLength = (uint16_t) tokenizer.getInt();
            uint16_t endingLength = (uint16_t) tokenizer.getInt();
            uint16_t intro[introLength];
            uint16_t repeat[repeatLength];
            uint16_t ending[endingLength];
            // FIXME: Should take care of overflows, if a duration > 65536us
            for (uint16_t i = 0; i < introLength; i++)
                intro[i] = (uint16_t) tokenizer.getInt();
            for (uint16_t i = 0; i < repeatLength; i++)
                repeat[i] = (uint16_t) tokenizer.getInt();
            for (uint16_t i = 0; i < endingLength; i++)
                ending[i] = (uint16_t) tokenizer.getInt();
            if (irSender == NULL)
                irSender = new IRsendRaw();
            sendIrSignal(irSender, noSends, frequency, introLength, repeatLength, endingLength, intro, repeat, ending); // waits
            stream.println(okString);
        } else
#endif // TRANSMIT

#ifdef RENDERER
            if (cmd.startsWith("t")) { // transmit
            // TODO: handle unparseable data gracefully
            uint16_t noSends = (uint16_t) tokenizer.getInt();
                    String protocol = tokenizer.getToken();
                    IrSignal* signal = NULL;
            if (protocol == "nec1") {
                unsigned int D = (unsigned) tokenizer.getInt();
                        unsigned int S = (unsigned) tokenizer.getInt(); // TODO: Implement default S = 255-D;
                        unsigned int F = (unsigned) tokenizer.getInt();
                        signal = Nec1Renderer::render(D, S, F);
            } else if (protocol == "rc5") {
                unsigned int D = (unsigned) tokenizer.getInt();
                        unsigned int F = (unsigned) tokenizer.getInt();
                        unsigned int T = 0U; // FIXME?
                        signal = Rc5Renderer::render(D, F, T);
            } else {
                stream.println(errorString);
                        signal = NULL;
            }
            if (signal != NULL) {
                if (irSender == NULL)
                        irSender = new IRsendRaw();
                        sendIrSignal(irSender, noSends, signal); // waits, blinks
                }
            stream.println(okString);
        } else
#endif // RENDERER
    if (cmd.startsWith("v")) { // version
        stream.println(F(PROGNAME " " VERSION));
    } else {
        stream.println(F(errorString));
    }

    //flushIn(stream);
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
        lcdPrint("UDP: " + ip2string(remote), true, 0, 0);
        lcdPrint("@" + String(udp.remotePort(), DEC), false, 0, 1);
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
