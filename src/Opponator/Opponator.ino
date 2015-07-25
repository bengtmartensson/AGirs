// Dummy file to placify the silly Arduino IDE.
// Do not delete unless you know what you do.

#include "config.h"

#include <GirsMacros.h>

#ifdef ETHERNET
#include <Ethernet.h>
#endif

#ifdef LCD_I2C
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#endif

#ifdef LCD_4BIT
#include <LiquidCrystal.h>
#endif
