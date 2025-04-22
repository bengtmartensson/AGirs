/**
 * @file examples/Girs/config.h
 * @brief User configuration file.
 * This file determines the configuration of the built program.
 * Feel free to be creative here; you can include pre-built files, you can replace
 * the file with your content, you can include pre-built files and override values
 * in them, or ...
 */

#pragma once

#include "Board.h"

#ifdef __AVR_ATmega168__
#error Unsupported CPU ATmega168 (too little memory)
#endif

#ifdef ARDUINO_AVR_UNO
// Support sending signals without modulation, e.g. with RF module.
#define NON_MOD
#endif

#if defined(ARDUINO_AVR_UNO) | defined(ARDUINO_AVR_NANO) | defined(ARDUINO_AVR_MICRO) | defined(ARDUINO_AVR_LEONARDO) | defined(ARDUINO_SAMD_ZERO)

#include "GirsLite.config.h"

#elif defined(ARDUINO_AVR_MEGA2560)

#include "GirsFat.config.h"

#elif defined(ESP32)

#warning This platform is in active development and nothing is known to work yet.

#include "GirsFat.ESP32.config.h"

#else

#error Your platform is currently not supported. Please extend examples/Girs/config.h according to your needs and wishes.

#endif
