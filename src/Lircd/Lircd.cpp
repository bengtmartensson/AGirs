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

#include "config.h"
#include <LedLcdManager.h>
#include <GirsUtils.h>
#include <IrRenderer.h>

#ifdef ETHERNET
#include <Ethernet.h>
#include <IPAddress.h>

#endif // ETHERNET

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

#ifdef NAMED_COMMANDS
#include "IrNamedRemoteSet.h"
#include "NamedNec1Command.h"
#include "NamedRc5Command.h"
#endif

#define LED_PARAMETER_CONST const
#define PARAMETER_CONST const

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

static LED_PARAMETER_CONST pin_t idleled = IDLELED;

static LED_PARAMETER_CONST pin_t connectionled = CONNECTIONLED;
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

#ifdef TRANSMIT

void sendIrSignal(const IrSignal &irSignal, unsigned int noSends=1) {
#ifdef TRANSMITLED
    LedLcdManager::setLogicLed(transmitled, LedLcdManager::on);
#endif
    IrSender *irSender =
#ifdef NON_MOD
            (irSignal.getFrequency() == 0) ? (IrSender*) new IrSenderNonMod(NON_MOD_PIN) :
#endif
            (IrSender*) IrSenderPwm::getInstance(true);

    irSender->sendSignal(irSignal, noSends);

#ifdef NON_MOD
    if (irSignal.getFrequency() == 0)
        delete irSender;
    else
#endif
        IrSenderPwm::deleteInstance();

#ifdef TRANSMITLED
    LedLcdManager::setLogicLed(transmitled, LedLcdManager::off);
#endif
}
#endif  //TRANSMIT

/*#ifdef RENDERER
void sendIrSignal(int noSends, const IrSignal *signal) {
    sendIrSignal(noSends, signal->getFrequency(),
            signal->getLengthIntro(), signal->getLengthRepeat(), signal->getLengthEnding(),
        signal->getIntro(), signal->getRepeat(), signal->getEnding());
}

void sendIrSignal(int noSends, const Renderer* renderer) {
    const IrSignal* signal = renderer->toSignal();
    sendIrSignal(noSends, signal->getFrequency(),
            signal->getLengthIntro(), signal->getLengthRepeat(), signal->getLengthEnding(),
        signal->getIntro(), signal->getRepeat(), signal->getEnding());
    delete signal;
}
#endif // RENDERER
 */

#define modulesSupported EXPAND_AND_QUOTE(Base TRANSMIT_NAME CAPTURE_NAME RENDERER_NAME RECEIVE_NAME DECODER_NAME LED_NAME LCD_NAME PARAMETERS_NAME NAMED_COMMANDS_NAME )
#define PROGNAME "ALircd"
#define VERSION "2015-11-23"
#define okString "OK"
#define errorString "ERROR"
#define timeoutString "."

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
#if 0
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
#endif

#ifdef RECEIVE

void receive(Stream& stream) {
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
     setLogicLed(receiveled, LedLcdManager::off);
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
    IrWidget *irWidget = IrWidgetAggregating::newIrWidgetAggregating(captureSize, stream);
    if (irWidget == NULL)
        stream.println(F("This cannot happen"));//    irWidget = IrWidgetAggregating::getInstance();
    irWidget->setEndingTimeout(endingTimeout);
    irWidget->setBeginningTimeout(beginTimeout);
    irWidget->reset();
#ifdef CAPTURELED
    setLogicLed(captureled, HIGH);
#endif
    flushIn(stream);
    irWidget->capture();
#ifdef CAPTURELED
    setLogicLed(captureled, LedLcdManager::off); // FIXME
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
    IrWidgetAggregating::deleteInstance();
}
#endif // CAPTURE

#ifdef NAMED_COMMANDS
const IrNamedCommand* yamaha_cmds[] = {
    new NamedNec1Command("volume_up",    122, 26),
    new NamedNec1Command("volume_down",  122, 27),
    new NamedNec1Command("power_on",     122, 29),
    new NamedNec1Command("power_off",    122, 30)
};

