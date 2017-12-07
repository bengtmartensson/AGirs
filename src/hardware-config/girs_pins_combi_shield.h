#pragma once

// Configuration for a shield for Arduino Uno and similar.

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
#define SIGNAL_LED_8 2

#define IRSENSOR_1_PIN 8
// capture pin (ICP) 8/4/49
//#define IRSENSOR_1_PULLUP

#define IRRECEIVER_1_PIN 5
//#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_2_PIN 11
#define IRRECEIVER_3_PIN 12
//#define IRRECEIVER_4_PIN 8 // Non-demodulating sensor
//#define IRRECEIVER_4_PIN IRSENSOR_1_PIN // Non-demodulating sensor

// Send pin 3/9

#define NON_MOD_PIN 10
