#pragma once

// Configuration for Arduino Nano with components soldered on the PCB.

#ifndef ARDUINO_AVR_NANO
#error This file is for Nano only
#endif

#define SIGNAL_LED_1        LED_BUILTIN // 13
#define SIGNAL_LED_2        A1
#define SIGNAL_LED_2_GND    A0
#define SIGNAL_LED_3        A3
#define SIGNAL_LED_3_GND    A2
#define SIGNAL_LED_4        A5
#define SIGNAL_LED_4_GND    A4
//#define SIGNAL_LED_5      A7
//#define SIGNAL_LED_5_GND  A6

// capture pin (ICP)        8
//#define IRSENSOR_1_PULLUP
#define IRSENSOR_1_GND      9
#define IRSENSOR_1_VCC      10

#define IRRECEIVER_1_PIN    5
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_1_GND    6
#define IRRECEIVER_1_VCC    7

// Send pin 3

/* From http://busyducks.com/ascii-art-arduinos

                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[ ]~|   B3
         | [ ]V.ref     ___    SS/D10[ ]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [ ]A4/SDA               D5[ ]~|   D5
    C5   | [ ]A5/SCL               D4[ ] |   D4
         | [ ]A6              INT1/D3[ ]~|   D3
         | [ ]A7              INT0/D2[ ] |   D2
         | [ ]5V                  GND[ ] |
    C6   | [ ]RST                 RST[ ] |   C6
         | [ ]GND   5V MOSI GND   TX1[ ] |   D0
         | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |   D1
         |          [ ] [ ] [ ]          |
         |          MISO SCK RST         |
         | NANO-V3                       |
         +-------------------------------+
*/
