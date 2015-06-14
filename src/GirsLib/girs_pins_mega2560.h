// Configuration for Arduino Mega2560

#ifndef GIRS_PINS_MEGA2560_H
#define GIRS_PINS_MEGA2560_H

#ifndef ARDUINO_AVR_MEGA2560
#error This file is for AVR_MEGA2560 only
#endif
#ifdef LED
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
