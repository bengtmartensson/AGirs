// Configuration for Arduino Uno and similar.

#ifndef GIRS_PINS_DEFAULT_H
#define GIRS_PINS_DEFAULT_H

#ifdef ARDUINO_AVR_NANO
#error This file is not for Nano
#elif defined(ARDUINO_AVR_MICRO)
#error This file is not for Micro
#endif

#ifdef LED
#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_3 A2
#define SIGNAL_LED_4 A3
#define SIGNAL_LED_5 A0
#define SIGNAL_LED_6 6
#define SIGNAL_LED_7 7
#define SIGNAL_LED_8 9
#endif

#if defined(CAPTURE)
// capture pin (ICP) 8/4/49
#endif

#ifdef RECEIVE
#define IRRECEIVER_PIN 5
#endif
#ifdef RECEIVER_2
#define IRRECEIVER_2_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 3/9
#endif

#endif // ! GIRS_PINS_DEFAULT_H
