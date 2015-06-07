// Configuration for Arduino Nano with components soldered on the PCB.

#ifndef GIRS_PINS_H
#define GIRS_PINS_H

#if defined(ARDUINO_AVR_NANO)
#include "girs_pins_nano.h"
#elif defined(ARDUINO_AVR_MICRO)
#include "girs_pins_micro.h"
#elif defined(ARDUINO_AVR_MEGA2560)
#include "girs_pins_mega2560.h"
#elif defined(ARDUINO_AVR_LEONARDO)
#include "girs_pins_leonardo.h"
#elif defined(ARDUINO_AVR_PRO)
#include "girs_pins_pro.h"
#else
#include "girs_pins_default.h"
#endif

#endif // ! GIRS_PINS_H
