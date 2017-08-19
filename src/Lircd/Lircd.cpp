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

#ifdef ETHERNET
#include <Ethernet.h>
#include <IPAddress.h>
#endif // ETHERNET

#ifdef BEACON
#include <Beacon.h>
#endif

#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif

#ifdef TRANSMIT
#include <IrSenderPwm.h>
#include <Pronto.h>
#endif

#ifdef NON_MOD
#include <IrSenderNonMod.h>
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

#ifdef LED
#ifdef TRANSMIT
static LED_PARAMETER_CONST pin_t transmitled = TRANSMITLED;
#endif

static LED_PARAMETER_CONST pin_t commandled = COMMANDLED;

static LED_PARAMETER_CONST pin_t idleled = IDLELED;

static LED_PARAMETER_CONST pin_t connectionled = CONNECTIONLED;
#endif // LED

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

    irSender->sendIrSignal(irSignal, noSends);

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

#define modulesSupported EXPAND_AND_QUOTE(Base TRANSMIT_NAME RENDERER_NAME DECODER_NAME LED_NAME LCD_NAME PARAMETERS_NAME NAMED_COMMANDS_NAME )
#define PROGNAME "ALircd"
#define VERSION "2015-11-25"
#define okString "OK"
#define errorString "ERROR"
#define timeoutString "."

void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}

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
#if defined(TRANSMIT)
    // Make sure that sender is quiet (if reset or such)
    IrSenderPwm::getInstance(true)->mute();
#endif
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(F(PROGNAME "\n" VERSION));

#ifdef LED
    LedLcdManager::setupShouldTimeout(transmitled, false);
    LedLcdManager::setupShouldTimeout(commandled, false);
    LedLcdManager::setupShouldTimeout(idleled, false);
    LedLcdManager::setupShouldTimeout(connectionled, false);
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

    String ipstring = GirsUtils::ip2string(Ethernet.localIP());
    LedLcdManager::lcdPrint(ipstring, false, 0, 3);
#ifdef BEACON
    Beacon::setup(PROGNAME, "DE-AD-BE-EF-FE-ED", "Utility", "www.harctoolbox.org",
            "", "", "", "http://arduino/nosuchfile.html");
#endif

#ifdef USEUDP
#ifndef SERVER
#error Client mode for UDP not implemented
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

bool isPrefix(const String& cmd, const char *string) {
    return strncmp(cmd.c_str(), string, cmd.length()) == 0;
}

bool isPrefix(const char *string, const String& cmd) {
    return strncmp(cmd.c_str(), string, strlen(string)) == 0;
}

bool isPrefix(const String& cmd, const __FlashStringHelper *pstring) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, cmd.length()) == 0;
}

bool isPrefix(const __FlashStringHelper *pstring, const String& cmd) {
    return strncmp_PF(cmd.c_str(), (uint_farptr_t) pstring, strlen_PF((uint_farptr_t) pstring)) == 0;
}

// Process one command.
bool readProcessOneCommand(Stream& stream) {
#ifdef ETHERNET_SESSION
    bool quit = false;
#endif
#ifdef COMMANDLED
    LedLcdManager::setLogicLed(commandled, LedLcdManager::on);
#endif
    if (stream.available() == 0)
        return true;
    String line = stream.readStringUntil(EOLCHAR);
    line.trim();
    while (stream.peek() == EOLCHAR)
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
    myprintln(stream, F("BEGIN"));

    if (cmd.length() == 0) {
        myprintln(stream, "");
        myprintln(stream, F("ERROR"));
        myprintln(stream, F("DATA"));
        myprintln(stream, 1);
        myprintln(stream, F("bad send packet"));
    } else

#ifdef LCD
        if (cmd == F("lcd")) { //LCD
        myprintln(stream, line);
        myprintln(stream, F("SUCCESS"));
        String str = tokenizer.getRest();
        LedLcdManager::lcdPrint(str, true, 0, 0);
    } else
#endif // LCD

#ifdef LED
        if (isPrefix(cmd, F("led"))) {
        myprintln(stream, line);
        myprintln(stream, F("SUCCESS"));
        pin_t no = (pin_t) tokenizer.getInt();
        String value = tokenizer.getToken();
        bool success = LedLcdManager::setLogicLed(no, value.c_str());
        stream.println(success ? F(okString) : F(errorString));
    } else
#endif // LED

#ifdef FREEMEM
        if (cmd.startsWith(F("mem"))) {
        myprintln(stream, line);
        myprintln(stream, F("SUCCESS"));
        myprintln(stream, F("DATA"));
#ifdef RAMEND
        myprintln(stream, "2");
        myprintln(stream, RAMEND);
        myprintln(stream, GirsUtils::freeRam());
#else
        myprintln(stream, "1");
        myprintln(stream, freeRam());
#endif
    } else
#endif
        if (cmd.startsWith("m")) {
        myprintln(stream, line);
        myprintln(stream, F("SUCCESS"));
        myprintln(stream, F("DATA"));
        myprintln(stream, "1");
        stream.println(modulesSupported);
    } else

#ifdef ETHERNET_SESSION
        if (cmd.startsWith("q")) { // quit
        myprintln(stream, F("SUCCESS"));
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
            myprintln(stream, F("SUCCESS"));
            myprintln(stream, F("DATA"));
            // list all remotes
            myprintln(stream, remoteSet.getNoIrNamedRemotes());
            for (unsigned int i = 0; i < remoteSet.getNoIrNamedRemotes(); i++)
                myprintln(stream, remoteSet.getIrNamedRemotes()[i]->getName());
        } else {
            const IrNamedRemote* remote = remoteSet.getIrNamedRemote(remoteName.c_str());
            if (remote == NULL) {
                // unknown remote requested
                myprintln(stream, F("ERROR"));
                myprintln(stream, F("DATA"));
                myprintln(stream, 1);
                myprintln(stream, "unknown remote: \"" + String(remoteName) + "\"");
            } else {
                if (commandName.length() == 0) {
                    myprintln(stream, F("SUCCESS"));
                    myprintln(stream, F("DATA"));
                    // All commands of an existing remote
                    myprintln(stream, remote->getNoCommands());
                    for (unsigned int i = 0; i < remote->getNoCommands(); i++)
                        myprintln(stream, "0000000000000000 " + String(remote->getAllCommands()[i]->getName()));

                } else {
                    // One command of an existing remote
                    const IrNamedCommand* command = remote->getIrNamedCommand(commandName.c_str());
                    if (command == NULL) {
                        myprintln(stream, F("ERROR"));
                        myprintln(stream, F("DATA"));
                        myprintln(stream, 1);
                        myprintln(stream, "unknown command: \"" + String(commandName) + "\"");
                    } else {
                        myprintln(stream, F("SUCCESS"));
                        myprintln(stream, F("DATA"));
                        myprintln(stream, 1);
                        myprintln(stream, "0000000000000000 " + String(command->getName()));
                    }
                }
            }
        }
    } else
