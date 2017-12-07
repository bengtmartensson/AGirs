#pragma once

// Configuration for Arduino Leonardo

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
#define SIGNAL_LED_8 8

// capture pin (ICP) 4

#define IRSENSOR_1_PIN 4

#define IRRECEIVER_1_PIN 5
#define IRRECEIVER_2_PIN 12

// Send pin 3/9
