#pragma once

// Configuration for Arduino Uno and similar.

#ifdef ARDUINO_AVR_NANO
#error This file is not for Nano
#elif defined(ARDUINO_AVR_MICRO)
#error This file is not for Micro
#endif

#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_3 A2
#define SIGNAL_LED_4 A3
#define SIGNAL_LED_5 A0
#define SIGNAL_LED_6 6
#define SIGNAL_LED_7 7
#define SIGNAL_LED_8 108 // dummy
#ifndef NON_MOD
//#define SIGNAL_LED_8 9
#endif

#define IRSENSOR_1_PIN 8
// capture pin (ICP) 8/4/49
//#define IRSENSOR_1_PULLUP

#define IRRECEIVER_1_PIN 5
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_2_PIN 4
#define IRRECEIVER_3_PIN 9
#define IRRECEIVER_4_PIN 8 // Non-demodulating sensor
//#define IRRECEIVER_4_PIN IRSENSOR_1_PIN // Non-demodulating sensor

// Send pin 3/9

#define NON_MOD_PIN 10

/* From http://busyducks.com/ascii-art-arduinos                             +-----+

         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   B5
         | [ ]v.ref                 MISO/12[ ] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[ ]~|   .
         | [ ]5v     | A |                9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |   B0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   D7
         |          -| I |-               6[ ]~|   .
         | [ ]A0    -| N |-               5[ ]~|   .
         | [ ]A1    -| O |-               4[ ] |   .
         | [ ]A2     +---+           INT1/3[ ]~|   .
         | [ ]A3                     INT0/2[ ] |   .
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/

*/
