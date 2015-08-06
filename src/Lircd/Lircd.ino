// Dummy file to placify the silly Arduino IDE.
// The real program resides in Girs.cpp.
// Rationale: see http://www.gammon.com.au/forum/?id=12625

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
