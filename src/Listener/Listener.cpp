/*
Copyright (C) 2014, 2015, 2016 Bengt Martensson.

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

#include "config.h"
#include "LedLcdManager.h"
#include <GirsUtils.h>
#include <IrReceiverSampler.h>
#ifdef DECODEIR
#include <DecodeIRClass.h>
#endif

#ifdef ETHERNET
#include <Ethernet.h>
#include <IPAddress.h>
#ifdef BEACON
#include <Beacon.h>
#endif
#endif // ETHERNET

static const milliseconds_t beginTimeout = BEGINTIMEOUT; // not really relevant here
static const milliseconds_t endingTimeout = ENDINGTIMEOUT;
static const size_t captureSize = CAPTURESIZE;
static const microseconds_t markExcess = IRRECEIVER_MARK_EXCESS;

#ifdef RECEIVELED
static LED_PARAMETER_CONST led_t receiveled = RECEIVELED;
#endif

static IrReceiver *irReceiver = NULL;

#define PROGNAME "Listener"
#define VERSION "2017-05-12"

#ifdef ETHERNET
#ifdef USEUDP
static EthernetUDP udp;
static IPAddress broadcastIp(BROADCAST_IP);
#else // !USEUDP
#error only UDP supported
#endif // !USEUDP
#endif // ETHERNET

static char decode[100];

static void readOneDecode() {
    decode[0] = '\0';
#ifdef RECEIVELED
    LedLcdManager::setLogicLed(receiveled, LedLcdManager::on);
#endif
    irReceiver->enable();
    // Wait until something arrives
    while (!irReceiver->isReady())
        LedLcdManager::checkTurnoff();

#ifdef RECEIVELED
    LedLcdManager::setLogicLed(receiveled, LedLcdManager::off);
#endif
    int type;

#ifdef DECODEIR
    IrSequence *irSequence = irReceiver->toIrSequence();
    DecodeIRClass decoder(*irSequence, IrSignal::defaultFrequency);
    delete irSequence;
    if (decoder.getProtocol()[0] != '\0') {
#ifdef LCD
        LedLcdManager::lcdPrint(decoder.getString1(), true, 0, 0);
        LedLcdManager::lcdPrint(decoder.getString2(), false, 0, 1);
#endif
        strcpy(decode, decoder.getString1());
        strcat(decode, decoder.getString2());
    }
    type = strncmp(decoder.getProtocol(), "NEC", 3) == 0 ? 3
            : strncmp(decoder.getProtocol(), "RC5", 3) == 0 ? 4
            : decoder.getProtocol()[0] != 0 ? 5 : 0;
#else // ! DECODEIR

    MultiDecoder multiDecoder(*irReceiver);
#ifdef LCD
    if (multiDecoder.getType() > MultiDecoder::noise) {
        LedLcdManager::lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
                ? "." : multiDecoder.getDecode(),
                multiDecoder.getType() != MultiDecoder::nec_ditto);
        if (multiDecoder.getType() == MultiDecoder::nec)
            LedLcdManager::lcdSetCursor(0, 1); // prepare for dittos
    }
#endif
    type = multiDecoder.getType();
    strncpy(decode, multiDecoder.getDecode(), 100);

#endif // ! DECODEIR
    LedLcdManager::setLogicLed(GirsUtils::decode2logicalLed(type), LedLcdManager::blink);
    irReceiver->disable();
}

void setup() {
    LedLcdManager::setupLedGroundPins();
    GirsUtils::setupReceivers();
    GirsUtils::setupLeds();
    LedLcdManager::setup(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT,
            (const pin_t[]) {SIGNAL_LED_1, SIGNAL_LED_2, SIGNAL_LED_3, SIGNAL_LED_4,
                    SIGNAL_LED_5, SIGNAL_LED_6, SIGNAL_LED_7, SIGNAL_LED_8 });
    LedLcdManager::selfTest(F(PROGNAME " " VERSION));
#ifdef LED
    LedLcdManager::setupShouldTimeout(receiveled, false);
#endif

#ifdef LCD
#ifdef ETHERNET
    LedLcdManager::lcdPrint(GirsUtils::ip2string(broadcastIp) + ":" + BROADCAST_PORT, false, 0, 1);
#else // ! ETHERNET
    LedLcdManager::lcdPrint(F("Serial"), false, 0, 1);
#endif // ! ETHERNET
#endif // LCD

#ifdef ETHERNET
#ifdef SDCARD_ON_ETHERSHIELD_PIN
    // disable the SD card, as recommended in the doc
    pinMode(SDCARD_ON_ETHERSHIELD_PIN, OUTPUT);
    digitalWrite(SDCARD_ON_ETHERSHIELD_PIN, LOW);
#endif

    byte mac[] = { MACADDRESS };

    Ethernet.begin(mac
#ifndef DHCP
    , IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK)
#endif // !DHCP
    );

    String ipstring = GirsUtils::ip2string(Ethernet.localIP());
    LedLcdManager::lcdPrint(ipstring, false, 0, 2);

#ifdef BEACON
    Beacon::setup(PROGNAME, "DE-AD-BE-EF-FE-ED", "Utility", "www.harctoolbox.org",
            "", "", "", "http://arduino/nosuchfile.html");
#endif

#define DUMMYPORT 8888
    udp.begin(DUMMYPORT);

#else // ! ETHERNET

    Serial.begin(SERIALBAUD);
    Serial.setTimeout(SERIALTIMEOUT);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
    while (!Serial)
        ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
    Serial.println(F(PROGNAME " " VERSION));

#endif // ! ETHERNET

    irReceiver = IrReceiverSampler::newIrReceiverSampler(captureSize, IRRECEIVER_1_PIN,
            IRRECEIVER_1_PULLUP_VALUE, markExcess, beginTimeout, endingTimeout);
}

void loop() {
    LedLcdManager::checkTurnoff();
#ifdef BEACON
    Beacon::checkSend();
#endif
    readOneDecode();
    if (decode[0] != '\0') {
#ifdef ETHERNET
        udp.beginPacket(broadcastIp, BROADCAST_PORT);
        udp.write(decode);
        udp.write(EOLCHAR);
        udp.endPacket();
#else // ! ETHERNET
        Serial.println(decode);
#endif // ! ETHERNET
    }
}
