#pragma once

// Dummy pin Configuration

#ifdef ARDUINO
#error This file is not for flashing on Arduino
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
#define IRSENSOR_1_PIN 49
#define IRSENSOR_1_PULLUP
// not presently implemented
#define IRSENSOR_2_PIN 48
#define IRSENSOR_2_PULLUP

#define IRRECEIVER_1_PIN 5
#define IRRECEIVER_1_PULLUP
#define IRRECEIVER_2_PIN 7 // 38kHz, ACG4
#define IRRECEIVER_3_PIN 8 // 56kHz
#define IRRECEIVER_4_PIN 6 // 433 MHz
#define IRRECEIVER_5_PIN 49 //IRSENSOR_1_PIN // Non-demodulating sensor

#define NON_MOD_PIN 3
