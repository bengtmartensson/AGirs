// Configuration for Arduino Micro with components soldered on the PCB.

#ifndef GIRS_PINS_MICRO_H
#define GIRS_PINS_MICRO_H

#ifndef ARDUINO_AVR_MICRO
#error This file is for Micro only
#endif

#ifdef LED
#define SIGNAL_LED_1 2
#define SIGNAL_LED_1_GND 3
#define SIGNAL_LED_2 A1
#define SIGNAL_LED_2_GND A0
#define SIGNAL_LED_3 A3
#define SIGNAL_LED_3_GND A2
#define SIGNAL_LED_4 7
#define SIGNAL_LED_4_GND 8
#endif

#if defined(CAPTURE)
// capture pin (ICP) 4
#define SENSOR_GND 5
#define SENSOR_VSS 6
#endif

#ifdef RECEIVE
#define IRRECEIVER_PIN 10
#define IRRECEIVER_GND 16
#define IRRECEIVER_VSS 14
#endif

#ifdef TRANSMIT
// Send pin 9
#endif

#endif // ! GIRS_PINS_MICRO_H
