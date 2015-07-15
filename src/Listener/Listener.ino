// Dummy file to placify the silly Arduino IDE.
// Do not delete unless you know what you do.

#include "config.h"
#include <GirsMacros.h>

#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#endif

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
