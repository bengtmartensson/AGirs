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

#define RECEIVE
#define DECODE
#define LED

#ifdef ARDUINO_AVR_NANO
#define LCD_I2C
//#include <girs_pins_nano_shield.h>
#include <girs_pins_nano_lcd.h>
#else
//#define LCD
#define LCD_I2C
//#include <girs_pins_lcdshield.h>
#include <girs_pins.h>
#endif

#define ETHERNET
//#ifdef ARDUINO_AVR_MEGA2560
//#define DHCP
//#endif

#include <GirsMacros.h>
#define VERSION "2015-06-06"

#ifdef LCD_I2C
//#include <lcd_0x27_16_2.h>
#include <lcd_0x3F_20_4.h>
#endif

static const unsigned long endingTimeout = 35000UL;
static const unsigned long beginningTimeout = 10000000UL;
#if defined(LCD_I2C) | defined(LCD)
static const unsigned long blinkTime = 2500L; // milliseconds
#else
static const unsigned long blinkTime = 250L;
#endif

#ifdef ETHERNET
#define MACADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#ifndef DHCP
#define IPADDRESS  192,168,1,29
#define GATEWAY    192,168,1,254
#define DNSSERVER  192,168,1,4
#define SUBNETMASK 255,255,255,0
#endif // DHCP
#define PORT       33333

#ifdef ARDUINO_AVR_NANO
#include <UIPEthernet.h>
#else
#include <Ethernet.h>
#endif
#else
#define serialBaud 115200
#define serialTimeout 5000L
#endif

#include "IRLib.h"
#include "Nec1Decoder.h"
#include "Rc5Decoder.h"

#ifdef LCD_I2C
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
#else
LiquidCrystal lcd(LCD_INIT_ARGS);
#endif

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

#if defined(LCD_I2C) | defined(LCD)
void lcdPrint(String str, bool clear) {
  if (clear)
    lcd.clear();
  lcd.display();
#ifdef LCD_I2C
  lcd.backlight();
#else
  digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
#endif
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
#ifdef LCD_I2C
  lcd.begin();
  lcdPrint(F("GirsListener"), true);
#endif
#ifdef LCD
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  lcdPrint(F("GirsListener"), true);
#endif
  
#ifdef ETHERNET
  // disable the SD card, as recommended in the doc
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
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
#if defined(LCD_I2C) | defined(LCD)
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
#if defined(LCD_I2C) | defined(LCD)
    lcdPrint(s, true);
#endif
    BLINK_LED_3(blinkAck);
    return;
  }
  // undecoded
  stream.println(F("undecoded"));
#if defined(LCD_I2C) | defined(LCD)
  lcdPrint(F("*** unknown: "), true);
  lcdPrint(String(decoder.getRawlen()), false);
#endif
  //BLINK_LED_2;
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
