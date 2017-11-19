// Configuration for Arduino * with LCD shield.
// Makes very little sense on non-Uno's.

#ifdef ARDUINO_AVR_NANO
#error This file is not for Nano
#elif defined(ARDUINO_AVR_MICRO)
#error This file is not for Micro
#endif

//#include <lcd_4bit_16_2.h>

#ifdef LED
#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_3 A2
#define SIGNAL_LED_4 A3
//#define SIGNAL_LED_5 A4
//#define SIGNAL_LED_6 A5
//#define SIGNAL_LED_7 6
//#define SIGNAL_LED_8 7
#endif

#if defined(CAPTURE)
// capture pin 8
#endif

#ifdef RECEIVE
#define IRRECEIVER_1_PIN 11
#endif
#ifdef RECEIVER_2
#define IRRECEIVER_2_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 3
#endif
