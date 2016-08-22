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

#include "config.h"
#include "LedLcdManager.h"
#include <GirsUtils.h>

#ifdef ARDUINO

#else // ! ARDUINO

// Define some dummy stuff to be able to compile and test
#if 0
#define SIGNAL_LED_1     13
#define SIGNAL_LED_2     101
#define SIGNAL_LED_3     102
#define SIGNAL_LED_4     103

#define SIGNAL_LED_5_GND 22
#define SIGNAL_LED_5     23
#define SIGNAL_LED_6_GND 24
#define SIGNAL_LED_6     25
#define SIGNAL_LED_7_GND 26
#define SIGNAL_LED_7     27
#define SIGNAL_LED_8_GND 28
#define SIGNAL_LED_8     29
#define IRRECEIVER_1_PIN 5
#define IRRECEIVER_1_GND 6
#define IRRECEIVER_1_VSS 7
#endif

#endif // ! ARDUINO

#ifdef ETHERNET
#include <Ethernet.h>
#include <IPAddress.h>
#endif // ETHERNET

#ifdef LCD
#include <LiquidCrystal_I2C.h>
#endif

#ifdef RECEIVE
#include <IrReceiverSampler.h>
#endif

static const long serialBaud = SERIALBAUD;
static const long serialTimeout = SERIALTIMEOUT;
static const milliseconds_t beginTimeout = DEFAULT_BEGINTIMEOUT; // not really relevant here
static const milliseconds_t endingTimeout = DEFAULT_RECEIVE_ENDINGTIMEOUT;
static const size_t captureSize = DEFAULT_CAPTURESIZE;
static const microseconds_t markExcess = IRRECEIVER_MARK_EXCESS;
#ifdef BEACON
#include <Beacon.h>
#endif

#ifdef RECEIVELED
static LED_PARAMETER_CONST led_t receiveled = RECEIVELED;
#endif

#ifdef COMMANDLED
static LED_PARAMETER_CONST led_t commandled = COMMANDLED;
#endif

static IrReceiver *irReceiver = NULL;

#define PROGNAME "Listener"
#define VERSION "2016-08-21"

#ifdef ETHERNET
#ifdef USEUDP
EthernetUDP udp;
IPAddress broadcastIp(BROADCAST_IP);
#else // !USEUDP
#ifdef SERVER
static EthernetServer server(PORT);
#else
IPAddress peer(PEER_IP);
#endif
#endif // !USEUDP
#endif // ETHERNET

#ifdef ARDUINO
void flushIn(Stream &stream) {
    while (stream.available())
        stream.read();
}
Stream& stream = Serial;
#else
static Stream stream(std::cout);
#endif

static const char* readOneDecode() {
#ifdef RECEIVELED
    LedLcdManager::setLogicLed(receiveled, LedLcdManager::on);
#endif
    irReceiver->enable();
    // Wait until something arrives
    while (!irReceiver->isReady())
        LedLcdManager::checkTurnoff();

    MultiDecoder multiDecoder(*irReceiver);
#ifdef RECEIVELED
    LedLcdManager::setLogicLed(receiveled, LedLcdManager::off);
#endif
#ifdef LCD
    if (multiDecoder.getType() > MultiDecoder::noise) {
        LedLcdManager::lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
                ? "." : multiDecoder.getDecode(),
                multiDecoder.getType() != MultiDecoder::nec_ditto);
        if (multiDecoder.getType() == MultiDecoder::nec)
            LedLcdManager::lcdSetCursor(0, 1); // prepare for dittos
    }
#endif
    LedLcdManager::setLogicLed(GirsUtils::decode2logicalLed(multiDecoder.getType()), LedLcdManager::blink);
    irReceiver->disable();
    return multiDecoder.getDecode();
}

// Read and process one signal (or timeout).
//static void printOneDecode(Stream& stream) {
//    const char *decode = readOneDecode();
//    stream.println(decode);
//}

