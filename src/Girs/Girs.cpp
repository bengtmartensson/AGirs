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
#include <IrSenderPwm.h>
#endif
#if defined(RECEIVE)
#include <IrReceiverSampler.h>
#endif

#ifdef CAPTURE
#include <IrWidgetAggregating.h>
#endif

#ifdef NON_MOD
#include <IrSenderNonMod.h>
#endif

#ifdef DECODER
#ifndef RECEIVE
#error DECODER without RECEIVE is nonsensical, aborting.
#endif
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
static LED_PARAMETER_CONST pin_t transmitled = TRANSMITLED;
#endif

#ifdef RECEIVE
static LED_PARAMETER_CONST pin_t receiveled = RECEIVELED;
#endif

#ifdef CAPTURE
static LED_PARAMETER_CONST pin_t captureled = CAPTURELED;
#endif

static LED_PARAMETER_CONST pin_t commandled = COMMANDLED;
#endif // LED

static PARAMETER_CONST unsigned long beginTimeout = DEFAULT_BEGINTIMEOUT; // milliseconds
#if defined(RECEIVE) || defined(CAPTURE)
static PARAMETER_CONST unsigned long endingTimeout = DEFAULT_ENDINGTIMEOUT; // milliseconds
#endif

#if defined(RECEIVE) | defined(CAPTURE)
static PARAMETER_CONST uint16_t captureSize = DEFAULT_CAPTURESIZE;
#endif

#if defined(RECEIVE) & defined(IRRECEIVER_2_PIN)
static PARAMETER_CONST uint8_t receiverNo = 1;
#else
static const uint8_t receiverNo = 1;
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

#ifdef TRANSMITLED
    setLogicLed(transmitled, LOW);
#endif
}
#endif

#ifdef RENDERER
void sendIrSignal(unsigned int noSends, const IrSignal *signal) {
    sendIrSignal(noSends, signal->getFrequency(),
            signal->getLengthIntro(), signal->getLengthRepeat(), signal->getLengthEnding(),
        signal->getIntro(), signal->getRepeat(), signal->getEnding());
}
#endif

#define modulesSupported EXPAND_AND_QUOTE(Base TRANSMIT_NAME CAPTURE_NAME RENDERER_NAME RECEIVE_NAME DECODER_NAME LED_NAME LCD_NAME PARAMETERS_NAME)
#define PROGNAME "AGirs"
#define VERSION "2015-07-27"
#define welcomeString "Welcome to " PROGNAME
#define okString "OK"
#define errorString "ERROR"
#define noSuchVariableError "NO-SUCH-VARAIBLE"
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
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO) | defined(ARDUINO_ARCH_SAMD)
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

void decodeOrDump(IrReader *irReader, Stream& stream) {
#ifdef DECODER
    MultiDecoder multiDecoder(*irReader); // multiDecoder(decoder);
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
            irReader->dump(stream); // report data of undecoded signals
            break;
        default:
            stream.println(multiDecoder.getDecode()); // also for timeout
            break;
    }
#else  // !DECODER
    irReader->dump(stream);
#endif // !DECODER
}

#ifdef RECEIVE

void receive(Stream& stream) {
#ifdef CAPTURE
    IrWidgetAggregating::deleteInstance(); // save memory
#endif // CAPTURE
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
     if (interrupted) {
         stream.println(F(timeoutString));
         return;
     }
     decodeOrDump(irReceiver, stream);
     IrReceiverSampler::deleteInstance();
}
#endif // RECEIVE

#ifdef CAPTURE

void capture(Stream& stream) {
#ifdef RECEIVE
    IrReceiverSampler::deleteInstance();
#endif
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
    if (irWidget->isReady()) {
        // Do not try to decode, that is what "receive" is for.
#ifdef DECODE_CAPTURES
        decodeOrDump(irWidget, stream);
#else
        irWidget->dump(stream);
#endif
    } else
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
#ifdef RAMEND
        stream.println(RAMEND);
#endif
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
        pin_t no = (pin_t) tokenizer.getInt();
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
        unsigned long *variable32 = NULL;
        uint16_t *variable16 = NULL;
        uint8_t *variable8 = NULL;
        //if (!value) // parse error
        //    stream.println(errorString);
        //else
#if defined(RECEIVE) || defined(CAPTURE)
            if (variableName.startsWith(F("end")))
            variable32 = &endingTimeout;
        else if (variableName.startsWith(F("beg")))
            variable32 = &beginTimeout;
        else
#endif
#if defined(RECEIVE) & defined(IRRECEIVER_2_PIN)
            if (variableName.startsWith(F("receiver")))
            variable8 = &receiverNo;
        else
#endif
#ifdef CAPTURE
            if (variableName.startsWith(F("captures")))
            // TODO: check evenness of value
            variable16 = &captureSize;
        else
#endif
#ifdef LED
#ifdef CONFIGURABLE_LEDS
#ifdef TRANSMITLED
            if (variableName.startsWith(F("transmitl")))
            variable8 = &transmitled;
        else
#endif
#ifdef CAPTURELED
            if (variableName.startsWith(F("capturel")))
            variable8 = &captureled;
        else
#endif
#ifdef RECEIVELED
            if (variableName.startsWith(F("receivel")))
            variable8 = &receiveled;
        else
#endif
#ifdef COMMANDLED
            if (variableName.startsWith(F("commandl")))
            variable8 = &commandled;
        else
#endif
#endif
            if (variableName.startsWith(F("bli")))
            variable32 = &blinkTime;
        else
#endif
        {
        }

        if (variable32 != NULL) {
            if (value)
                *variable32 = value;
            else
                stream.println(variableName + "=" + *variable32);
        } else if (variable16 != NULL) {
            if (value)
                *variable16 = (uint16_t) value;
            else
                stream.println(variableName + "=" + *variable16);
        } else if (variable8 != NULL) {
            if (value)
                *variable8 = (uint8_t) value;
            else
                stream.println(variableName + "=" + *variable8);
        } else
            stream.println(F(noSuchVariableError));
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
    } else
#endif // TRANSMIT

#ifdef RENDERER
        if (cmd.startsWith("t")) { // transmit
        // TODO: handle unparseable data gracefully
        uint16_t noSends = (uint16_t) tokenizer.getInt();
        String protocol = tokenizer.getToken();
        IrSignal *signal = NULL;
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
            sendIrSignal(noSends, signal); // waits, blinks
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
