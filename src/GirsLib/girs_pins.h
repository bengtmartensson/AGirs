#ifndef GIRS_PINS_H
#define GIRS_PINS_H

#if defined(ARDUINO_AVR_NANO)
// Configuration for Arduino Nano with components soldered on the PCB.
#include "girs_pins_nano.h"
#elif defined(ARDUINO_AVR_MICRO)
// Configuration for Arduino Micro with components soldered on the PCB.
#include "girs_pins_micro.h"
#elif defined(ARDUINO_AVR_MEGA2560)
#include "girs_pins_mega2560.h"
#elif defined(ARDUINO_AVR_LEONARDO)
#include "girs_pins_leonardo.h"
#else
// Generic configuration for Uno-like boards
#include "girs_pins_default.h"
#endif

#endif // ! GIRS_PINS_H
