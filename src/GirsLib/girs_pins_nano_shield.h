// Configuration for Arduino Nano with components soldered on the PCB.

#ifndef GIRS_PINS_NANO_H
#define GIRS_PINS_NANO_H

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
#ifndef ETHERNET
#define SENSOR_GND 9
#define SENSOR_VSS 10
#endif
#endif

#ifdef RECEIVE
#define IRRECEIVER_PIN 5
#define IRRECEIVER_GND 6
#define IRRECEIVER_VSS 7
#endif
#ifdef RECEIVER_2
#define IRRECEIVER_2_PIN 12
#endif

#ifdef TRANSMIT
// Send pin 3
#endif

#endif // ! GIRS_PINS_NANO_H