#endif

#ifdef TRANSMIT
        if (cmd.startsWith("send_ccf_once")) { // send
        uint16_t noSends = (uint16_t) tokenizer.getInt() + 1;
        IrSignal *irSignal = Pronto::parse(tokenizer.getRest().c_str());
        myprintln(stream, line);
        sendIrSignal(*irSignal, noSends); // waits
        myprintln(stream, F("SUCCESS"));
    } else
#endif // TRANSMIT

#ifdef RENDERER
        if (cmd.startsWith(F("render"))) { // transmit
            // TODO
        // TODO: handle unparseable data gracefully
        int16_t noSends = (int16_t) tokenizer.getInt();
        String protocol = tokenizer.getToken();
        const IrSignal *renderer = NULL;
        if (protocol == "nec1") {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int S = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            renderer = (F == Tokenizer::invalid)
                    ? Nec1Renderer::newIrSignal(D, S)
                    : Nec1Renderer::newIrSignal(D, S, F);
        } else if (protocol == "rc5") {
            unsigned int D = (unsigned) tokenizer.getInt();
            unsigned int F = (unsigned) tokenizer.getInt();
            unsigned int T = (unsigned) tokenizer.getInt();
            renderer = (T == Tokenizer::invalid)
                    ? Rc5Renderer::newIrSignal(D, F)
                    : Rc5Renderer::newIrSignal(D, F, T);
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
            myprintln(stream, F("ERROR"));
            myprintln(stream, F("DATA"));
            myprintln(stream, 1);
            myprintln(stream, "unknown remote: \"" + remoteName + "\"");
        } else {
            const IrNamedCommand* command = remote->getIrNamedCommand(commandName.c_str());
            if (command == NULL) {
                myprintln(stream, F("ERROR"));
                myprintln(stream, F("DATA"));
                myprintln(stream, 1);
                myprintln(stream, "unknown command: \"" + commandName + "\"");
            } else {
                const IrSignal *irSignal = command->getIrSignal();
                sendIrSignal(*irSignal, noSends + 1); // waits, blinks
                myprintln(stream, F("SUCCESS"));
            }
        }
    } else
#endif

        if (cmd == "version") { // version
        myprintln(stream, line + "  ");
        myprintln(stream, F("SUCCESS"));
        myprintln(stream, F("DATA"));
        myprintln(stream, 1);
        myprintln(stream, F(PROGNAME " " VERSION ""));

    } else {
        myprintln(stream, line);
        myprintln(stream, F("ERROR"));
        myprintln(stream, F("DATA"));
        myprintln(stream, 1);
        myprintln(stream, "unknown directive: \"" + cmd + "\"");
    }
    myprintln(stream, F("END"));

#ifdef ETHERNET_SESSION
    if (quit)
        return false;
#endif

    return true;
}

bool readProcessOneTcpCommand(EthernetClient& client) {
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
#ifdef CONNECTIONLED
    LedLcdManager::setLogicLed(connectionled, LedLcdManager::off);
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
}
