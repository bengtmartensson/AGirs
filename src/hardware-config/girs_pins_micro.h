#pragma once

// Configuration for Arduino Micro with components soldered on the PCB.

#ifndef ARDUINO_AVR_MICRO
#error This file is for Micro only
#endif

#define SIGNAL_LED_1 2
//#define SIGNAL_LED_1_GND 3
#define SIGNAL_LED_4 A0
#define SIGNAL_LED_4_GND 15
#define SIGNAL_LED_3 A2
#define SIGNAL_LED_3_GND A1
#define SIGNAL_LED_2 A3
//#define SIGNAL_LED_1_GND 8

// capture pin (ICP) 4

//#define IRSENSOR_1_PULLUP
#define IRSENSOR_GND 5
#define IRSENSOR_VSS 6

#define IRRECEIVER_PIN 10
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_GND 16
#define IRRECEIVER_VSS 14

// Send pin 9
