/**
 * @file config.h
 * @brief User configuration file.
 * This file determines the configuration of the built program.
 * Feel free to be creative here; you can include pre-built files, you can replace
 * the file with your content, you can include pre-built files and override values
 * in them, or ...
 */

#pragma once

#ifdef ARDUINO_AVR_UNO
// Support sending signals without modulation, e.g. with RF module.
#define NON_MOD
#endif

#if defined(ARDUINO_AVR_UNO) | defined(ARDUINO_AVR_NANO) | defined(ARDUINO_AVR_MICRO) | defined(ARDUINO_AVR_LEONARDO)

#include "GirsLite.config.h"

#elif defined(ARDUINO_AVR_MEGA2560)

#include "GirsFat.config.h"

#elif ! defined(ARDUINO)

#else

#error Your platform is currently not supported. Please extend examples/Girs/config.h according to your needs and wishes.

#endif