const IrNamedCommand* tv_cmds[] = {
    new NamedRc5Command("0", 0, 0),
    new NamedRc5Command("1", 0, 1),
    new NamedRc5Command("2", 0, 2),
    new NamedRc5Command("3", 0, 3),
    new NamedRc5Command("4", 0, 4),
    new NamedRc5Command("5", 0, 5),
    new NamedRc5Command("6", 0, 6),
    new NamedRc5Command("7", 0, 7),
    new NamedRc5Command("8", 0, 8),
    new NamedRc5Command("9", 0, 9),
    new NamedRc5Command("power_toggle", 0, 12),
};

const IrNamedRemote* remotes[] = {
    new IrNamedRemote("yamaha", yamaha_cmds, sizeof (yamaha_cmds) / sizeof (IrNamedCommand*)),
    new IrNamedRemote("tv",     tv_cmds,     sizeof (tv_cmds) / sizeof (IrNamedCommand*)),
};

IrNamedRemoteSet remoteSet(remotes, sizeof (remotes) / sizeof (IrNamedRemote*));

#if 0
boolean sendNamedCommand(Stream& stream, unsigned int noSends, String& remoteName, String& commandName) {
    const Remote* remote = remoteSet.getRemote(remoteName);
    if (remote == NULL) {
        stream.println(F("No such remote"));
        return false;
    }

    const Command* command = remote->getCommand(commandName);
    if (command == NULL) {
        stream.println(F("No such command"));
        return false;
    }
    const Renderer *renderer = command->getRenderer();
    if (renderer != NULL) {
        sendIrSignal(noSends, renderer); // waits, blinks
        delete renderer;
    }
    return true;
}
#endif
#endif // NAMED_COMMANDS

void myprintln(Stream& stream, const String& str) {
    stream.write((str + '\n').c_str(), str.length() + 1);
}

void myprintln(Stream& stream, int n) {
    myprintln(stream, String(n));
}

void myprintln(Stream& stream, const char* str) {
    char s[strlen(str) + 1];
    strcpy(s, str);
    s[strlen(str)] = '\n';
    stream.write(s, strlen(str) + 1);
}

void setup() {
    LedLcdManager::setupLedGroundPins();
    GirsUtils::setupReceivers();
    GirsUtils::setupSensors();
#if defined(TRANSMIT)
    // Make sure that sender is quiet (if reset or such)
    IrSenderPwm::getInstance(true)->mute();
#endif
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(PROGNAME "\n" VERSION);
#ifdef LED
    LedLcdManager::setupShouldTimeout(transmitled, false);
    LedLcdManager::setupShouldTimeout(receiveled, false);
    LedLcdManager::setupShouldTimeout(captureled, false);
    LedLcdManager::setupShouldTimeout(commandled, false);
#endif
#ifdef LCD
#ifdef ETHERNET
#ifdef USEUDP
    LedLcdManager::lcdPrint(F("UDP"), false, 0, 2);
#else
    LedLcdManager::lcdPrint(F("TCP"), false, 0, 2);
#endif
#ifdef SERVER
    LedLcdManager::lcdPrint(F(",Srv"), false);
#else
    LedLcdManager::lcdPrint(" " + String(peer[0], DEC) + "." + String(peer[1], DEC) + "."
            + String(peer[2], DEC) + "." + String(peer[3], DEC) + "@" + String(PEER_PORT), false);
#endif
#ifdef SERIAL_DEBUG
    LedLcdManager::lcdPrint(F(",SerialDbg"), false);
#endif
#else // ! ETHERNET
    LedLcdManager::lcdPrint(F("Serial"), false, 0, 2);
#endif // ! ETHERNET
#endif // LCD

#ifdef ETHERNET
#ifdef SDCARD_ON_ETHERSHIELD_PIN
    // disable the SD card, as recommended in the doc
    pinMode(SDCARD_ON_ETHERSHIELD_PIN, OUTPUT);
    digitalWrite(SDCARD_ON_ETHERSHIELD_PIN, LOW);
#endif
    byte mac[] = { MACADDRESS };
#ifdef DHCP
    Ethernet.begin(mac);
#else // !DHCP
    Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

    String ipstring = ip2string(Ethernet.localIP());
    LedLcdManager::lcdPrint(ipstring, false, 0, 3);
#ifdef BEACON
    Beacon::setup(PROGNAME, "DE-AD-BE-EF-FE-ED", "Utility", "www.harctoolbox.org",
            "", "", "", "http://arduino/nosuchfile.html");
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

#ifdef SERIAL_DEBUG

    Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.println(F(PROGNAME " " VERSION));
    Serial.setTimeout(serialTimeout);

#ifdef SERIAL_DEBUG
    Serial.println(Ethernet.localIP());
#endif
#endif // SERIAL_DEBUG
}

