// Configuration for my Arduino Nano shield.

#ifndef ARDUINO_AVR_NANO
#error This file is for Nano only
#endif

#ifdef LED
#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_3 A2
#define SIGNAL_LED_4 A3
#endif

#if defined(CAPTURE)
// capture pin (ICP) 8
#endif

#ifdef RECEIVE
#define IRRECEIVER_1_PIN 5
#endif
#ifdef RECEIVER_2
#define IRRECEIVER_2_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 3
#endif

#ifdef NON_MOD
#define NON_MOD_PIN 9
#endif
