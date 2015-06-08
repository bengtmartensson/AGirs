// Configuration for Arduino Nano with receiver on the ICSP pin

#ifndef GIRS_PINS_NANO_H
#define GIRS_PINS_NANO_H

#ifndef ARDUINO_AVR_NANO
#error This file is for Nano only
#endif

#ifdef LED
#define SIGNAL_LED_1 13
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_2_GND A0
#define SIGNAL_LED_3 A3
#define SIGNAL_LED_3_GND A2
#define SIGNAL_LED_4 11
#endif

#if defined(CAPTURE)
// capture pin (ICP) 8
#endif

#ifdef RECEIVE
#define IRRECEIVER_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 3
#endif

#endif // ! GIRS_PINS_NANO_H