boolean isPrefix(const String& cmd, const char *string) {
    return strncmp(cmd.c_str(), string, cmd.length()) == 0;
}

boolean isPrefix(const char *string, const String& cmd) {
    return strncmp(cmd.c_str(), string, strlen(string)) == 0;
}

boolean isPrefix(const String& cmd, const __FlashStringHelper *pstring) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, cmd.length()) == 0;
}

boolean isPrefix(const __FlashStringHelper *pstring, const String& cmd) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, strlen_PF((uint_farptr_t) pstring)) == 0;
}

// Process one command.

boolean readProcessOneCommand(Stream& stream) {
#ifdef ETHERNET_SESSION
    boolean quit = false;
#endif
#ifdef COMMANDLED
    LedLcdManager::setLogicLed(commandled, LedLcdManager::off);
#endif
    //stream.println(F(okString));
    //flushIn(stream);
    //while (stream.available() == 0) {
#ifdef LED
        //checkTurnoff();
#endif
    //}
    if (stream.available() == 0)
        return true;
    String line = stream.readStringUntil(EOLCHAR);
    line.trim();
    while (stream.peek() == '\n')
        stream.read();
#if defined(DEBUG_CMD)
    LedLcdManager::lcdPrint(line, true, 0, 0);
#endif

    Tokenizer tokenizer(line);
    String cmd = tokenizer.getToken();
    cmd.toLowerCase();
    LedLcdManager::lcdPrint(cmd, false, 0, 3);
#ifdef COMMANDLED
    LedLcdManager::setLogicLed(commandled, LedLcdManager::off);
#endif
    myprintln(stream, "BEGIN");

    if (cmd.length() == 0) {
        myprintln(stream, "");
        myprintln(stream, "ERROR");
        myprintln(stream, "DATA");
        myprintln(stream, 1);
        myprintln(stream, "bad send packet");
    } else

#ifdef CAPTURE
        if (cmd.startsWith("a") || cmd.startsWith("c")) {
        capture(stream);
    } else
#endif // CAPTURE

#ifdef LISTEN
        if (cmd == "listen") {
        do {
            receive(stream);
        } while (stream.available() == 0);
        stream.println(F(okString));
    } else
#endif // LISTEN

#ifdef LCD
        if (cmd == "lcd") { //LCD
        myprintln(stream, line);
        myprintln(stream, "SUCCESS");
        String str = tokenizer.getRest();
        LedLcdManager::lcdPrint(str, true, 0, 0);
    } else
#endif // LCD

#ifdef LED
        if (isPrefix(cmd, F("led"))) {
        myprintln(stream, line);
        myprintln(stream, "SUCCESS");
        pin_t no = (pin_t) tokenizer.getInt();
        String value = tokenizer.getToken();
        boolean success = LedLcdManager::setLogicLed(no, value.c_str());
        stream.println(success ? F(okString) : F(errorString));
    } else
#endif // LED

#ifdef FREEMEM
        if (cmd.startsWith("mem")) {
        myprintln(stream, line);
        myprintln(stream, "SUCCESS");
        myprintln(stream, "DATA");
#ifdef RAMEND
        myprintln(stream, "2");
        myprintln(stream, RAMEND);
        myprintln(stream, freeRam());
#else
        myprintln(stream, "1");
        myprintln(stream, freeRam());
#endif
    } else
#endif
        if (cmd.startsWith("m")) {
        myprintln(stream, line);
        myprintln(stream, "SUCCESS");
        myprintln(stream, "DATA");
        myprintln(stream, "1");
        stream.println(modulesSupported);
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
            stream.println(F("No such variable"));
    } else
#endif // PARAMETERS

#ifdef ETHERNET_SESSION
        if (cmd.startsWith("q")) { // quit
        myprintln(stream, "SUCCESS");
        quit = true;
    } else
#endif

#ifdef NAMED_COMMANDS
        if (cmd == "list") {
        String remoteName = tokenizer.getToken();
        String commandName = tokenizer.getToken();
        myprintln(stream, line
                + (remoteName.length() == 0 ? " " : "")
                + (commandName.length() == 0 ? " " : ""));
        if (remoteName.length() == 0) {
            myprintln(stream, "SUCCESS");
            myprintln(stream, "DATA");
            // list all remotes
            myprintln(stream, remoteSet.getNoIrNamedRemotes());
            for (unsigned int i = 0; i < remoteSet.getNoIrNamedRemotes(); i++)
                myprintln(stream, remoteSet.getIrNamedRemotes()[i]->getName());
        } else {
            const IrNamedRemote* remote = remoteSet.getIrNamedRemote(remoteName.c_str());
            if (remote == NULL) {
                // unknown remote requested
                myprintln(stream, "ERROR");
                myprintln(stream, "DATA");
                myprintln(stream, 1);
                myprintln(stream, "unknown remote: \"" + String(remoteName) + "\"");
            } else {
                if (commandName.length() == 0) {
                    myprintln(stream, "SUCCESS");
                    myprintln(stream, "DATA");
                    // All commands of an existing remote
                    myprintln(stream, remote->getNoCommands());
                    for (unsigned int i = 0; i < remote->getNoCommands(); i++)
                        myprintln(stream, "0000000000000000 " + String(remote->getAllCommands()[i]->getName()));

                } else {
                    // One command of an existing remote
                    const IrNamedCommand* command = remote->getIrNamedCommand(commandName.c_str());
                    if (command == NULL) {
                        myprintln(stream, "ERROR");
                        myprintln(stream, "DATA");
                        myprintln(stream, 1);
                        myprintln(stream, "unknown command: \"" + String(commandName) + "\"");
                    } else {
                        myprintln(stream, "SUCCESS");
                        myprintln(stream, "DATA");
                        myprintln(stream, 1);
                        myprintln(stream, "0000000000000000 " + String(command->getName()));
                    }
                }
            }
        }
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
#if 0
// Not implemented yet
        if (cmd.startsWith("send_ccf_once")) { // send
            // TODO
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
        IrSignal irSignal(intro, introLength, repeat, repeatLength, ending, endingLength, frequency);
        sendIrSignal(irSignal, noSends); // waits
        stream.println(F(okString));
    } else
#endif
#endif // TRANSMIT

#ifdef RENDERER
        if (cmd.startsWith("render")) { // transmit
            // TODO
        // TODO: handle unparseable data gracefully
        int16_t noSends = (int16_t) tokenizer.getInt();
        String protocol = tokenizer.getToken();
        IrRenderer *renderer = NULL;
        if (protocol == "nec1") {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int S = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            renderer = (F == Tokenizer::invalid)
                    ? new Nec1Renderer(D, S)
                    : new Nec1Renderer(D, S, F);
        } else if (protocol == "rc5") {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            unsigned int T = (unsigned) tokenizer.getInt();
            renderer = (T == Tokenizer::invalid)
                    ? new Rc5Renderer(D, F)
                    : new Rc5Renderer(D, F, T);
        } else {
            stream.print(F("no such protocol: "));
            stream.println(protocol);
        }
        if (renderer != NULL) {
            sendIrSignal(*renderer, noSends); // waits, blinks
            delete renderer;
        }
        stream.println(okString);
    } else
#endif // RENDERER

#ifdef NAMED_COMMANDS
        if (cmd == "send_once") {
        String remoteName = tokenizer.getToken();
        String commandName = tokenizer.getToken();
        int noSends = (int) tokenizer.getInt();
        if (noSends == Tokenizer::invalid)
            noSends = 0;

        myprintln(stream, line);

        const IrNamedRemote* remote = remoteSet.getIrNamedRemote(remoteName.c_str());
        if (remote == NULL) {
            myprintln(stream, "ERROR");
            myprintln(stream, "DATA");
            myprintln(stream, 1);
            myprintln(stream, "unknown remote: \"" + remoteName + "\"");
        } else {
            const IrNamedCommand* command = remote->getIrNamedCommand(commandName.c_str());
            if (command == NULL) {
                myprintln(stream, "ERROR");
                myprintln(stream, "DATA");
                myprintln(stream, 1);
                myprintln(stream, "unknown command: \"" + commandName + "\"");
            } else {
                const IrSignal& irSignal = command->getIrSignal();
                //if (irSignal != NULL) {
                sendIrSignal(irSignal, noSends + 1); // waits, blinks
                //delete renderer;
                myprintln(stream, "SUCCESS");
                //} else
                //   myprintln(stream, "ERROR");
            }
        }
    } else
#endif

        if (cmd == "version") { // version
        myprintln(stream, line + "  ");
        myprintln(stream, "SUCCESS");
        myprintln(stream, "DATA");
        myprintln(stream, 1);
        myprintln(stream, PROGNAME " " VERSION "");

    } else {
        myprintln(stream, line);
        myprintln(stream, "ERROR");
        myprintln(stream, "DATA");
        myprintln(stream, 1);
        myprintln(stream, "unknown directive: \"" + cmd + "\"\n");
    }
    myprintln(stream, "END");

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

boolean readProcessOneTcpCommand(EthernetClient& client) {
    while (client.available() == 0) {
        LedLcdManager::checkTurnoff();
#ifdef BEACON
        Beacon::checkSend();
#endif
        if (!client.connected())
            return false;
    }
    return readProcessOneCommand(client);
}

void loop() {
    LedLcdManager::checkTurnoff();
#ifdef BEACON
    Beacon::checkSend();
#endif
#ifdef IDLELED
    LedLcdManager::setLogicLed(idleled, LedLcdManager::on);
#endif

    EthernetClient client = server.available();
    if (!client)
        return;
    client.setTimeout(10000);
#ifdef LCD
    LedLcdManager::lcdPrint(F("Connection!"), true, 0, 0);
#endif

#ifdef IDLELED
    LedLcdManager::setLogicLed(idleled, LedLcdManager::off);
#endif
#ifdef CONNECTIONLED
    LedLcdManager::setLogicLed(connectionled, LedLcdManager::on);
#endif
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection!"));
#endif
    //client.println(F(PROGNAME));
    //while (client.read() != -1)
    //    LedLcdManager::checkTurnoff();
    while (readProcessOneTcpCommand(client))
#ifdef BEACON
        Beacon::checkSend()
#endif
        ;
#ifdef LCD
    LedLcdManager::lcdPrint(F("Connection closed!"), true, 0, 0);
#endif
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection closed!"));
#endif
    client.println(F("Bye"));

    if (client.connected())
        client.flush();
    client.stop();
#ifdef CONNECTIONLED
    LedLcdManager::setLogicLed(connectionled, LedLcdManager::off);
#endif
}
