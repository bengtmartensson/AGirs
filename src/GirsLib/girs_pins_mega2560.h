// Configuration for Arduino Nano with components soldered on the PCB.

#ifndef GIRS_PINS_MEGA2560_H
#define GIRS_PINS_MEGA2560_H

#ifndef ARDUINO_AVR_MEGA2560
#error This file is for AVR_MEGA2560 only
#endif
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
// capture pin (ICP) 49
#endif

#ifdef RECEIVE
#define IRRECEIVER_PIN 5
#endif
#ifdef RECEIVER_2
//#define IRRECEIVER_2_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 9
#endif

#endif // ! GIRS_PINS_MEGA2560_H
