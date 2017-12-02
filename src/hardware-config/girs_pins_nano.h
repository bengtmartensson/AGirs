#pragma once

// Configuration for Arduino Nano with components soldered on the PCB.

#ifndef ARDUINO_AVR_NANO
#error This file is for Nano only
#endif

#define SIGNAL_LED_1 LED_BUILTIN // 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_2_GND A0
#define SIGNAL_LED_3 A3
#define SIGNAL_LED_3_GND A2
#define SIGNAL_LED_4 A5
#define SIGNAL_LED_4_GND A4
//#define SIGNAL_LED_5 A7
//#define SIGNAL_LED_5_GND A6

// capture pin (ICP) 8
//#define IRSENSOR_1_PULLUP
#define IRSENSOR_1_GND 9
#define IRSENSOR_1_VSS 10

#define IRRECEIVER_1_PIN 5
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_1_GND 6
#define IRRECEIVER_1_VSS 7

// Send pin 3
