#include "config.h"

#include <IRLib.h>
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