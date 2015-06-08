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
#include <UIPEthernet.h>
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
#include "IRLib.h"
#endif
#ifdef DECODER
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"
#endif

#define VERSION "2015-06-07"

static const unsigned long endingTimeout = 35000UL;
static const unsigned long beginningTimeout = 10000000UL;
#ifdef LCD
static const unsigned long blinkTime = 2500L; // milliseconds
#else
static const unsigned long blinkTime = 250L;
#endif

LCD_DEFINE(lcd);

unsigned long turnoffTime = 0L; // millis() semantic (ms since progarm start)
bool somethingTurnedOn = false;
void updateTurnoffTime() {
  somethingTurnedOn = true;
  turnoffTime = millis() + blinkTime;
}

void turnoff() {
  ALL_LEDS_OFF;
  LCD_OFF(lcd);
  somethingTurnedOn = false;
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

// Need a trivial IRdecodeBase instance, but that class is abstract...
class TrivialIrDecoder : public IRdecodeBase {
  public:
  boolean isTimeOut() {
    return rawlen == 1;
  }
  unsigned char getRawlen() {
    return rawlen;
  }
};

IRrecv *irReceiver = NULL;
TrivialIrDecoder decoder;

#ifdef ETHERNET
EthernetServer server(PORT);
#endif

void setup() {
  DEFINE_IRRECEIVER;
  DEFINE_LEDS;
  ALL_LEDS_ON(blinkAck); // Just as a test
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
  pinMode(53, OUTPUT);
  digitalWrite(53, LOW);
#endif
  byte mac[] = { MACADDRESS };
#ifdef DHCP
  Ethernet.begin(mac);
#else // !DHCP
  Ethernet.begin(mac, IPAddress(IPADDRESS), IPAddress(DNSSERVER), IPAddress(GATEWAY), IPAddress(SUBNETMASK));
#endif // !DHCP

  server.begin();
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

void decode(Stream& stream) {
  if (decoder.isTimeOut()) {
    stream.println(F("."));
    return;
  }
  if (decoder.getRawlen() < 4) { // Probably noise
    stream.println(F(":"));
    return;
  }
  
  Nec1Decoder nec1decoder(decoder);
  if (nec1decoder.isValid()) {
    String s = nec1decoder.toString();
    stream.println(s);
#ifdef LCD
    if (nec1decoder.isDitto()) {
      lcdPrint(".", false);
    } else {
      lcdPrint(s, true);
      lcd.setCursor(0,1); // prepare for dittos
    }
#endif
    BLINK_LED_4(blinkAck);
    return;
  }
  Rc5Decoder rc5decoder(decoder);
  if (rc5decoder.isValid()) {
    String s = rc5decoder.toString();
    stream.println(s);
#ifdef LCD
    lcdPrint(s, true);
#endif
    BLINK_LED_3(blinkAck);
    return;
  }
  // undecoded
  stream.println(F("undecoded"));
#ifdef LCD
  lcdPrint(F("*** unknown: "), true);
  lcdPrint(String(decoder.getRawlen()), false);
#endif
  BLINK_LED_2(blinkAck);
}

bool work(Stream& stream) {
  irReceiver->enableIRIn();
  while (!(irReceiver->GetResults(&decoder))) {
    if (millis() > turnoffTime && somethingTurnedOn)
      turnoff();
    if (stream.available())
      return false;
  }
  decoder.decode();
  decode(stream);
  irReceiver->disableIRIn();
  return true;  
}

void loop() {
#ifdef ETHERNET
  if (millis() > turnoffTime && somethingTurnedOn)
    turnoff();
  EthernetClient client = server.available();
  if (!client)
    return;
  BLINK_LED_2(blinkAck);
  client.println(F("GirsListener"));
  while (client.read() != -1)
    if (millis() > turnoffTime && somethingTurnedOn)
      turnoff();

  do
    ;
  while (work(client));
  
  client.println(F("Bye"));
  client.flush();
  client.stop();
#else
  work(Serial);
#endif
}

