#pragma once

// Configuration for Arduino Mega2560

#ifndef ARDUINO_AVR_MEGA2560
#error This file is for AVR_MEGA2560 only
#endif

#define SIGNAL_LED_1     LED_BUILTIN // 13
#define SIGNAL_LED_2     A1
#define SIGNAL_LED_3     A2
#define SIGNAL_LED_4     A3

#define SIGNAL_LED_5_GND 22
#define SIGNAL_LED_5     23
#define SIGNAL_LED_6_GND 24
#define SIGNAL_LED_6     25
#define SIGNAL_LED_7_GND 26
#define SIGNAL_LED_7     27
#define SIGNAL_LED_8_GND 28
#define SIGNAL_LED_8     29

// capture pin (ICP) 49

// send pin 9

#define IRSENSOR_1_PIN 49
//#ifdef IRSENSOR_1_PULLUP
// not presently implemented
#define IRSENSOR_2_PIN 48
#define IRSENSOR_2_PULLUP

#define IRRECEIVER_1_PIN 5
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_2_PIN 7 // 38kHz, ACG4
#define IRRECEIVER_3_PIN 8 // 56kHz
#define IRRECEIVER_4_PIN 6 // 433 MHz
#define IRRECEIVER_5_PIN 49 //IRSENSOR_1_PIN // Non-demodulating sensor

#define NON_MOD_PIN 3

/* From http://busyducks.com/ascii-art-arduinos                              +-----+

         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |           GND/RST2  [ ] [ ]         |
         |         MOSI2/SCK2  [ ] [ ]  SCL[ ] |   D0
         |            5V/MISO2 [ ] [ ]  SDA[ ] |   D1
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ]~|   B7
         | [ ]v.ref                 MISO/12[ ]~|   B6
         | [ ]RST                   MOSI/11[ ]~|   B5
         | [ ]3V3      +----------+      10[ ]~|   B4
         | [ ]5v       | ARDUINO  |       9[ ]~|   H6
         | [ ]GND      |   MEGA   |       8[ ]~|   H5
         | [ ]GND      +----------+            |
         | [ ]Vin                         7[ ]~|   H4
         |                                6[ ]~|   H3
         | [ ]A0                          5[ ]~|   E3
         | [ ]A1                          4[ ]~|   G5
         | [ ]A2                     INT5/3[ ]~|   E5
         | [ ]A3                     INT4/2[ ]~|   E4
         | [ ]A4                       TX>1[ ]~|   E1
         | [ ]A5                       RX<0[ ]~|   E0
         | [ ]A6                               |
         | [ ]A7                     TX3/14[ ] |   J1
         |                           RX3/15[ ] |   J0
         | [ ]A8                     TX2/16[ ] |   H1
         | [ ]A9                     RX2/17[ ] |   H0
         | [ ]A10               TX1/INT3/18[ ] |   D3
         | [ ]A11               RX1/INT2/19[ ] |   D2
         | [ ]A12           I2C-SDA/INT1/20[ ] |   D1
         | [ ]A13           I2C-SCL/INT0/21[ ] |   D0
         | [ ]A14                              |
         | [ ]A15                              |   Ports:
         |                RST SCK MISO         |    22=A0  23=A1
         |         ICSP   [ ] [ ] [ ]          |    24=A2  25=A3
         |                [ ] [ ] [ ]          |    26=A4  27=A5
         |                GND MOSI 5V          |    28=A6  29=A7
         | G                                   |    30=C7  31=C6
         | N 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    32=C5  33=C4
         | D 2 0 8 6 4 2 0 8 6 4 2 0 8 6 4 2 V |    34=C3  35=C2
         |         ~ ~                         |    36=C1  37=C0
         | @ # # # # # # # # # # # # # # # # @ |    38=D7  39=G2
         | @ # # # # # # # # # # # # # # # # @ |    40=G1  41=G0
         |           ~                         |    42=L7  43=L6
         | G 5 5 4 4 4 4 4 3 3 3 3 3 2 2 2 2 5 |    44=L5  45=L4
         | N 3 1 9 7 5 3 1 9 7 5 3 1 9 7 5 3 V |    46=L3  47=L2
         | D                                   |    48=L1  49=L0    SPI:
         |                                     |    50=B3  51=B2     50=MISO 51=MOSI
         |     2560                ____________/    52=B1  53=B0     52=SCK  53=SS
          \_______________________/

*/
