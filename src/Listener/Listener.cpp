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
#include <avr/pgmspace.h>

#include "config.h"
#include <GirsMacros.h>

#ifdef ETHERNET
#ifdef ETHER_ENC28J60
#error ETHER_ENC28J60 not yet supported, only W5100.
#else
#include <Ethernet.h>
#endif
#else // ! ETHERNET
#define serialBaud 115200
#define serialTimeout 5000L
#endif // ! ETHERNET

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#endif

#if  defined(RECEIVE) | defined(TRANSMIT)
#include <IRLib.h>
#include <TrivialDecoder.h>
#endif
#ifdef DECODER
#include <MultiDecoder.h>
#endif

#define VERSION "2015-06-14"

static const unsigned long endingTimeout = 35000UL;
static const unsigned long beginningTimeout = 10000000UL;
#ifdef LCD
static const unsigned long blinkTime = 2500L; // milliseconds
#else
static const unsigned long blinkTime = 500L;
#endif

LCD_DEFINE(lcd);

unsigned long turnoffTime = 0L; // millis() semantic (ms since progarm start)
bool somethingTurnedOn = false;
void updateTurnoffTime() {
  somethingTurnedOn = true;
  turnoffTime = millis() + blinkTime;
}

static void checkTurnoff() {
  if (millis() > turnoffTime && somethingTurnedOn) {
    ALL_LEDS_OFF;
    LCD_OFF(lcd);
    somethingTurnedOn = false;
  }
}

#ifdef LCD
void lcdPrint(String str, bool clear) {
  if (clear)
    lcd.clear();
  lcd.display();
  LCD_BACKLIGHT_ON(lcd);
  lcd.print(str);
  updateTurnoffTime();
}
#endif

#ifdef LED
void blinkAck(uint8_t pin) {
  digitalWrite(pin, HIGH);
  updateTurnoffTime();
}
#endif

IRrecv *irReceiver = NULL;
TrivialIrDecoder decoder;

#ifdef ETHERNET
#ifdef SERVER
EthernetServer server(PORT);
#endif
#ifdef USEUDP
EthernetUDP udp;
IPAddress peer_ip(PEER_IP);
#endif // USEUDP
#endif // ETHERNET

void setup() {
  DEFINE_IRRECEIVER;
  DEFINE_LEDS;
  ALL_LEDS_ON(blinkAck); // self test
#ifdef LCD
  LCD_INIT(lcd);
  lcdPrint(F("GirsListener"), true);
  lcd.setCursor(0, 1);
  lcdPrint(F(VERSION), false);  
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
  byte mac[] = { MACADDRESS };
#ifdef DHCP
  Ethernet.begin(mac);
#else // !DHCP
  Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

#ifdef USEUDP
  udp.begin(PORT);
#else
#ifdef SERVER
  server.begin();
#endif // SERVER
#endif // USEUDP
#else // !ETHERNET

  Serial.begin(serialBaud);
#if defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_AVR_MICRO)
  while (!Serial)
    ; // wait for serial port to connect. "Needed for Leonardo only"
#endif
  Serial.println(F("GirsListener version " VERSION));
  Serial.setTimeout(serialTimeout);
#endif // !ETHERNET

  irReceiver = new IRrecv(IRRECEIVER_PIN);
  irReceiver->setEndingTimeout(endingTimeout);
  irReceiver->setBeginningTimeout(beginningTimeout);
  irReceiver->Mark_Excess = 50;
}

// Read one IR signal.
// If something arrives on the stream, abort and return false.
bool work(Stream& stream) {
  irReceiver->enableIRIn();
  // Wait until something arrives
  while (!(irReceiver->GetResults(&decoder))) {
    checkTurnoff();
    if (stream.available())
      return false;
  }
  // Setup decoder
  decoder.decode();
  // Do actual decode
  MultiDecoder multiDecoder(decoder);
#ifdef LCD
  if (multiDecoder.getType() > MultiDecoder::noise) {
    lcdPrint(multiDecoder.getType() == MultiDecoder::nec_ditto
	     ? F(".") : multiDecoder.getDecode(),
	     multiDecoder.getType() != MultiDecoder::nec_ditto);
    if (multiDecoder.getType() == MultiDecoder::nec)
            lcd.setCursor(0,1); // prepare for dittos
  }
#endif
#ifdef LED
  blinkAck(LED2PIN(multiDecoder.getType()+1));
#endif
#ifdef USEUDP
  udp.beginPacket(peer_ip, PEER_PORT);
#endif
  stream.println(multiDecoder.getDecode());
#ifdef USEUDP
  udp.endPacket();
#endif
  irReceiver->disableIRIn();
  return true;  
}

void loop() {
#ifdef ETHERNET

#ifdef USEUDP
 work(udp);
#else // !USEUDP
  checkTurnoff();
#ifdef SERVER
  EthernetClient client = server.available();
  if (!client)
    return;
  client.println(F("GirsListener"));
  
  while (client.read() != -1)
    checkTurnoff();
  while (work(client))
    ;
  
  client.println(F("Bye"));
#else // !SERVER
  IPAddress peer(PEER_IP);
  EthernetClient client;
  boolean status = client.connect(peer, PEER_PORT);
  if (!status)
      return;

  //blinkAck(SIGNAL_LED_8);
  while(work(client))
    ;
#endif // !SERVER
  client.flush();
  client.stop();
  
#endif // !USEUDP
#else // ! ETHERNET
  work(Serial);
#endif // !ETHERNET
}
