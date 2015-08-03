// Configuration for Arduino Mega2560

#ifndef GIRS_PINS_MEGA2560_H
#define GIRS_PINS_MEGA2560_H

#ifndef ARDUINO_AVR_MEGA2560
#error This file is for AVR_MEGA2560 only
#endif
#ifdef LED
#define SIGNAL_LED_1     LED_BUILTIN // 13
#define SIGNAL_LED_2     16
#define SIGNAL_LED_3     17
#define SIGNAL_LED_4     30

//#define SIGNAL_LED_5_GND 22
#define SIGNAL_LED_5     22
//#define SIGNAL_LED_6_GND 24
#define SIGNAL_LED_6     24
//#define SIGNAL_LED_7_GND 26
#define SIGNAL_LED_7     28
//#define SIGNAL_LED_8_GND 28
#define SIGNAL_LED_8     32
#endif

#if defined(CAPTURE)
// capture pin (ICP) 49
#define IRSENSOR_1_PIN 49
//#ifdef IRSENSOR_PIN_PULLUP
// not presently implemented
//#define IRSENSOR_2_PIN 48
//#define IRSENSOR_2_PIN_PULLUP
#endif

#ifdef RECEIVE
#define IRRECEIVER_1_PIN 47
//#define IRRECEIVER_1_PIN_PULLUP
#define IRRECEIVER_2_PIN IRSENSOR_1_PIN // Non-demodulating sensor
//#define IRRECEIVER_3_PIN 8 // 56kHz
//#define IRRECEIVER_4_PIN 6 // 433 MHz
//#define IRRECEIVER_5_PIN IRSENSOR_1_PIN // Non-demodulating sensor
#endif

#ifdef TRANSMIT
// Send pin 9
#endif

#ifdef NON_MOD
#define NON_MOD_PIN 39
#endif

#endif // ! GIRS_PINS_MEGA2560_H