void setup() {
    LedLcdManager::setupLedGroundPins();
    GirsUtils::setupReceivers();
    GirsUtils::setupLeds();
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(F(PROGNAME "\n" VERSION));
#ifdef LED
    LedLcdManager::setupShouldTimeout(receiveled, false);
    LedLcdManager::setupShouldTimeout(commandled, false);
#endif
#ifdef LCD
#ifdef ETHERNET
#ifdef USEUDP
    LedLcdManager::lcdPrint(F("UDP"), false, 0, 2);
#else
    LedLcdManager::lcdPrint(F("TCP"), false, 0, 2);

#ifdef SERVER
    LedLcdManager::lcdPrint(F(",Srv"), false);
#else
    LedLcdManager::lcdPrint(" " + String(peer[0], DEC) + "." + String(peer[1], DEC) + "."
            + String(peer[2], DEC) + "." + String(peer[3], DEC) + "@" + String(PEER_PORT), false);
#endif
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
#ifdef SERVER
#error Server mode for UDP not implementd
#endif
#define DUMMYPORT 8888
    udp.begin(DUMMYPORT);
#else
#ifdef SERVER
    server.begin();
#endif // SERVER
#endif // USEUDP

#endif // ETHERNET
#if defined(ARDUINO) & !defined(ETHERNET) | defined(SERIAL_DEBUG)

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
#endif // defined(ARDUINO) & !defined(ETHERNET) | defined(SERIAL_DEBUG)

    stream.println(F(PROGNAME " " VERSION));

    irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, IRRECEIVER_1_PIN,
            IRRECEIVER_1_PULLUP_VALUE, markExcess, beginTimeout, endingTimeout);
}

void readPrintOneCommand(Stream& stream) {
    //    String line = readCommand(stream);
    //#ifdef SERIAL_DEBUG
    //    Serial.println("Command: " + line);
    //#endif
    const char* decode = readOneDecode();
    stream.println(decode);
    //return true;
}

#if defined(ETHERNET)
#ifdef USEUDP
void readPrintOneUdpCommand() {
    const char *decode = readOneDecode();
    udp.beginPacket(broadcastIp, BROADCAST_PORT);
    udp.write(decode);
    udp.write(EOLCHAR);
    udp.endPacket();
}
#else // ! USEUDP
boolean readProcessOneTcpCommand(EthernetClient& client) {
    while (client.available() > 0)
        client.read();

    //    LedLcdManager::checkTurnoff();
#ifdef BEACON
        Beacon::checkSend();
#endif
        if (!client.connected())
            return false;
    //}
    readPrintOneCommand(client);
    return true;
}
#endif // ! USEUDP
#endif // ETHERNET

void loop() {
    LedLcdManager::checkTurnoff();
#ifdef ETHERNET
#ifdef BEACON
    Beacon::checkSend();
#endif
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
        LedLcdManager::lcdPrint("UDP: " + ip2string(remote), true, 0, 0); // TODO: #ifdef...
        LedLcdManager::lcdPrint("@" + String(udp.remotePort(), DEC), false, 0, 1);
        String peek(char(udp.peek()));
        LedLcdManager::lcdPrint(peek, false, 0, 2);

        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        readProcessOneCommand(udp);
        udp.endPacket();
    } else {
        delay(10);
        LedLcdManager::checkTurnoff();
    }
#endif // SERVER
    readPrintOneUdpCommand();
#else // ! USEUDP

#ifdef SERVER
    EthernetClient client = server.available();
    if (!client)
        return;
    client.setTimeout(10000);
#ifdef LCD
    LedLcdManager::lcdPrint(F("Connection!"), true, 0, 0);
#endif
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection!"));
#endif
    client.println(F(PROGNAME));
#if defined(COMMANDLED) & defined(LED)
    LedLcdManager::setLogicLed(commandled, LedLcdManager::on);
#endif

    //while (client.read() != -1)
    //    LedLcdManager::checkTurnoff();
#ifdef SESSION
    while (readProcessOneTcpCommand(client))
#if defined(COMMANDLED) & defined(LED)
        LedLcdManager::setLogicLed(commandled, LedLcdManager::on)
#endif
        ;
#else
    readProcessOneTcpCommand(client);
#endif
#ifdef LCD
    LedLcdManager::lcdPrint(F("Connection closed!"), true, 0, 0);
#endif
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection closed!"));
#endif
    //client.println(F("Bye"));
    client.stop();

#if defined(COMMANDLED) & defined(LED)
    LedLcdManager::setLogicLed(commandled, LedLcdManager::off);
#endif

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
    LedLcdManager::lcdPrint(F("Connection!"), true, 0, 0);
#endif

#ifdef SESSION
    while
#endif
        (readProcessOneTcpCommand(client))
        ;
#endif // !SERVER
    if (client.connected())
        client.flush();
    client.stop();
#ifdef LCD
    LedLcdManager::lcdPrint(F("Connection closed!"), true, 0, 0);
#endif
#ifdef SERIAL_DEBUG
    Serial.println(F("Connection closed!"));
#endif

#endif // !USEUDP
#else // ! ETHERNET

#ifdef ARDUINO
    Stream& stream = Serial;
 #else
    Stream stream(std::cout);
#endif
   readPrintOneCommand(stream);
#endif // ! ETHERNET
}

#ifndef ARDUINO
int main() {
    setup();
    while (true)
        loop();
}
#endif
